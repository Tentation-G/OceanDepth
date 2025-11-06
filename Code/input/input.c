#include <stdio.h>
#include <string.h>
#include "input.h"
#include "../utils/utils.h"
#include "../world/world.h"
#include "../types/types.h"
#include "../combat/combat.h"

// bool
int zone_is_grotte(ZoneType type) {
    return (type == ZoneType_GROTTE       ||
            type == ZoneType_GROTTE_NORD  ||
            type == ZoneType_GROTTE_SUD   ||
            type == ZoneType_GROTTE_OUEST ||
            type == ZoneType_GROTTE_EST);
}

// Retourne la liste des touches valides selon l'écran courant
char* saisies_utilisateur_autorise(int status) {
    switch (status) {
    case 0:  return "CDIcdiSsE";               // Exploration | [C] Carte  | [I] Inv      | [D] Depl        | [E] Ecran combat pour test dev
    //-------------------------------------------------------------------------------------------------------------------------------------------------------  
    case 1:  return "ABCDIabcdiQq";          // Combat        | [A] Atq A  | [B] Atq B    | [C] Atq Passive | [D] Competences | [I] Inventaire | [Q] Quitter
    //-------------------------------------------------------------------------------------------------------------------------------------------------------     
    case 20:  return "1234Qq";             // Carte Ecran    | [1] Carte I | [2] Carte II | [3] Carte III   | [4] Carte IV    |[Q] Quitter
    case 21:  return "RrQq";              // Carte 1         | [R] Retour  | [Q] Quitter
    case 22:  return "RrQq";             // Carte 2          | [R] Retour  | [Q] Quitter
    case 23:  return "RrQq";            // Carte 3           | [R] Retour  | [Q] Quitter
    case 24:  return "RrQq";           // Carte 4            | [R] Retour  | [Q] Quitter
    //-------------------------------------------------------------------------------------------------------------------------------------------------------     
    case 3:  return "Qq12";          // Inventaire           | [Q] Quitter | [1] Utiliser Objet | [2] Equiper Objet
    case 4:  return "Qq";           // Trésor                | [Q] Quitter
    //-------------------------------------------------------------------------------------------------------------------------------------------------------     
    default: return "";
     // ton case 11 (anciennement case 5) tu me le cale ici (et tu changes case 1 par case 10)
    // Tu ecrases pas cette fonction, laisses la en paix la pauvre
    }
}

// Verif si un char c est present dans la liste *liste => 0/1
int char_in(char *liste, char c) {
    for (int i = 0; liste[i] != '\0'; i++)
        if (liste[i] == c) return 1;
    return 0;
}

// Verif si le joueur peut utiliser une lettre
char prompt_for_command(World *w, Plongeur *p, int status) {
    char *allowed = saisies_utilisateur_autorise(status);
    char input[16];

    for (;;) {
        printf("Que souhaitez vous faire : ");
        if (scanf("%15s", input) != 1) {
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
        if ((c == 'S' || c == 's') && w && p) {
            ZoneType t = world_get_zone_type(w, p->map_pos_y, p->map_pos_x);
            if (!zone_is_grotte(t)) {
                printf("La sauvegarde est impossible ici.\n");
                continue;
            }
        }
        return c;
    }
}

// Question oui non (pour l'instant juste utilisé pour la confirmation de sortie de zone)
int ask_yes_no(char *question) {
    char input[16];
    for (;;) {
        printf("%s [O/N] : ", question);
        if (scanf(" %15s", input) != 1) return 0;
        char c = to_upper_ascii(input[0]);
        if (c == 'O') return 1;
        if (c == 'N') return 0;
        printf("Répondez par O ou N.\n");
    }
}

// Nouvelle fonction pour choisir une cible
int prompt_for_target(int nbr_mobs, CreatureMarine *creatures){
    int choix_mob;
    printf("\nChoisissez la creature a attaquer: \n");
    creatures_restants(creatures, nbr_mobs);
    printf("> ");

    while(scanf("%d", &choix_mob) != 1 || choix_mob < 1 || choix_mob < 1 || choix_mob > nbr_mobs || !creatures[choix_mob - 1].est_vivant){
        printf("Choix invalide, entrez a nouveau : ");
        // Vide le buffer d'entrée en cas d'erreur
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return choix_mob - 1; // index de mob de (0 => nbr_mobs -1)
}

// A modifier (pas crutial, a faire quand on aura le temps)
// pour inventaire
int prompt_for_inventory_slot(const char* action_prompt) {
    int slot;
    printf("%s (1-8) : ", action_prompt);
    
    while (scanf("%d", &slot) != 1 || slot < 1 || slot > 8) {
        printf("Choix invalide. Entrez un numero de 1 a 8 : ");
        // Vide le buffer d'entrée
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return slot - 1; // Retourne l'index (0-7)
}
