#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <windows.h>  // affichage console wd
#include <time.h>    // rand



// ===================== Config / Globals =====================
//res screen
int largeur = 69; // screen screen main
int hauteur = 19;

char fill = ' ';

// 0 = Exploration | 1 = combat | 2 = map | 3 = inventaire | 4 = tresor
int screen_status = 0;
int previous_screen_status = 0;

// Axes pour conversion coords
const char *X_AXIS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!?@#$^&";
const char *Y_AXIS = "0123456789ABCDEFGHI";

// Pour calcul de distance parcourue => conso en O2 | dans le plongeur direct
// int* player_last_pos_y;
// int* player_last_pos_x;

// init info bull (str)
char* info = " ";

// ===================== Types =====================
typedef struct {
    int points_de_vie;
    int points_de_vie_max; // 0 a 100

    int niveau_oxygene;
    int niveau_oxygene_max; // 0 a 100

    int niveau_fatigue;
    int niveau_fatigue_max; // 0 à 5 => ramené en % => 0 à 100

    int defense;
    int vitesse;

    int perles; // monnaie du jeu

    int id_arme_equipe; // id unique de l'arme
    int id_equipement_equipe;

    int pos_y;
    int pos_x;
    int last_pos_y;
    int last_pos_x;
} Plongeur;

typedef struct {
    int id; // identifiant unique pour cibler
    char nom[30];

    int points_de_vie_max;
    int points_de_vie_actuels;

    int attaque_minimale;
    int attaque_maximale;

    int defense;
    int vitesse;

    char effet_special[20]; // "paralysie", "poison", "aucun"
    int est_vivant;
} CreatureMarine;

typedef struct {
    int id; // id unique de l'arme
    int id_type; // id du type de l'arme (si id_type = 0 => c'est un harpon rouillé, il a entre x et x de dmg
    char nom[30];

    int rarete; // 0 - 5

    int degats_min; // debat min de l'arme (fourchette aleatoire a la generation de l'arme)
    int degats_max; // degat max de l'arme (fourchette aleatoire a la generation de l'arme)
    int consomation; // conso d'02 par attaque

    int special;
} Arme;

typedef struct {
    int id;
    char nom[30];
    int rarete; // 0 - 5
    int defense;
} Equipement; // Armure (combinaison)

// ===================== Prototypes =====================
char **init_screen();
void print_screen(char **screen);

void ajout_joueur_combat_screen(char **screen);
void ajout_creature_combat_screen(char** screen, CreatureMarine *c);

void full_screen(Plongeur *p, CreatureMarine *c, char **screen, char* info);

void screen_header(Plongeur *p, char* pv_bar, char* oxy_bar, char* fatigue_bar, char* info);
void screen_main(Plongeur *p, CreatureMarine *c, char** screen);
void screen_footer();

char* convert_to_visual_bar(int stat);

void demander_player_for_coords(char** screen, Plongeur *p);
void action_apres_deplacement(Plongeur *p, CreatureMarine *c, int y, int x, char **screen);

// ======= Utilitaires =======
//converti les coord char en cord int lisible par screen[][]
int index_in_axis(char c, const char *axis) {
    const char *p = strchr(axis, c);
    if (p != NULL) {
        return (int)(p - axis); // position du caract
    } else {
        return -1; // caracte non trouvé
    }
}
char to_upper_ascii(char c) {
    if (c >= 'a' && c <= 'z') {
        return (char)(c - ('a' - 'A')); // convertir en majuscule
    } else {
        return c; // sinon return la lettre (on touche a rien)
    }
}

// Clear la grille (premiere couche d'affichage)
void clear_screen_grid(char **screen, char fill_char){
    for (int i = 0; i < hauteur; i++)
        for (int j = 0; j < largeur; j++)
            screen[i][j] = fill_char;
}

// base Base (dessous (decors/choses)) de l'ecran d'exploration
void layer_explo_base(char **screen){
    // pour l’instant : juste le fond vide (le rien)
    // (déjà géré par clear_screen_grid) => donc rien rien
    (void)screen;
}

// Evite d'avoir une valeur de stats en dehors des limites
int clamp(int valeur, int max) {
    if (valeur < 0){
        return 0;
    }
    if (valeur > max){
        return max;
    }
    return valeur;

}

