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
    printf("├─────────────────────────────────────────────────────────────┬───────────────┤\n");
    printf("│  [Info] : %-48s  │  Zone-[%d][%d]  │\n", info, p->map_pos_y, p->map_pos_x);
    printf("├─────────────────────────────────────────────────────────────┤ %-13s │\n", zone_type_to_string(type));
}

void screen_main(World *w, Plongeur *p, CreatureMarine *c, char** screen){
    switch (screen_status) {
        case 0: { // Exploration
            printf("│                                                             ╰───────────────┤\n");
            printf("│    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!?@#$^&    │\n");
            printf("│   ╭──────────────────────────── Exploration ────────────────────────────╮   │\n");

            // base + joueur
            // layer_explo_base : no-op
            layer_player(p, screen);
            print_screen(screen);
            break;
        }
        case 1: { // Combat
            printf("│                                                             ╰───────────────┤\n");
            //char * creature_pv_bar = convert_to_visual_bar(c->points_de_vie_actuels, c->points_de_vie_max);
            printf("│   ╭───────────────────────────── Combat ────────────────────────────────╮   │\n");
            //printf("│   │  [%d]: %s %3d%%                                               │   │\n",
            //       c->id, creature_pv_bar, convert_to_percent(c->points_de_vie_actuels, c->points_de_vie_max));
            //printf("│   ├─────────────────────────────────────────────────────────────────────┤   │\n");
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
            //printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            //free(creature_pv_bar);

            //char** screen_combat = init_screen();
            //ajout_joueur_combat_screen(screen_combat);
            //ajout_creature_combat_screen(screen_combat, c);
            //print_screen(screen_combat);
            break;
        }
        case 20:{

            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                            ╔══╗                                     │   │\n");
            printf("│   │            ╔═╗             ║  ║                     ╔══╗            │   │\n");
            printf("│   │           ╔╝ ║            ╔╝I ║          ╔═╗        ║  ╚╗           │   │\n");
            printf("│   │           ║ I║            ║ I╔╝          ║I╚╗       ║ I ║           │   │\n");
            printf("│   │           ║  ╚╗           ║  ║           ║I ║       ║ V ║           │   │\n");
            printf("│   │           ║   ║           ║ C║          ╔╝I ║       ║   ║           │   │\n");
            printf("│   │           ║ C ║           ║ A║          ║   ║       ║ C ║           │   │\n");
            printf("│   │           ║ A@@@          ║ R╚╗         ║ C ║      @@@A ║           │   │\n");
            printf("│   │          @@ R @@@         ║ T ║         ║ A ║     @@@ R @@          │   │\n");
            printf("│   │         @@║ T@║@@@        ║ E ║         ║ R ║    @@@║ T ║@@         │   │\n");
            printf("│   │         @@║ E @@@@@       ║   ║         ║ T ║   @@@@@ E ║@@         │   │\n");
            printf("│   │         @@║   ║@@@        ║   ║         ║ E ║    @@@║   ║@@         │   │\n");
            printf("│   │        @@@║   ║@          ║  ╔╝         ║  ╔╝      @╚╗  ║@@@        │   │\n");
            printf("│   │        @@@║  ╔╝           ╚══╝          ║  ║         ╚╗ ║@@@        │   │\n");
            printf("│   │       @@@@╚══╝                          ║ ╔╝          ╚═╝@@@@       │   │\n");
            printf("│   │      @@@@@@@@@@                         ╚═╝           @@@@@@@@      │   │\n");
            printf("│   │      @@@@@@                                             @@@@@@      │   │\n");
            //printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            //printf("│                                                                             │\n");
            break;
        }
        case 21: { // Carte 1
             // C'est un maxi paté, je sais, simple et efficace
            // (le mapping est geré ailleurs(map_lt.c), pas grand besoin de faire quelque chose fancy

            // Ligne 0
            const char* c00 = zone_already_visited(w, 0, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 0)) : " ?? ";
            const char* c01 = zone_already_visited(w, 0, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 1)) : " ?? ";
            const char* c02 = zone_already_visited(w, 0, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 2)) : " ?? ";
            const char* c03 = zone_already_visited(w, 0, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 3)) : " ?? ";
            const char* c04 = zone_already_visited(w, 0, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 4)) : " ?? ";
            const char* c05 = zone_already_visited(w, 0, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 5)) : " ?? ";
            const char* c06 = zone_already_visited(w, 0, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 6)) : " ?? ";
            const char* c07 = zone_already_visited(w, 0, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 7)) : " ?? ";
            const char* c08 = zone_already_visited(w, 0, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 8)) : " ?? ";
            const char* c09 = zone_already_visited(w, 0, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 0, 9)) : " ?? ";

            // Ligne 1
            const char* c10 = zone_already_visited(w, 1, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 0)) : " ?? ";
            const char* c11 = zone_already_visited(w, 1, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 1)) : " ?? ";
            const char* c12 = zone_already_visited(w, 1, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 2)) : " ?? ";
            const char* c13 = zone_already_visited(w, 1, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 3)) : " ?? ";
            const char* c14 = zone_already_visited(w, 1, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 4)) : " ?? ";
            const char* c15 = zone_already_visited(w, 1, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 5)) : " ?? ";
            const char* c16 = zone_already_visited(w, 1, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 6)) : " ?? ";
            const char* c17 = zone_already_visited(w, 1, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 7)) : " ?? ";
            const char* c18 = zone_already_visited(w, 1, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 8)) : " ?? ";
            const char* c19 = zone_already_visited(w, 1, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 1, 9)) : " ?? ";

            // Ligne 2
            const char* c20 = zone_already_visited(w, 2, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 0)) : " ?? ";
            const char* c21 = zone_already_visited(w, 2, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 1)) : " ?? ";
            const char* c22 = zone_already_visited(w, 2, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 2)) : " ?? ";
            const char* c23 = zone_already_visited(w, 2, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 3)) : " ?? ";
            const char* c24 = zone_already_visited(w, 2, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 4)) : " ?? ";
            const char* c25 = zone_already_visited(w, 2, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 5)) : " ?? ";
            const char* c26 = zone_already_visited(w, 2, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 6)) : " ?? ";
            const char* c27 = zone_already_visited(w, 2, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 7)) : " ?? ";
            const char* c28 = zone_already_visited(w, 2, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 8)) : " ?? ";
            const char* c29 = zone_already_visited(w, 2, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 2, 9)) : " ?? ";

            printf("│                                                              ╰──────────────┤\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │      ╔═══════╗ ╔══════════╗            ╔═══════════╗  ╔═════╗       │   │\n");
            printf("│   │      ║  Carte╚╗║I]        ╚════════════╝           ║ ╔╝     ╚╗      │   │\n");
            printf("│   │      ╚╗       ╚╝                                   ╚╗║       ║      │   │\n");
            printf("│   │       ╚╗                                         Pro╚╝ndeur  ║      │   │\n");
            printf("│   │       ╔╝                                                     ║      │   │\n");
            printf("│   │      ╔╝                                                      ║      │   │\n");
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  S  ║      │   │\n", c00, c01, c02, c03, c04, c05, c06, c07, c08, c09);
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  I  ║@@    │   │\n", c10, c11, c12, c13, c14, c15, c16, c17, c18, c19);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  I  ║ @@@  │   │\n", c20, c21, c22, c23, c24, c25, c26, c27, c28, c29);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@╚═╗        │ ╔═════╗ │                ╔══════╗│      II╔╝@@@@  │   │\n");
            printf("│   │  @@@@  ╚══════════╝     ╚══════════════════╝      ╚═════════╝ @@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");

            break;
        }
        case 22: { // Carte 1
             // C'est un maxi paté, je sais, simple et efficace
            // (le mapping est geré ailleurs(map_lt.c), pas grand besoin de faire quelque chose fancy

            // Ligne 0
            const char* c30 = zone_already_visited(w, 3, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 0)) : " ?? ";
            const char* c31 = zone_already_visited(w, 3, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 1)) : " ?? ";
            const char* c32 = zone_already_visited(w, 3, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 2)) : " ?? ";
            const char* c33 = zone_already_visited(w, 3, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 3)) : " ?? ";
            const char* c34 = zone_already_visited(w, 3, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 4)) : " ?? ";
            const char* c35 = zone_already_visited(w, 3, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 5)) : " ?? ";
            const char* c36 = zone_already_visited(w, 3, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 6)) : " ?? ";
            const char* c37 = zone_already_visited(w, 3, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 7)) : " ?? ";
            const char* c38 = zone_already_visited(w, 3, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 8)) : " ?? ";
            const char* c39 = zone_already_visited(w, 3, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 3, 9)) : " ?? ";

            // Ligne 1
            const char* c40 = zone_already_visited(w, 4, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 0)) : " ?? ";
            const char* c41 = zone_already_visited(w, 4, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 1)) : " ?? ";
            const char* c42 = zone_already_visited(w, 4, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 2)) : " ?? ";
            const char* c43 = zone_already_visited(w, 4, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 3)) : " ?? ";
            const char* c44 = zone_already_visited(w, 4, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 4)) : " ?? ";
            const char* c45 = zone_already_visited(w, 4, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 5)) : " ?? ";
            const char* c46 = zone_already_visited(w, 4, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 6)) : " ?? ";
            const char* c47 = zone_already_visited(w, 4, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 7)) : " ?? ";
            const char* c48 = zone_already_visited(w, 4, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 8)) : " ?? ";
            const char* c49 = zone_already_visited(w, 4, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 4, 9)) : " ?? ";

            // Ligne 2
            const char* c50 = zone_already_visited(w, 5, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 0)) : " ?? ";
            const char* c51 = zone_already_visited(w, 5, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 1)) : " ?? ";
            const char* c52 = zone_already_visited(w, 5, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 2)) : " ?? ";
            const char* c53 = zone_already_visited(w, 5, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 3)) : " ?? ";
            const char* c54 = zone_already_visited(w, 5, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 4)) : " ?? ";
            const char* c55 = zone_already_visited(w, 5, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 5)) : " ?? ";
            const char* c56 = zone_already_visited(w, 5, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 6)) : " ?? ";
            const char* c57 = zone_already_visited(w, 5, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 7)) : " ?? ";
            const char* c58 = zone_already_visited(w, 5, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 8)) : " ?? ";
            const char* c59 = zone_already_visited(w, 5, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 5, 9)) : " ?? ";

            printf("│                                                              ╰──────────────┤\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │      ╔═══════╗ ╔══════════╗            ╔═══════════╗  ╔═════╗       │   │\n");
            printf("│   │      ║  Carte╚╗║II]       ╚════════════╝           ║ ╔╝     ╚╗      │   │\n");
            printf("│   │      ╚╗       ╚╝                                   ╚╗║       ║      │   │\n");
            printf("│   │       ╚╗                                         Pro╚╝ndeur  ║      │   │\n");
            printf("│   │       ╔╝                                                     ║      │   │\n");
            printf("│   │      ╔╝                                                      ║      │   │\n");
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  II ║      │   │\n", c30, c31, c32, c33, c34, c35, c36, c37, c38, c39);
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  II ║@@    │   │\n", c40, c41, c42, c43, c44, c45, c46, c47, c48, c49);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s III ║ @@@  │   │\n", c50, c51, c52, c53, c54, c55, c56, c57, c58, c59);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@╚═╗        │ ╔═════╗ │                ╔══════╗│     III╔╝@@@@  │   │\n");
            printf("│   │  @@@@  ╚══════════╝     ╚══════════════════╝      ╚═════════╝ @@@@@ │   │\n");
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
        // Explo
        case 0:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [C] Carte  [I] Inventaire  [D] Se Deplacer  %15s                │\n", sauvegarde);
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        //Combat
        case 1:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [A] Attaque légère  [B] Attaque Lourde  [C]  Méditation  [I] Inventaire    │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        //Carte
        case 20:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [1] Carte I  [2] Carte II  [3] Carte III  [4] Carte IV  [Q]  Quitter       │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 21:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [R]  Retour  [Q]  Quitter                                                  │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 22:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [R]  Retour  [Q]  Quitter                                                  │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 23:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [R]  Retour  [Q]  Quitter                                                  │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 24:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [R]  Retour  [Q]  Quitter                                                  │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        // Inventaire
        case 3:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [Q]  Quitter                                                               │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        // Coffre
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
