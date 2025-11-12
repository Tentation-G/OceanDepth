#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "../globals/globals.h"
#include "../save/save.h"
#include "../utils/utils.h"
#include "../world/world.h"
#include "../world/map_lt.h"
#include "../player/player.h"
#include "../combat/combat.h"
#include "../inventaire/inventaire.h"
#include "../marchand/marchand.h"

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

    switch (screen_status){
        case 50:
        case 51:
        case 52: {
            printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n");
            printf("│                                                                             │\n");
            break;
        }
        default:{
            ZoneType type = world_get_zone_type(w, p->map_pos_y, p->map_pos_x);
            char* arme_equipe = "Harpon Rouille";

            int profondeur;
            // Ce truc la n'a plus de sens, a refaire
            switch(p->map_pos_y) {
            case 0 : profondeur = 0; break;
            case 1 : profondeur = -50; break;
            case 2 : profondeur = -150; break;
            case 3 : profondeur = -300; break;
            case 4 : profondeur = -500; break;
            default : profondeur = 0; break;
            }
            ItemTemplate *weapon = get_item_template(p->equip_weapon.item_id);
            printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n");
            printf("│  Vie: %s %3d%%  │", pv_bar, p->points_de_vie);
            printf("  O₂: %s %3d%%  │", oxy_bar, p->niveau_oxygene);
            printf("  Fatigue: %s %3d%%  │\n", fatigue_bar, p->niveau_fatigue);
            printf("│  Profondeur: %4dm     ", profondeur);
            printf("│  Arme équipé : %-24s | Save : %d │\n", weapon->nom, active_save);
            printf("├─────────────────────────────────────────────────────────────┬───────────────┤\n");
            printf("│  [Info] : %-48s  │  Zone-[%d][%d]  │\n", info, p->map_pos_y, p->map_pos_x);
            printf("├─────────────────────────────────────────────────────────────┤ %-13s │\n", zone_type_to_string(type));
            break;
        }
    }
}