// couche d'affichage des fleches de sortie de Case map
void layer_explo_arrows(char **screen){
    screen[0][largeur/2] = '^';
    screen[hauteur/2][0] = '<';
    screen[hauteur-1][largeur/2] = 'v';
    screen[hauteur/2][largeur-1] = '>';

    screen[5][15] = 'E';
}
void layer_player(Plongeur *p, char **screen){
    // place le joueur
    if (p->pos_y >= 0 && p->pos_y < hauteur && p->pos_x >= 0 && p->pos_x < largeur){
        screen[p->pos_y][p->pos_x] = '@';
    }
}

// === Validation entrée user ===

// Vérifie si un caract est la liste
int char_in(char *liste, char c) {
    for (int i = 0; liste[i] != '\0'; i++) {
        if (liste[i] == c) {
            return 1;
        }
    }
    return 0;
}

// Retourne la liste des touches valides selon l'écran courant
// (tu peux ajuster au fur et à mesure que tu ajoutes des écrans)
char* saisies_utilisateur_autorise(int status) {
    switch (status) {
        case 0:  return "CDIcdi";       // Exploration: [C] Carte, [I] Inventaire, [D] Déplacer
        case 1:  return "ABIFabif";    // Combat: A,B,C, [I] Inventaire, [F] Fuire
        case 2:  return "Qq";         // Carte: quitter retour exploration
        case 3:  return "Qq";        // Inventaire: quitter retour exploration
        case 4:  return "Qq";       // Trésor: quitter (exemple)
        default: return "";        // Par défaut rien
    }
}

// Prompt + validation : redemande tant que la touche n'est pas autorisée
char prompt_for_command(int status) {
    char *allowed = saisies_utilisateur_autorise(status);
    char input[16];

    // for(;;) => boucle infinie (apparement c'est mieux que faire un while infinie)
    for (;;) {
        printf("Que souhaitez vous faire : ");
        if (scanf("%15s", input) != 1) {
            // entrée EOF/erreur -> renvoyer un code neutre
            return '\0';
        }
        if (strlen(input) != 1) {
            printf("Entrez UN seul caractère.\n");
            continue;
        }
        char c = input[0];
        if (!char_in(allowed, c)) {
            printf("Touche non disponible sur cet écran.\n");
            continue;
        }
        return c;
    }
}

// ===================== MAIN =====================
int main(void) {
    SetConsoleOutputCP(CP_UTF8); // affichage console wd
    srand((unsigned)time(NULL));  // rand

    // Créa plongeur
    Plongeur plongeur = {
        .points_de_vie = 100,
        .points_de_vie_max = 100,
        .niveau_oxygene = 100,
        .niveau_oxygene_max = 100,
        .niveau_fatigue = 0,
        .niveau_fatigue_max = 100,
        .perles = 10,
        .pos_y      = 0,
        .pos_x      = 0,
        .last_pos_y = 0,    // set a la pos de base
        .last_pos_x = 0    // set a la pos de base
    };
    // Créa ennemi
    CreatureMarine crabe = {
        .id = 1,
        .nom = "Poisson-Scie",
        .points_de_vie_max = 100,
        .points_de_vie_actuels = 55,
        .attaque_minimale = 12,
        .attaque_maximale = 20,
        .defense = 0,
        .vitesse = 0,
        .effet_special = "aucun",
        .est_vivant = 1
    };

    char **screen = init_screen();

// ======= Boucle de gameplay =======
int running = 1;
while (running) {

    // Affichage
    full_screen(&plongeur, &crabe, screen, info);

    // Récup input user validé selon l'écran courant
    char cmd = prompt_for_command(screen_status);
    if (cmd == '\0') break; // fin d'entrée

    switch (screen_status) {

        // ── Exploration ─────────────────────────────────────────────
        case 0: {
            if (cmd == 'C' || cmd == 'c') {
                screen_status = 2; // Carte
            }
            else if (cmd == 'I' || cmd == 'i') {
                screen_status = 3; // Inventaire
            }
            else if (cmd == 'D' || cmd == 'd') {
                // Demander coords => déplacer joueur (2 chars)
                demander_player_for_coords(screen, &plongeur);
                // Reste en exploration
                screen_status = 0;
            }
            break;
        }

        // ── Combat ─────────────────────────────────────────────────
        case 1: {
            if (cmd == 'A' || cmd == 'a') {
                // Attaque légère
                // A faire : implémentation
            }
            else if (cmd == 'B' || cmd == 'b') {
                // Attaque lourde
            }
            else if (cmd == 'C' || cmd == 'c') {
                // Attaque passive (?)
            }
            else if (cmd == 'I' || cmd == 'i') {
                // Ouvrir inventaire en combat
            }
            else if (cmd == 'F' || cmd == 'f') {
                // Tentative de fuite => grandes chances d'ecoucher
                // si echoue => passe tour
                // si reussie, sors du combat (perte d'O2 ?)
            }
            break;
        }

        // ── Carte ──────────────────────────────────────────────────
        case 2: {
            if (cmd == 'Q' || cmd == 'q') {
                screen_status = 0; // retour exploration
            }
            break;
        }

        // ── Inventaire ─────────────────────────────────────────────
        case 3: {
            if (cmd == 'Q' || cmd == 'q') {
                screen_status = 0; // retour exploration
            }
            break;
        }

        // ── Trésor / autres écrans ────────────────────────────────
        case 4: {
            if (cmd == 'Q' || cmd == 'q') {
                screen_status = 0;
            }
            break;
        }

        default:
            // écran inconnu -> retour sécu
            screen_status = 0;
            break;
    }

    printf("\n"); // petite séparation
}


    // faire une function pour free le tab
    return 0;
}

