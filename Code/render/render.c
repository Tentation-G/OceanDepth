#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "../globals/globals.h"
#include "../utils/utils.h"
#include "../world/world.h"
#include "../world/map_lt.h"
#include "../player/player.h"
#include "../combat/combat.h"

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
                printf("│  %c│", label);
                for (int j = 0; j < largeur; j++) {
                    printf("%c", screen[i][j]);
                }
                printf("│   │\n");
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

void screen_header(World *w, Plongeur *p, char* pv_bar, char* oxy_bar, char* fatigue_bar, char* info){
    ZoneType type = world_get_zone_type(w, p->map_pos_y, p->map_pos_x);

    char* arme_equipe = "Harpon Rouille";

    int profondeur;
    switch(p->map_pos_y) {
        case 0 : profondeur = 0; break;
        case 1 : profondeur = -50; break;
        case 2 : profondeur = -150; break;
        case 3 : profondeur = -300; break;
        case 4 : profondeur = -500; break;
        default : profondeur = 0; break;
    }
    printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n");
    printf("│  Vie: %s %3d%%  │", pv_bar, p->points_de_vie);
    printf("  O₂: %s %3d%%  │", oxy_bar, p->niveau_oxygene);
    printf("  Fatigue: %s %3d%%  │\n", fatigue_bar, p->niveau_fatigue);
    printf("│  Profondeur: %4dm     ", profondeur);
    printf("│  Arme équipé : %-36s│\n", arme_equipe);
    printf("├──────────────────────────────────────────────────────────────┬──────────────┤\n");
    printf("│  [Info] : %-49s  │  Zone[%d][%d]  │\n", info, p->map_pos_y, p->map_pos_x);
    printf("├──────────────────────────────────────────────────────────────┤ type: %-6s │\n", zone_type_to_string(type));
}

