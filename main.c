#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h> // affichage console wd

#include <time.h> // rand

//█▓▒░
// ✹
//res screen
int largeur = 69; // screen screen main
int hauteur = 19;

char fill = ' ';
char fill_background[] = {'*','.','~',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' '};
//        int value = rand() % (upper_bound - lower_bound + 1)+ lower_bound;

// 0 = rien | 1 = combat
int screen_status = 1;

typedef struct {
    int points_de_vie;
    int points_de_vie_max; // 0 a 100
    int niveau_oxygene;
    int niveau_oxygene_max; // 0 a 100
    int niveau_fatigue;
    int niveau_fatigue_max; // 0 à 5 => ramené en % => 0 à 100
    int perles; // monnaie du jeu
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

char **init_screen();
void print_screen(char **screen);
void ajout_joueur_combat_screen(char **screen);

void ajout_creature_combat_screen(char** screen, CreatureMarine *c);


void full_screen(Plongeur *p, CreatureMarine *c);

void screen_header(Plongeur *p, char* pv_bar, char* oxy_bar, char* fatigue_bar);
void screen_main(CreatureMarine *c);
void screen_footer();

char* convert_to_visual_bar(int stat);


int main(void) {
    SetConsoleOutputCP(CP_UTF8); // affichage console wd

    // Créa plongeur
    Plongeur plongeur = {
        .points_de_vie = 55,
        .points_de_vie_max = 100,
        .niveau_oxygene = 78,
        .niveau_oxygene_max = 100,
        .niveau_fatigue = 12,
        .niveau_fatigue_max = 100,
        .perles = 10
    };
    // Créa Crabe
    CreatureMarine crabe = {
        .id = 1,
        .nom = "Crabe",
        .points_de_vie_max = 100,
        .points_de_vie_actuels = 100,
        .attaque_minimale = 12,
        .attaque_maximale = 20,
        .defense = 0,
        .vitesse = 0,
        .effet_special = "aucun",
        .est_vivant = 1
    };

    srand((unsigned)time(NULL)); // rand

    full_screen(&plongeur, &crabe);

    return 0;
}

char* convert_to_visual_bar(int stat){

    int full_blocks = stat / 10; // full_block par dizaine de pv
    int semi_block = (stat % 10 != 0); // semi_block si reste entre 1 et 9
    int empty_blocks = 10 - full_blocks - semi_block; // nb de block restant => "vide"

    // init a la taille des 10 block
    // 10 caract par indic bar * 3octet (chaque caract fait 3 octet) + 1 (le \0)
    char *bar = malloc(10 * 3 + 1);
    if (!bar) return NULL;
    bar[0] = '\0';

    // Remplissage de la bar
    for (int i = 0; i < full_blocks; i++){
         strcat(bar, "█");
    }
    if (semi_block){
        strcat(bar, "▓");
    }
    for (int i = 0; i < empty_blocks; i++){
        strcat(bar, "░");
    }

    return bar; // PAS OUBLIER DE free(bar) APRES USAGE
}

void full_screen(Plongeur *p, CreatureMarine *c){

    char* pv_bar = convert_to_visual_bar(p->points_de_vie);
    char* oxy_bar = convert_to_visual_bar(p->niveau_oxygene);
    char* fatigue_bar = convert_to_visual_bar(p->niveau_fatigue);

    screen_header(p, pv_bar, oxy_bar, fatigue_bar);
    // 69 bords non compris
    screen_main(c);
    screen_footer();

    // Libération mémoire
    free(pv_bar);
    free(oxy_bar);
    free(fatigue_bar);
}

void screen_header(Plongeur *p, char* pv_bar, char* oxy_bar, char* fatigue_bar){

    printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n");
    printf("│  Vie: %s %3d%%  │", pv_bar, p->points_de_vie);
    printf("  O₂: %s %3d%%  │", oxy_bar, p->niveau_oxygene);
    printf("  Fatigue: %s %3d%%  │\n", fatigue_bar, p->niveau_fatigue);
    //printf("│  Vie: ███████▒░░ 72%  |  O₂: ██████▒░░░ 64%  |  Fatigue: █▒░░░░░░░░ 15%     │\n");
    printf("│  Profondeur: 500M     │  Arme équipé : Harpon Rouillé                       │\n");
    printf("├─────────────────────────────────────────────────────────────────────────────┤\n");




}

void screen_main(CreatureMarine *c){
    char * creature_pv_bar = convert_to_visual_bar(c->points_de_vie_actuels);

    printf("│                                                                             │\n");

    switch(screen_status){
        case 0: {
            printf("│   ╭────────────────────────────── Base ─────────────────────────────────╮   │\n");
            char **screen = init_screen();
            print_screen(screen);

            break;
        }
        case 1: {
            printf("│   ╭───────────────────────────── Combat ────────────────────────────────╮   │\n");
            printf("│   │  [%d]: %s %3d%%                                               │   │\n"
            ,c->id, creature_pv_bar, c->points_de_vie_actuels);
            printf("│   ├─────────────────────────────────────────────────────────────────────┤   │\n");
            //affichage ecran "vide"
            char **screen = init_screen();
            ajout_joueur_combat_screen(screen);
            ajout_creature_combat_screen(screen, c);
            print_screen(screen);
            break;
        }
    }

    printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
    printf("│                                                                             │\n");

}
void screen_footer(){

    switch(screen_status){
        case 0:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [M] Carte  [I] Inventaire  [R] Remonter  [D] Descendre  [A] Attaquer       │\n"); //dyna
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 1:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [A] Attaque légère  [B] Attaque Lourde  [I] Inventaire  [F] Fuire          │\n"); //dyna
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
    }


}

char** init_screen(){

    //size_t n = sizeof(fill_background) / sizeof(fill_background[0]); // rand

    // Allocation dynamique du tab d'ecran
    //Remplissage all
    char **screen = malloc(hauteur * sizeof(char*));
    for (int i = 0; i < hauteur; i++){
        screen[i] = malloc((largeur) * sizeof(char));

        for (int j = 0; j < largeur; j++) {
            screen[i][j] = fill; //fill_background[rand() % n]; // rand
        }
    }

    return screen;
}

// Affichage de truand ep2
void print_screen(char **screen) {

    for (int i = 0; i < hauteur; i++) {
        printf("│   │"); // met les bord avant le screen[][]
        for (int j = 0; j < largeur; j++) {
            printf("%c", screen[i][j]);
        }
        printf("│   │"); // met les bord apres le screen[][]
        printf("\n");
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
    if(strcmp(c->nom, "Crabe") == 0){
        //Ecrire a l'écran le nom de la creature
        int taille_nom = strlen(c->nom);
        int indice_depart = largeur - 8 - taille_nom; // 5 : 2 espace + [i] id creature

        screen[0][indice_depart] = '[';
        screen[0][indice_depart+1] = '0' + c->id;
        screen[0][indice_depart+2] = ']';

        //for(int j = indice_depart + 3; j < largeur; j++){
        //    screen[0][j] = c->nom[j - 59 - 3];
        //}

        int pos = indice_depart + 3;
        for (int k = 0; k < taille_nom && (pos + k) < largeur; ++k) {
            screen[0][pos + k] = c->nom[k];
        }
        //printf("%d",indice_depart);

    }
}