// ===================== Convert bar =====================
char* convert_to_visual_bar(int stat){
    int full_blocks = stat / 10; // full_block par dizaine de pv
    int semi_block = (stat % 10 != 0); // semi_block si reste entre 1 et 9
    int empty_blocks = 10 - full_blocks - semi_block; // nb de block restant => "vide"

    char *bar = malloc(10 * 3 + 1);
    if (!bar) return NULL;
    bar[0] = '\0';

    for (int i = 0; i < full_blocks; i++) strcat(bar, "█");
    if (semi_block) strcat(bar, "▓");
    for (int i = 0; i < empty_blocks; i++) strcat(bar, "░");

    return bar; // PAS OUBLIER DE free(bar) APRES USAGE
}

// ===================== Rendu global =====================
void full_screen(Plongeur *p, CreatureMarine *c, char** screen, char* info){
    char* pv_bar = convert_to_visual_bar(p->points_de_vie);
    char* oxy_bar = convert_to_visual_bar(p->niveau_oxygene);
    char* fatigue_bar = convert_to_visual_bar(p->niveau_fatigue);

    screen_header(p, pv_bar, oxy_bar, fatigue_bar, info);
    // 69, bords non compris
    screen_main(p, c, screen);
    screen_footer();

    free(pv_bar);
    free(oxy_bar);
    free(fatigue_bar);
}

void screen_header(Plongeur *p, char* pv_bar, char* oxy_bar, char* fatigue_bar, char* info){
    //max 64
    //char* info = "Information";
    printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n");
    printf("│  Vie: %s %3d%%  │", pv_bar, p->points_de_vie);
    printf("  O₂: %s %3d%%  │", oxy_bar, p->niveau_oxygene);
    printf("  Fatigue: %s %3d%%  │\n", fatigue_bar, p->niveau_fatigue);
    printf("│  Profondeur: 500M     │  Arme équipé : Harpon Rouillé                       │\n");
    printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
    //printf("│  [Info] : %64d  │\n", p->niveau_oxygene);
    printf("│  [Info] : %-64s  │\n", info);
    printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
}