void screen_main(World *w, Plongeur *p, CreatureMarine *c, char** screen){
    switch (screen_status) {
        case 0: { // Exploration
            printf("│                                                              ╰──────────────┤\n");
            printf("│    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!?@#$^&    │\n");
            printf("│   ╭──────────────────────────── Exploration ────────────────────────────╮   │\n");

            // base + joueur
            // layer_explo_base : no-op
            layer_player(p, screen);
            print_screen(screen);
            break;
        }
        case 1: { // Combat
            printf("│                                                              ╰──────────────┤\n");
            char * creature_pv_bar = convert_to_visual_bar(c->points_de_vie_actuels, c->points_de_vie_max);
            printf("│   ╭───────────────────────────── Combat ────────────────────────────────╮   │\n");
            //printf("│   │  [%d]: %s %3d%%                                               │   │\n",
            //       c->id, creature_pv_bar, convert_to_percent(c->points_de_vie_actuels, c->points_de_vie_max));
            printf("│   ├─────────────────────────────────────────────────────────────────────┤   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │          @@@@@@                                                     │   │\n");
            printf("│   │        @@@@@@@@@                                                    │   │\n");
            printf("│   │       @@@@@@@@@@@                                                   │   │\n");
            printf("│   │       @@@@@@@@@@@@                                                  │   │\n");
            printf("│   │        @@@@@@@@@@@                                                  │   │\n");
            printf("│   │        @@@@@@@@@@                                                   │   │\n");
            printf("│   │         @@@@@@@                                                     │   │\n");
            printf("│   │         @@@@@@                                                      │   │\n");
            printf("│   │       @@@@@@@@@@                                                    │   │\n");
            printf("│   │     @@@@@@@@@@@@@@                                                  │   │\n");
            printf("│   │   @@@@@@@@@@@@@@@@@                                                 │   │\n");
            printf("│   │ @@@@@@@@@@@@@@@@@@@@                                                │   │\n");
            printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            free(creature_pv_bar);

            //char** screen_combat = init_screen();
            //ajout_joueur_combat_screen(screen_combat);
            //ajout_creature_combat_screen(screen_combat, c);
            //print_screen(screen_combat);
            break;
        }
        case 2: { // Carte
            char* c00 = (zone_already_visited(w, 0, 0)) ? " Base " : "  ??  ";
            char* c01 = (zone_already_visited(w, 0, 1)) ? "      " : "  ??  ";
            char* c02 = (zone_already_visited(w, 0, 2)) ? "      " : "  ??  ";
            char* c03 = (zone_already_visited(w, 0, 3)) ? "Bateau" : "  ??  ";
            char* c04 = (zone_already_visited(w, 0, 4)) ? "      " : "  ??  ";

            char* c10 = (zone_already_visited(w, 1, 0)) ? "Grotte" : "  ??  ";
            char* c11 = (zone_already_visited(w, 1, 1)) ? "      " : "  ??  ";
            char* c12 = (zone_already_visited(w, 1, 2)) ? "      " : "  ??  ";
            char* c13 = (zone_already_visited(w, 1, 3)) ? "      " : "  ??  ";
            char* c14 = (zone_already_visited(w, 1, 4)) ? "      " : "  ??  ";

            char* c20 = (zone_already_visited(w, 2, 0)) ? "      " : "  ??  ";
            char* c21 = (zone_already_visited(w, 2, 1)) ? " Boss " : "  ??  ";
            char* c22 = (zone_already_visited(w, 2, 2)) ? "      " : "  ??  ";
            char* c23 = (zone_already_visited(w, 2, 3)) ? "      " : "  ??  ";
            char* c24 = (zone_already_visited(w, 2, 4)) ? "Grotte" : "  ??  ";

            char* c30 = (zone_already_visited(w, 3, 0)) ? "      " : "  ??  ";
            char* c31 = (zone_already_visited(w, 3, 1)) ? "      " : "  ??  ";
            char* c32 = (zone_already_visited(w, 3, 2)) ? "      " : "  ??  ";
            char* c33 = (zone_already_visited(w, 3, 3)) ? "      " : "  ??  ";
            char* c34 = (zone_already_visited(w, 3, 4)) ? "      " : "  ??  ";

            char* c40 = (zone_already_visited(w, 4, 0)) ? " Boss " : "  ??  ";
            char* c41 = (zone_already_visited(w, 4, 1)) ? "      " : "  ??  ";
            char* c42 = (zone_already_visited(w, 4, 2)) ? "      " : "  ??  ";
            char* c43 = (zone_already_visited(w, 4, 3)) ? "      " : "  ??  ";
            char* c44 = (zone_already_visited(w, 4, 4)) ? "Grotte" : "  ??  ";

            printf("│                                                                             │\n");
            printf("│   ╭───────────────────────────── Carte ─────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │           ╔═══════════╗       ╔══════════════╗                      │   │\n");
            printf("│   │      ╔════╝           ╚═══════╝              ╚═══╗  ╔═══════╗       │   │\n");
            printf("│   │      ║                                           ╚══╝       ╚╗      │   │\n");
            printf("│   │      ║ %6s│%6s│%6s│%6s│%6s   Surface  [0m]    ║      │   │\n",c00, c01, c02, c03, c04);
            printf("│   │      ║ ──────┼──────┼──────┼──────┼──────                    ║      │   │\n");
            printf("│   │      ║ %6s│%6s│%6s│%6s│%6s   Zone I   [-50m]  ║      │   │\n",c10, c11, c12, c13, c14);
            printf("│   │      ║ ──────┼──────┼──────┼──────┼──────                    ║      │   │\n");
            printf("│   │    @@║ %6s│%6s│%6s│%6s│%6s   Zone II  [-150m] ║@@    │   │\n",c20, c21, c22, c23, c24);
            printf("│   │    @@║ ──────┼──────┼──────┼──────┼──────                    ║@@    │   │\n");
            printf("│   │  @@@@║ %6s│%6s│%6s│%6s│%6s   Zone III [-200m] ║@@@@  │   │\n",c30, c31, c32, c33, c34);
            printf("│   │  @@@ ║ ──────┼──────┼──────┼──────┼──────                    ║ @@@  │   │\n");
            printf("│   │  @@@@║ %6s│%6s│%6s│%6s│%6s   Zone IV  [-???m] ║@@@@  │   │\n",c40, c41, c42, c43, c44);
            printf("│   │  @@@@╚════╗                             ╔═════════╗        ╔═╝@@@@  │   │\n");
            printf("│   │ @@@@@@@   ╚═════════════════════════════╝         ╚════════╝ @@@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");
            break;
        }
        case 3: { // Inventaire
            printf("│                                                                             │\n");
            printf("│    [Inventaire] (à implémenter)                                             │\n");
            printf("│   ╭──────────────────────────── Inventaire ─────────────────────────────╮   │\n");
            print_screen(screen);
            break;
        }
        default: {
            printf("│                                                                             │\n");
            printf("│    [Écran 404]                                                              │\n");
            print_screen(screen);
            break;
        }
    }

    printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
    printf("│                                                                             │\n");
}

void screen_footer(World *w, Plongeur *p){
    ZoneType type = world_get_zone_type(w, p->map_pos_y, p->map_pos_x);

    char* sauvegarde;
    if(strcmp(zone_type_to_string(type), "Grotte") == 0){
        sauvegarde = "[S] Sauvegarder";
    } else{
        sauvegarde = "";
    }
    switch(screen_status){
        case 0:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [C] Carte  [I] Inventaire  [D] Se Deplacer  %15s                │\n", sauvegarde);
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
            printf("│  [1] Carte I  [2] Carte II  [3] Carte III  [4] Carte IV  [Q]  Quitter       │\n");
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
            printf("│  [ ]                                                                        │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
    }
}

void full_screen(World *w, Plongeur *p, CreatureMarine *c, char** screen, char* info){
    char* pv_bar = convert_to_visual_bar(p->points_de_vie, p->points_de_vie_max);
    char* oxy_bar = convert_to_visual_bar(p->niveau_oxygene, p->niveau_oxygene_max);
    char* fatigue_bar = convert_to_visual_bar(p->niveau_fatigue, p->niveau_fatigue_max);

    screen_header(w, p, pv_bar, oxy_bar, fatigue_bar, info);
    screen_main(w, p, c, screen);
    screen_footer(w, p);

    free(pv_bar);
    free(oxy_bar);
    free(fatigue_bar);
}