void screen_main(World *w, Plongeur *p, CreatureMarine *creatures, char** screen){
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

        case 10: { // Combat
            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────────────── Combat ──────────────────────────────╮   │\n");
            if (type_combat == 1){
                CreatureMarine *boss = &creatures[0];
                char *boss_pv_bar = convert_to_visual_bar(boss->points_de_vie_actuels, boss->points_de_vie_max);
                int boss_pv_percent = convert_to_percent(boss->points_de_vie_actuels, boss->points_de_vie_max);

                printf("│   │                 %10s  %3d%%                                    │   │\n",
                    boss_pv_bar, boss_pv_percent);
                printf("│   │                 [%d]%12s                                     │   │\n",
                    boss->id, boss->nom);
                printf("│   │                                        ###########                  │   │\n");
                printf("│   │                                   #####################             │   │\n");
                printf("│   │                               #############################         │   │\n");
                printf("│   │                            #################################        │   │\n");
                printf("│   │                           ###################################       │   │\n");
                printf("│   │          @@@@@@           ####################################      │   │\n");
                printf("│   │        @@@@@@@@@          ####################################      │   │\n");
                printf("│   │       @@@@@@@@@@@         ###################################       │   │\n");
                printf("│   │       @@@@@@@@@@@@        #################################         │   │\n");
                printf("│   │        @@@@@@@@@@@          ##############################          │   │\n");
                printf("│   │        @@@@@@@@@@             ##########################            │   │\n");
                printf("│   │         @@@@@@@                  ##################                 │   │\n");
                printf("│   │         @@@@@@                                                      │   │\n");
                printf("│   │       @@@@@@@@@@                                                    │   │\n");
                printf("│   │     @@@@@@@@@@@@@@                                                  │   │\n");
                printf("│   │   @@@@@@@@@@@@@@@@@                                                 │   │\n");
                printf("│   │ @@@@@@@@@@@@@@@@@@@@                                                │   │\n");
                
                free(boss_pv_bar);
            }       
            else if (type_combat ==0 && g_nbr_creatures_en_combat == 1) {
                CreatureMarine *c1 = &creatures[0];
                char *creature1_pv_bar = convert_to_visual_bar(c1->points_de_vie_actuels, c1->points_de_vie_max);
                
                printf("│   │                         %10s  %3d%%                            │   │\n",
                    creature1_pv_bar, 
                    convert_to_percent(c1->points_de_vie_actuels, c1->points_de_vie_max));
                printf("│   │                         [%d]%12s                             │   │\n",
                    c1->id, c1->nom);
                printf("│   │                                                                     │   │\n");
                printf("│   │                            ####                                     │   │\n");
                printf("│   │                          #########                                  │   │\n");
                printf("│   │                            ####                                     │   │\n");
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
                
                free(creature1_pv_bar);
            }
            else if (type_combat ==0 && g_nbr_creatures_en_combat == 2) {
                CreatureMarine *c1 = &creatures[0];
                CreatureMarine *c2 = &creatures[1];
                char *creature1_pv_bar = convert_to_visual_bar(c1->points_de_vie_actuels, c1->points_de_vie_max);
                char *creature2_pv_bar = convert_to_visual_bar(c2->points_de_vie_actuels, c2->points_de_vie_max);
                
                printf("│   │                         %10s  %3d%%         %10s  %3d%%   │   │\n",
                    creature1_pv_bar, 
                    convert_to_percent(c1->points_de_vie_actuels, c1->points_de_vie_max),
                    creature2_pv_bar, 
                    convert_to_percent(c2->points_de_vie_actuels, c2->points_de_vie_max));
                printf("│   │                         [%d]%12s          [%d]%12s    │   │\n",
                    c1->id, c1->nom, c2->id, c2->nom);
                printf("│   │                                                                     │   │\n");
                printf("│   │                            ####                   ##########        │   │\n");
                printf("│   │                          #########              ##############      │   │\n");
                printf("│   │                            ####                   #########         │   │\n");
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
                
                free(creature1_pv_bar);
                free(creature2_pv_bar);

            }
            else if (type_combat ==0 && g_nbr_creatures_en_combat == 3) {
                CreatureMarine *c1 = &creatures[0];
                CreatureMarine *c2 = &creatures[1];
                CreatureMarine *c3 = &creatures[2];
                char *creature1_pv_bar = convert_to_visual_bar(c1->points_de_vie_actuels, c1->points_de_vie_max);
                char *creature2_pv_bar = convert_to_visual_bar(c2->points_de_vie_actuels, c2->points_de_vie_max);
                char *creature3_pv_bar = convert_to_visual_bar(c3->points_de_vie_actuels, c3->points_de_vie_max);
                
                printf("│   │   %10s  %3d%%     %10s  %3d%%         %10s  %3d%%    │   │\n",
                    creature1_pv_bar, 
                    convert_to_percent(c1->points_de_vie_actuels, c1->points_de_vie_max),
                    creature2_pv_bar, 
                    convert_to_percent(c2->points_de_vie_actuels, c2->points_de_vie_max),
                    creature3_pv_bar, 
                    convert_to_percent(c3->points_de_vie_actuels, c3->points_de_vie_max));
                printf("│   │   [%d]%12s      [%d]%12s          [%d]%12s     │   │\n", 
                    c1->id, c1->nom, c2->id, c2->nom, c3->id, c3->nom);
                printf("│   │                                                                     │   │\n");
                printf("│   │    #########               ####                   ##########        │   │\n");
                printf("│   │     ########             #########              ##############      │   │\n");
                printf("│   │   ###########              ####                   #########         │   │\n");
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
                
                free(creature1_pv_bar);
                free(creature2_pv_bar);
                free(creature3_pv_bar);
            }
            
            //printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            break;
        }
        case 11: { // Competences | Je me suis fait chier à aligner votre merdier in game, pensez y si vous changez quelque chose
            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────── COMPETENCES MARINES ─────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │  ╔════════════════════════════════════════════════════════════════╗ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║  [1] Brume marine                 [2] Cuirasse Aquatique       ║ │   │\n");
            printf("│   │  ║  ┌──────────────────────┐       ┌──────────────────────┐       ║ │   │\n");
            printf("│   │  ║  │ Cout: 20%% oxygene    │       │ Cout: 20%% oxygene    │       ║ │   │\n");
            printf("│   │  ║  │ Effet: 50%% esquive   │       │ Effet: -30%% degats   │       ║ │   │\n");
            printf("│   │  ║  │ Cooldown: 2 tours    │       │ Cooldown: 3 tours    │       ║ │   │\n");
            printf("│   │  ║  └──────────────────────┘       └──────────────────────┘       ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║  [3] Souffle maitrise           [4] Vague regenerante          ║ │   │\n");
            printf("│   │  ║  ┌──────────────────────┐       ┌──────────────────────┐       ║ │   │\n");
            printf("│   │  ║  │ Cout: 10%% oxygene    │       │ Cout: 25%% oxygene    │       ║ │   │\n");
            printf("│   │  ║  │ Effet: Fatigue/2     │       │ Effet: +25%% PV       │       ║ │   │\n");
            printf("│   │  ║  │ Cooldown: 3 tours    │       │ Effet immediat       │       ║ │   │\n");
            printf("│   │  ║  └──────────────────────┘       └──────────────────────┘       ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ╚════════════════════════════════════════════════════════════════╝ │   │\n");
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

            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │      ╔═══════╗ ╔═══════════════════════════════════╗  ╔═════╗       │   │\n");
            printf("│   │      ║  Carte╚╗║I]                                 ║ ╔╝     ╚╗      │   │\n");
            printf("│   │      ╚╗       ╚╝                                   ╚╗║       ║      │   │\n");
            printf("│   │       ╚╗                                         Pro╚╝ndeur  ║      │   │\n");
            printf("│   │       ╔╝                                                     ║      │   │\n");
            printf("│   │      ╔╝                                                      ║      │   │\n");
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  S  ║      │   │\n", c00, c01, c02, c03, c04, c05, c06, c07, c08, c09);
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  I  ║@@    │   │\n", c10, c11, c12, c13, c14, c15, c16, c17, c18, c19);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  I  ║ @@@  │   │\n", c20, c21, c22, c23, c24, c25, c26, c27, c28, c29);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼──╔╗┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@╚═╗        │╔╗       │           ╔╝║          │      II╔╝@@@@  │   │\n");
            printf("│   │  @@@@  ╚═════════╝╚═══════════════════╝ ╚═══════════════════╝ @@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");

            break;
        }
        case 22: { // Carte 2

            // Ligne 3
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

            // Ligne 4
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

            // Ligne 5
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

            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │      ╔══════════════════╗    ╔═══════════════╗╔═════════════╗       │   │\n");
            printf("│   │      ║  Carte  [II]     ║   ╔╝               ╚╝             ╚╗      │   │\n");
            printf("│   │      ║                  ║   ║                                ║      │   │\n");
            printf("│   │      ║                  ╚╗ ╔╝                    Profondeur  ║      │   │\n");
            printf("│   │      ║                   ╚╗║                                 ║      │   │\n");
            printf("│   │      ║                    ╚╝                                 ║      │   │\n");
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  II ║      │   │\n", c30, c31, c32, c33, c34, c35, c36, c37, c38, c39);
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  II ║@@    │   │\n", c40, c41, c42, c43, c44, c45, c46, c47, c48, c49);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s III ║ @@@  │   │\n", c50, c51, c52, c53, c54, c55, c56, c57, c58, c59);
            printf("│   │  @@@@╚╗────┼────┼────┼────┼────╔═╗──┼────┼────╔╗───┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@ ╚╗   │         │        ╔╝ ║  │         ║╚╗        III ║@@@@  │   │\n");
            printf("│   │  @@@@  ╚══════════════════════╝  ╚════════════╝ ╚════════════╝@@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");
            printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            printf("│                                                                             │\n");

            break;
        }
        case 23:{

            // Ligne 6
            const char* c60 = zone_already_visited(w, 6, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 0)) : " ?? ";
            const char* c61 = zone_already_visited(w, 6, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 1)) : " ?? ";
            const char* c62 = zone_already_visited(w, 6, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 2)) : " ?? ";
            const char* c63 = zone_already_visited(w, 6, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 3)) : " ?? ";
            const char* c64 = zone_already_visited(w, 6, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 4)) : " ?? ";
            const char* c65 = zone_already_visited(w, 6, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 5)) : " ?? ";
            const char* c66 = zone_already_visited(w, 6, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 6)) : " ?? ";
            const char* c67 = zone_already_visited(w, 6, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 7)) : " ?? ";
            const char* c68 = zone_already_visited(w, 6, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 8)) : " ?? ";
            const char* c69 = zone_already_visited(w, 6, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 6, 9)) : " ?? ";

            // Ligne 7
            const char* c70 = zone_already_visited(w, 7, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 0)) : " ?? ";
            const char* c71 = zone_already_visited(w, 7, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 1)) : " ?? ";
            const char* c72 = zone_already_visited(w, 7, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 2)) : " ?? ";
            const char* c73 = zone_already_visited(w, 7, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 3)) : " ?? ";
            const char* c74 = zone_already_visited(w, 7, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 4)) : " ?? ";
            const char* c75 = zone_already_visited(w, 7, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 5)) : " ?? ";
            const char* c76 = zone_already_visited(w, 7, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 6)) : " ?? ";
            const char* c77 = zone_already_visited(w, 7, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 7)) : " ?? ";
            const char* c78 = zone_already_visited(w, 7, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 8)) : " ?? ";
            const char* c79 = zone_already_visited(w, 7, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 7, 9)) : " ?? ";

            // Ligne 8
            const char* c80 = zone_already_visited(w, 8, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 0)) : " ?? ";
            const char* c81 = zone_already_visited(w, 8, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 1)) : " ?? ";
            const char* c82 = zone_already_visited(w, 8, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 2)) : " ?? ";
            const char* c83 = zone_already_visited(w, 8, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 3)) : " ?? ";
            const char* c84 = zone_already_visited(w, 8, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 4)) : " ?? ";
            const char* c85 = zone_already_visited(w, 8, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 5)) : " ?? ";
            const char* c86 = zone_already_visited(w, 8, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 6)) : " ?? ";
            const char* c87 = zone_already_visited(w, 8, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 7)) : " ?? ";
            const char* c88 = zone_already_visited(w, 8, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 8)) : " ?? ";
            const char* c89 = zone_already_visited(w, 8, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 9)) : " ?? ";

            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │       ╔════════════╗ ╔═══════════════╗  ╔═══════════════════╗       │   │\n");
            printf("│   │       ║ Carte  [III╚╗║               ║ ╔╝                   ╚╗      │   │\n");
            printf("│   │      ╔╝             ╚╝               ║╔╝                     ║      │   │\n");
            printf("│   │      ║                               ╚╝          Profondeur  ║      │   │\n");
            printf("│   │      ║                                                       ║      │   │\n");
            printf("│   │      ║                                                       ║      │   │\n");
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s III ║      │   │\n", c60, c61, c62, c63, c64, c65, c66, c67, c68, c69);;
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s III ║@@    │   │\n", c70, c71, c72, c73, c74, c75, c76, c77, c78, c79);;
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  IV ║ @@@  │   │\n", c80, c81, c82, c83, c84, c85, c86, c87, c88, c89);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼─╔═╗┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@║  ╔╗      │         ╔╗                   ╔╝ ║│      IV ║@@@@  │   │\n");
            printf("│   │  @@@@╚══╝╚════════════════╝╚═══════════════════╝  ╚══════════╝@@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");
            printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            printf("│                                                                             │\n");
            break;
        }
        case 24:{

            // Ligne 8
            const char* c80 = zone_already_visited(w, 8, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 0)) : " ?? ";
            const char* c81 = zone_already_visited(w, 8, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 1)) : " ?? ";
            const char* c82 = zone_already_visited(w, 8, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 2)) : " ?? ";
            const char* c83 = zone_already_visited(w, 8, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 3)) : " ?? ";
            const char* c84 = zone_already_visited(w, 8, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 4)) : " ?? ";
            const char* c85 = zone_already_visited(w, 8, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 5)) : " ?? ";
            const char* c86 = zone_already_visited(w, 8, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 6)) : " ?? ";
            const char* c87 = zone_already_visited(w, 8, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 7)) : " ?? ";
            const char* c88 = zone_already_visited(w, 8, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 8)) : " ?? ";
            const char* c89 = zone_already_visited(w, 8, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 8, 9)) : " ?? ";

            // Ligne 9
            const char* c90 = zone_already_visited(w, 9, 0) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 0)) : " ?? ";
            const char* c91 = zone_already_visited(w, 9, 1) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 1)) : " ?? ";
            const char* c92 = zone_already_visited(w, 9, 2) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 2)) : " ?? ";
            const char* c93 = zone_already_visited(w, 9, 3) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 3)) : " ?? ";
            const char* c94 = zone_already_visited(w, 9, 4) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 4)) : " ?? ";
            const char* c95 = zone_already_visited(w, 9, 5) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 5)) : " ?? ";
            const char* c96 = zone_already_visited(w, 9, 6) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 6)) : " ?? ";
            const char* c97 = zone_already_visited(w, 9, 7) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 7)) : " ?? ";
            const char* c98 = zone_already_visited(w, 9, 8) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 8)) : " ?? ";
            const char* c99 = zone_already_visited(w, 9, 9) ? zone_type_to_string_four_char(world_get_zone_type(w, 9, 9)) : " ?? ";

            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │      ╔═══════╗╔════════════════════════════════╗  ╔══════╗          │   │\n");
            printf("│   │      ║  Carte║║[IV]                            ╚╗ ║      ╚══╗       │   │\n");
            printf("│   │      ║       ╚╬╗                                ╚╗║         ╚╗      │   │\n");
            printf("│   │      ║        ╚╬╗                                ╚╝ofondeur  ║      │   │\n");
            printf("│   │      ║         ║║                                            ║      │   │\n");
            printf("│   │      ║         ╚╝                                            ║      │   │\n");
            printf("│   │      ║     │         │              │    │         │      IV ║      │   │\n");
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  IV ║@@    │   │\n", c80, c81, c82, c83, c84, c85, c86, c87, c88, c89);
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s   V ║ @@@  │   │\n", c90, c91, c92, c93, c94, c95, c96, c97, c98, c99);
            printf("│   │  @@@@║ ────┼────┼╔╗──┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@║     │    │║╚╗      │    │                   │        ╔╝@@@@  │   │\n");
            printf("│   │  @@@@╚═══════════╝ ╚════════════════════════════════════════╝ @@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");
            printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            printf("│                                                                             │\n");

            break;
        }
        case 3: { // Inventaire
            printf("│                                                                             │\n");
            printf("│   ╭──────────────────────────── Inventaire ─────────────────────────────╮   │\n");
            
            char buffer[100]; // Buffer pour sprintf
 

            // Équipement
            printf("│   │%-69s│   │\n", "     EQUIPEMENT:");
            
            ItemTemplate* weapon = get_item_template(p->equip_weapon.item_id);
            sprintf(buffer, "       Arme: %s (ATK: %d-%d)", weapon->nom, weapon->atk_min, weapon->atk_max);
            printf("│   │%-69s│   │\n", buffer);

            ItemTemplate* suit = get_item_template(p->equip_suit.item_id);
            sprintf(buffer, "       Combi: %s (DEF: +%d)", suit->nom, suit->defense);
            printf("│   │%-69s│   │\n", buffer);

            printf("│   │%-69s│   │\n", " ");

            // Perles
            sprintf(buffer, "     PERLES: %d", p->perles);
            printf("│   │%-69s│   │\n", buffer);
            printf("│   │%-69s│   │\n", " ");

            // CLES
            printf("Clef: %d \n", p->cle_test);

            // Sac 
            printf("│   │%-69s│   │\n", "     SAC A DOS:");

            for (int i = 0; i < INVENTORY_SIZE; i++) {
                ItemTemplate* item = get_item_template(p->inventaire[i].item_id);
                
                if (p->inventaire[i].item_id == 0) {
                    sprintf(buffer, "       [%d] Vide", i + 1);
                } else {
                    sprintf(buffer, "       [%d] %s x%d - %s", 
                        i + 1, 
                        item->nom, 
                        p->inventaire[i].quantite,
                        item->description);
                }
                
                printf("│   │%-69s│   │\n", buffer);
            }
            
            printf("│   │%-69s│   │\n", " "); // Remplissage (ligne 19)
            
            break;
        }
        case 50: {
            //printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n");
            //printf("│                                                                             │\n");
            printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │           ,---.                  .-,--.          .  .               │   │\n");
            printf("│   │           |   | ,-. ,-. ,-. ,-.  ' |   \\ ,-. ,-. |- |-.             │   │\n");
            printf("│   │           |   | |   |-' ,-| | |  , |   / |-' | | |  | |             │   │\n");
            printf("│   │           `---' `-' `-' `-^ ' '  `-^--'  `-' |-' `' ' '             │   │\n");
            printf("│   │                                              |                      │   │\n");
            printf("│   │                                              '                      │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                             [P] Plonger                             │   │\n");
            printf("│   │                             [C] Charger                             │   │\n");
            printf("│   │                             [Q] Quitter                             │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                                                                     │   │\n");

            break;
        }
        case 51: {
            printf("│   ╭──────────────────────── Nouvelle Exploration ───────────────────────╮   │\n");
            printf("│   │#########                                                            │   │\n");
            printf("│   │###               Demarrez une nouvelle exploration                  │   │\n");
            printf("│   │#                                                                    │   │\n");
            printf("│   │                                                      <°(((><        │   │\n");
            printf("│   │                                         <°(((><                     │   │\n");
            printf("│   │       ╔═══╦═══════════════════╗                                     │   │\n");
            printf("│   │       ║ 1 ║ %-17s ║                                     │   │\n", saveName1);
            printf("│   │       ╚═══╩═══════════════════╝                                     │   │\n");
            printf("│   │  ><>                                                                │   │\n");
            printf("│   │                      ╔═══╦═══════════════════╗                      │   │\n");
            printf("│   │     \\\\   ><>         ║ 2 ║ %-17s ║                      │   │\n", saveName2);
            printf("│   │  \\\\ //            \\\\ ╚═══╩═══════════════════╝                      │   │\n");
            printf("│   │  || \\\\     ><>     \\\\                                               │   │\n");
            printf("│   │  \\\\ //  ><>        //               ╔═══╦═══════════════════╗       │   │\n");
            printf("│   │   \\Y/             //   ><>          ║ 3 ║ %-17s ║       │   │\n", saveName3);
            printf("│   │   // ><>          ||                ╚═══╩═══════════════════╝       │   │\n");
            printf("│   │##############     ||                                                │   │\n");
            printf("│   │##############################                          <*)))==<     │   │\n");
            printf("│   │###################################                                  │   │\n");

            break;
        }
        case 52: {
            printf("│   ╭───────────────────── Continuer une Exploration ─────────────────────╮   │\n");
            printf("│   │#########                                                            │   │\n");
            printf("│   │###                     Choisir une sauvegarde                       │   │\n");
            printf("│   │#                                                                    │   │\n");
            printf("│   │                                                      <°(((><        │   │\n");
            printf("│   │                                         <°(((><                     │   │\n");
            printf("│   │       ╔═══╦═══════════════════╗                                     │   │\n");
            printf("│   │       ║ 1 ║ %-17s ║                                     │   │\n", saveName1);
            printf("│   │       ╚═══╩═══════════════════╝                                     │   │\n");
            printf("│   │  ><>                                                                │   │\n");
            printf("│   │                      ╔═══╦═══════════════════╗                      │   │\n");
            printf("│   │     \\\\   ><>         ║ 2 ║ %-17s ║                      │   │\n", saveName2);
            printf("│   │  \\\\ //            \\\\ ╚═══╩═══════════════════╝                      │   │\n");
            printf("│   │  || \\\\     ><>     \\\\                                               │   │\n");
            printf("│   │  \\\\ //  ><>        //               ╔═══╦═══════════════════╗       │   │\n");
            printf("│   │   \\Y/             //   ><>          ║ 3 ║ %-17s ║       │   │\n", saveName3);
            printf("│   │   // ><>          ||                ╚═══╩═══════════════════╝       │   │\n");
            printf("│   │##############     ||                                                │   │\n");
            printf("│   │##############################                          <*)))==<     │   │\n");
            printf("│   │###################################                                  │   │\n");

            break;
        }
        // Marchand
        case 99:{
                printf("│                                                             ╰───────────────┤\n");
                printf("│   ╭───────────────────────────── Marchand ──────────────────────────────╮   │\n");
                printf("|   | Perles: %d                                                          |   |\n", p->perles);
                printf("│   │ ID | %-20s | %-20s | Prix | Stock      │   │\n", "Nom", "Description");
                printf("│   │─────────────────────────────────────────────────────────────────────│   │\n");
                
                // Afficher les 10 items
                for (int i = 0; i < g_item_marchand_db_size; i++) {
                    MarchandItem *obj = &g_item_marchand_db[i];
                    ItemTemplate *item = get_item_template(obj->item_id);
                    int display_id = (i + 1) % 10; // 1-9 puis 0 pour le 10ème
                    printf("│   │ %2d | %-20s | %-20s | %4d | %5d      │   │\n", 
                        display_id == 0 ? 0 : display_id, 
                        item->nom, item->description, obj->prix_perles, obj->stock);
                }
                
                // Remplir les lignes restantes avec des espaces pour atteindre 19 lignes
                for (int i = g_item_marchand_db_size; i < hauteur - 2; i++) { 
                    printf("│   │                                                                     │   │\n");
                }
                
                printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
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
    if(strcmp(zone_type_to_string_four_char(type), "Grot") == 0){
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
        case 10:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [A] Attaque légère  [B] Attaque Lourde  [C] S'économiser  [D] Compétence   │\n");
            printf("│  [I] Inventaire  [Q] Quitter                                                │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        // Competence
        case 11:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [1] Brume marine  [2] Cuirasse Aquatique  [3] Souffle maitrise             │\n");
            printf("│  [4] Vague regenerante [Q] Retour                                           │\n");
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
        case 21:
        case 22:
        case 23:
        case 24:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [R] Retour  [Q] Quitter                                                    │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }

        // Inventaire
        case 3:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [1] Utiliser Objet  [2] Equiper Objet  [Q] Retour                          │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        // Coffre
        case 4:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [Q] Quitter                                                                │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 50:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [P] Plonger  [C] Charger  [Q] Quitter                                      │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        case 51:
        case 52: {
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [1] %-17s  [2] %-17s  [3] %-17s        │\n", saveName1, saveName2, saveName3);
            printf("│  [R] Retour                                                                 │\n");
            printf("╰─────────────────────────────────────────────────────────────────────────────╯\n");
            break;
        }
        // Marchand
        case 99:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  [1-9] Sélectionner item  [I] Inventaire  [Q] Quitter                       │\n");
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

void full_screen(World *w, Plongeur *p, CreatureMarine *creatures, char** screen, char* info){
    char* pv_bar = convert_to_visual_bar(p->points_de_vie, p->points_de_vie_max);
    char* oxy_bar = convert_to_visual_bar(p->niveau_oxygene, p->niveau_oxygene_max);
    char* fatigue_bar = convert_to_visual_bar(p->niveau_fatigue, p->niveau_fatigue_max);

    screen_header(w, p, pv_bar, oxy_bar, fatigue_bar, info);
    screen_main(w, p, creatures, screen);
    screen_footer(w, p);

    free(pv_bar);
    free(oxy_bar);
    free(fatigue_bar);
}

 