void screen_main(Plongeur *p, CreatureMarine *c, char** screen){
    clear_screen_grid(screen, fill);

    switch (screen_status) {
        case 0: { // Exploration (couches : base -> flèches -> joueur)
            printf("│                                                                             │\n");
            printf("│    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!?@#$^&    │\n");
            printf("│   ╭────────────────────────── Exploration ──────────────────────────────╮   │\n");

            layer_explo_base(screen);
            layer_explo_arrows(screen);
            layer_player(p, screen);

            print_screen(screen);
            break;
        }
        case 1: { // Combat (couches : base -> joueur -> ennemi)
            printf("│                                                                             │\n");
            char * creature_pv_bar = convert_to_visual_bar(c->points_de_vie_actuels);
            printf("│   ╭───────────────────────────── Combat ────────────────────────────────╮   │\n");
            printf("│   │  [%d]: %s %3d%%                                               │   │\n",
                   c->id, creature_pv_bar, c->points_de_vie_actuels);
            printf("│   ├─────────────────────────────────────────────────────────────────────┤   │\n");
            free(creature_pv_bar);

            // couches combat
            // base vide (déjà clear)
            ajout_joueur_combat_screen(screen);
            ajout_creature_combat_screen(screen, c);

            print_screen(screen);
            break;
        }
        case 2: { // Carte
            printf("│                                                                             │\n");
            printf("│    [Carte] (à implémenter)                                                  │\n");
            printf("│   ╭────────────────────────────── Carte ────────────────────────────────╮   │\n");
            // remplissage de grille carte a faire ici
            print_screen(screen);
            break;
        }
        case 3: { // Inventaire
            printf("│                                                                             │\n");
            printf("│    [Inventaire] (à implémenter)                                             │\n");
            printf("│   ╭──────────────────────────── Inventaire ─────────────────────────────╮   │\n");
            // remplissage de grille inv a faire ici
            print_screen(screen);
            break;
        }
        default: {
            printf("│                                                                             │\n");
            printf("│    [Écran inconnu]                                                          │\n");
            print_screen(screen);
            break;
        }
    }

    printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
    printf("│                                                                             │\n");
}

