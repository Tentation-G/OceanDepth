#include <stdio.h>
#include <string.h>
#include "input.h"
#include "../utils/utils.h"
#include "../world/world.h"
#include "../types/types.h"

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
    case 0:  return "CDIcdiSsE";          // Exploration  / [C] Carte | [I] Inv | [D] Depl | [E] Ecran combat pour test dev
    case 1:  return "ABCIabciQ";       // Combat       / [A] Atq A | [B] Atq B | [C] Atq C | [D] Atq D | [Q] Sortie ecran combat pour test dev
    case 2:  return "1234Qq";         // Carte        / [1] Carte I | [2] Carte II | [3] Carte III | [4] Carte IV |[Q] Quitter
    case 3:  return "Qq";            // Inventaire   / [Q] Quitter
    case 4:  return "Qq";           // Trésor       / [Q] Quitter
    default: return "";
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