void screen_footer(){
    // 0 = Exploration | 1 = combat | 2 = map | 3 = inventaire | 4 = tresor
    switch(screen_status){
        case 0:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [C] Carte  [I] Inventaire  [D] Se Deplacer                                 │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 1:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [A] Attaque légère  [B] Attaque Lourde  [C]  Méditation  [I] Inventaire    │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 2:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [Q]  Quitter                                                               │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 3:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [Q]  Quitter                                                               │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 4:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [Q]  Quitter                                                               │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        default: {
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [ ]                                                                         │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
    }
}

// ===================== Grille / dessin existant =====================
char** init_screen(){
    // Allocation dynamique du tab d'ecran — rempli de ' '
    char **screen = malloc(hauteur * sizeof(char*));
    for (int i = 0; i < hauteur; i++){
        screen[i] = malloc((largeur) * sizeof(char));
        for (int j = 0; j < largeur; j++) {
            screen[i][j] = fill;
        }
    }

    return screen;
}

// Affichage de truand V3
void print_screen(char **screen) {
    switch (screen_status){
        case 0: { // Exploration: avec labels ligne
            for (int i = 0; i < hauteur; i++) {
                char label;
                if (i < 10){
                    label = '0' + i;
                } else {
                    label = 'A' + (i - 10);
                }
                printf("│  %c│", label); // met les bords avant le screen[][] + Coord Ligne
                for (int j = 0; j < largeur; j++) {
                    printf("%c", screen[i][j]);
                }
                printf("│   │\n"); // met les bords apres le screen[][]
            }
            break;
        }
        default: { // autres écrans: bords classique
            for (int i = 0; i < hauteur; i++) {
                printf("│   │");
                for (int j = 0; j < largeur; j++) {
                    printf("%c", screen[i][j]);
                }
                printf("│   │\n");
            }
            break;
        }
    }
}

void ajout_joueur_combat_screen(char** screen){
    char remp = '@';
    for(int i = 1; i < 21; i++){ screen[18][i] = remp;}
    for(int i = 3; i < 20; i++){ screen[17][i] = remp;}
    for(int i = 5; i < 19; i++){ screen[16][i] = remp;}
    for(int i = 7; i < 17; i++){ screen[15][i] = remp;}
    for(int i = 9; i < 15; i++){ screen[14][i] = remp;}
    for(int i = 9; i < 16; i++){ screen[13][i] = remp;}
    for(int i = 8; i < 18; i++){ screen[12][i] = remp;}
    for(int i = 8; i < 19; i++){ screen[11][i] = remp;}
    for(int i = 7; i < 19; i++){ screen[10][i] = remp;}
    for(int i = 7; i < 18; i++){ screen[9][i]  = remp;}
    for(int i = 8; i < 17; i++){ screen[8][i]  = remp;}
    for(int i = 10; i < 16; i++){ screen[7][i] = remp;}
}

void ajout_creature_combat_screen(char** screen, CreatureMarine *c) {
    //Ecrire a l'écran le nom de la creature
    int taille_nom = (int)strlen(c->nom);
    int indice_depart = largeur - 8 - taille_nom; // 5 : 2 espace + [i] id creature
    if (indice_depart < 0) indice_depart = 0;

    screen[0][indice_depart] = '[';
    screen[0][indice_depart+1] = (char)('0' + c->id);
    screen[0][indice_depart+2] = ']';

    int pos = indice_depart + 3;
    for (int k = 0; k < taille_nom && (pos + k) < largeur; ++k) {
        screen[0][pos + k] = c->nom[k];
    }
}

// ===================== Déplacement joueur =====================
void demander_player_for_coords(char **screen, Plongeur *p) {
    char input[8];

    for (;;) {
        printf("Entrez des coordonnées [↓][→] : ");
        if (scanf(" %7s", input) != 1) {
            // Erreur => "sortie propre (j'ai vu ca sur internet")
            return;
        }

        size_t longueur = strlen(input);
        if (longueur != 2) {
            printf("Erreur : entrez exactement 2 caractères (ex: bB).\n");
            continue;
        }

        // Ordre: [↓][→]  => Y puis X
        char y_char = to_upper_ascii(input[0]); // Y tolère min/maj
        char x_char = input[1];                 // X est sensible à la casse

        int y = index_in_axis(y_char, Y_AXIS);
        int x = index_in_axis(x_char, X_AXIS);

        if (y < 0) { printf("Erreur : ligne invalide.\n"); continue; }
        if (x < 0) { printf("Erreur : colonne invalide.\n"); continue; }
        if (y >= hauteur || x >= largeur) {
            printf("Erreur : coordonnées hors limites (max Y=%d, X=%d).\n", hauteur-1, largeur-1);
            continue;
        }

        // Maj last pos a pos avant changement de pos
        p->last_pos_y = p->pos_y;
        p->last_pos_x = p->pos_x;
        // MAJ position joueur "@"
        p->pos_y = y;
        p->pos_x = x;

        // Si probleme d'affichage, reaficher les grilles d'en dessous
        // clear_screen_grid(screen, fill);
        // layer_explo_base(screen);
        // layer_explo_arrows(screen);

        // Traitement de l'info sur arrivé de la case
        action_apres_deplacement(p, /*c=*/NULL, y, x, screen);

        // Tout bon => sortie de boucle de saisie
        break;
    }
}

// ===================== Arrivé sur case =====================
int distance_entre_pos(Plongeur *p){

    int dist_y = abs(p->last_pos_y - p->pos_y);
    int dist_x = abs(p->last_pos_x - p->pos_x);
    // √(dx² + dy²)
    double dist = sqrt(dist_y*dist_y + dist_x*dist_x);
    // return l'entier le plus proche
    return (int)(dist + 0.5);
}

void action_apres_deplacement(Plongeur *p, CreatureMarine *c, int y, int x, char **screen){
    int cout_oxy = distance_entre_pos(p)/5;
    if (cout_oxy < 1){
        cout_oxy = 1;
    }
    int count_oxy_par_deplacement = cout_oxy;
    p->niveau_oxygene = clamp(p->niveau_oxygene - count_oxy_par_deplacement, p->niveau_oxygene_max);

    if (screen_status == 0) { // Exploration

        switch (screen[y][x]) {
        case '^':
            //printf("Sortie NORD (à implémenter: changement de grille ↑).\n");
            info = "Sortie NORD";
            // plus tard: changer d’index de carte, re-positionner player_y/player_x, etc.
            break;
        case 'v':
            info = "Sortie SUD";
            break;
        case '<':
            info = "Sortie OUEST";
            break;
        case '>':
            info = "Sortie EST";
            break;


        case 'T': // trésor
            printf("Delcancher fonction pour le tresord (affichage ecran tresor => generation du loot => affichage du loot)\n");
            break;

        case 'E': // ennemi
            printf("Declancher fonction combat\n");
            info = "Combat";
            screen_status = 1; // combat
            // init l'ennemie ici maybe
            break;

        default: // case vide
            info = " ";
            // Consommation d'O2 par déplacement
            // plus tard : conso d'O2 en fonction de la distance
            break;
        }
    }
    else {
        // rien pour les autres ecran
    }
}