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

 // Les couleurs c'est du bonus pour l'instant la flore et le player
// (
void print_screen(char **screen) {
    switch (screen_status){
        case 0: { // Exploration : avec labels ligne
            for (int i = 0; i < hauteur; i++) {
                char label;
                if (i < 10){
                    label = '0' + i;
                } else {
                    label = 'A' + (i - 10);
                }
                printf("│  %c│", label);
                for (int j = 0; j < largeur; j++) {
                    char c = screen[i][j];
                    // roche - grise
                    //if (c == '#'){
                    //    printf("\x1b[38;2;120;120;120m%c\x1b[0m", c);
                    //}
                    // algue verte (et corail (dommage collateral)
                    if (c == 'Y' || c == '|' || c=='/' || c=='\\' || c == '0'){
                        printf("\x1b[32m%c\x1b[0m", c);
                    }
                    else if (c == '@')
                    {
                        printf("\x1b[31m%c\x1b[0m", c);
                    }
                    // Poisson
                    //else if (c == '<' || c == '>' || c=='(' || c==')' || c=='o' || c=='*' || c== '='){
                    //    printf("\x1b[0m%c\x1b[0m", c);
                    //}
                    // Fill - Bleu (pour l'instant ce sont des ' ', ça peut etre des '~')
                    // qui sait ce que nous reserve l'avenir
                    //else if (c == fill){
                    //    printf("\x1b[34m%c\x1b[0m", c);
                    //}
                    else{
                        printf("%c", c);
                    }

                }
                printf("│   │\n");
            }
            break;
        }
        default: { // autres écrans : bords classiques
            for (int i = 0; i < hauteur; i++) {
                printf("│   │");
                for (int j = 0; j < largeur; j++)
                {
                    char c = screen[i][j];
                    printf("%c", c);

                }
                printf("│   │\n");
            }
            break;
        }
    }
}

// Couleurs
const char *C_RESET  = "\x1b[0m";
const char *C_ROUGE  = "\x1b[31m";
const char *C_VERT   = "\x1b[32m";
const char *C_JAUNE  = "\x1b[33m";
const char *C_BLEU   = "\x1b[34m";
const char *C_VIOLET = "\x1b[35m";

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

            char* profondeur;
            switch (convert_y_to_depth_lvl(p->map_pos_y)){
                case 0: profondeur = "Surface"; break;
                case 1: profondeur = "I"; break;
                case 2: profondeur = "II"; break;
                case 3: profondeur = "III"; break;
                case 4: profondeur = "IV"; break;
                case 5: profondeur = "V"; break;
            }
            int pv      = p->points_de_vie;
            int oxy     = p->niveau_oxygene;
            int fatigue = p->niveau_fatigue;

            ItemTemplate *weapon = get_item_template(p->equip_weapon.item_id);
            printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n");

            // Pv
            const char *col_pv;
            if      (pv <= 10) col_pv = C_ROUGE;   // 0-10%
            else if (pv <= 60) col_pv = C_JAUNE;  // 11-60%
            else               col_pv = C_VERT;  // 61-100%

            printf("│  Vie: %s%s %3d%%%s  │", col_pv, pv_bar, pv, C_RESET);

            // Oxy
            const char *col_oxy;
            if      (oxy <= 10) col_oxy = C_ROUGE;    // 0-10%
            else if (oxy <= 40) col_oxy = C_VIOLET;  // 11-40%
            else                col_oxy = C_BLEU;   // 41-100%

            printf("  O₂: %s%s %3d%%%s  │", col_oxy, oxy_bar, oxy, C_RESET);

            // Fatigue
            const char *col_fat;
            if      (fatigue >= 70) col_fat = C_ROUGE;    // 70-100%
            else                    col_fat = C_JAUNE;   // 0-69%

            printf("  Fatigue: %s%s %3d%%%s  │\n", col_fat, fatigue_bar, fatigue, C_RESET);

            printf("│  Profondeur: %-10s", profondeur);
            printf("│  Arme équipé : %-24s | Save : %d │\n", weapon->nom, active_save);
            printf("├─────────────────────────────────────────────────────────────┬───────────────┤\n");

            // Prevention pv / oxy low
            int urge_info;
            if (p->points_de_vie <= 10 && p->niveau_oxygene <= 10) urge_info = 0;
            else if (p->points_de_vie <= 10)                       urge_info = 1;
            else if (p->niveau_oxygene <= 10)                      urge_info = 2;
            else                                                   urge_info = 5;

            switch (urge_info){
            case 0: info = "Niveau d'oxygene et de sante critique"; break;
            case 1: info = "Niveau de sante critique";              break;
            case 2: info = "Niveau d'oxygene critique";             break;
            default:;
            }

            const char *info_color = C_RESET;

            switch (urge_info){
            case 0: info_color = C_ROUGE;    break; // double critique
            case 1: info_color = C_ROUGE;    break; // PV critique
            case 2: info_color = C_ROUGE;    break; // O2 critique
            default: info_color = C_RESET; break; // info "normale" / neutre
            }

            printf("│  [Info] : %s%-48s%s  │  Zone-[%d][%d]  │\n",info_color, info, C_RESET, p->map_pos_y, p->map_pos_x);
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
        case 12: { // Loading combat (si gagnant ou vaincu)
            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────── LOADING ─────────────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │  ╔════════════════════════════════════════════════════════════════╗ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");

            if (g_victoire) {
                printf("│   │  ║                          VICTOIRE                              ║ │   │\n");
                printf("│   │  ║                    Vous avez gagné %d Perles                   ║ │   │\n", g_perles_gagnees);
            } else {
                printf("│   │  ║                          GAME OVER                             ║ │   │\n");
                printf("│   │  ║                    Vous avez perdu %d Perles                   ║ │   │\n", g_perles_perdues);
            }

            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ║                                                                ║ │   │\n");
            printf("│   │  ╚════════════════════════════════════════════════════════════════╝ │   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│                                                                             │\n");
            printf("│                                                                             │\n");


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
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  S  ║      │   │\n", cc(w,0,0), cc(w,0,1), cc(w,0,2), cc(w,0,3), cc(w,0,4), cc(w,0,5), cc(w,0,6), cc(w,0,7), cc(w,0,8), cc(w,0,9));
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  I  ║@@    │   │\n", cc(w,1,0), cc(w,1,1), cc(w,1,2), cc(w,1,3), cc(w,1,4), cc(w,1,5), cc(w,1,6), cc(w,1,7), cc(w,1,8), cc(w,1,9));
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  I  ║ @@@  │   │\n", cc(w,2,0), cc(w,2,1), cc(w,2,2), cc(w,2,3), cc(w,2,4), cc(w,2,5), cc(w,2,6), cc(w,2,7), cc(w,2,8), cc(w,2,9));
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼──╔╗┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@╚═╗        │╔╗       │           ╔╝║          │      II╔╝@@@@  │   │\n");
            printf("│   │  @@@@  ╚═════════╝╚═══════════════════╝ ╚═══════════════════╝ @@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");

            break;
        }
        case 22: { // Carte 2

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
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  II ║      │   │\n", cc(w,3,0), cc(w,3,1), cc(w,3,2), cc(w,3,3), cc(w,3,4), cc(w,3,5), cc(w,3,6), cc(w,3,7), cc(w,3,8), cc(w,3,9));
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  II ║@@    │   │\n", cc(w,4,0), cc(w,4,1), cc(w,4,2), cc(w,4,3), cc(w,4,4), cc(w,4,5), cc(w,4,6), cc(w,4,7), cc(w,4,8), cc(w,4,9));
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s III ║ @@@  │   │\n", cc(w,5,0), cc(w,5,1), cc(w,5,2), cc(w,5,3), cc(w,5,4), cc(w,5,5), cc(w,5,6), cc(w,5,7), cc(w,5,8), cc(w,5,9));
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
            printf("│   │      ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s III ║      │   │\n", cc(w,6,0), cc(w,6,1), cc(w,6,2), cc(w,6,3), cc(w,6,4), cc(w,6,5), cc(w,6,6), cc(w,6,7), cc(w,6,8), cc(w,6,9));;
            printf("│   │    @@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@    │   │\n");
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  IV ║@@    │   │\n", cc(w,7,0), cc(w,7,1), cc(w,7,2), cc(w,7,3), cc(w,7,4), cc(w,7,5), cc(w,7,6), cc(w,7,7), cc(w,7,8), cc(w,7,9));;
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s  IV ║ @@@  │   │\n", cc(w,8,0), cc(w,8,1), cc(w,8,2), cc(w,8,3), cc(w,8,4), cc(w,8,5), cc(w,8,6), cc(w,8,7), cc(w,8,8), cc(w,8,9));
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼─╔═╗┼────     ║@@@@  │   │\n");
            printf("│   │  @@@@║  ╔╗      │         ╔╗                   ╔╝ ║│       V ║@@@@  │   │\n");
            printf("│   │  @@@@╚══╝╚════════════════╝╚═══════════════════╝  ╚══════════╝@@@@@ │   │\n");
            printf("│   │@@@@@@@@@@                                                 @@@@@@@@@@│   │\n");
            printf("│   │@@@@@@                                                         @@@@@@│   │\n");
            printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            printf("│                                                                             │\n");
            break;
        }
        case 24:{

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
            printf("│   │    @@║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s   V ║@@    │   │\n", cc(w,9,0), cc(w,9,1), cc(w,9,2), cc(w,9,3), cc(w,9,4), cc(w,9,5), cc(w,9,6), cc(w,9,7), cc(w,9,8), cc(w,9,9));
            printf("│   │  @@@@║ ────┼────┼────┼────┼────┼────┼────┼────┼────┼────     ║@@@@  │   │\n");
            printf("│   │  @@@ ║ %-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s│%-4s   V ║ @@@  │   │\n", cc(w,10,0), cc(w,10,1), cc(w,10,2), cc(w,10,3), cc(w,10,4), cc(w,10,5), cc(w,10,6), cc(w,10,7), cc(w,10,8), cc(w,10,9));
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
            
            char buffer[100];
 
            printf("│   │%-69s│   │\n", "     EQUIPEMENT:");
            
            ItemTemplate* weapon = get_item_template(p->equip_weapon.item_id);
            sprintf(buffer, "       Arme: %s (ATK: %d-%d)", weapon->nom, weapon->atk_min, weapon->atk_max);
            printf("│   │%-69s│   │\n", buffer);

            ItemTemplate* suit = get_item_template(p->equip_suit.item_id);
            sprintf(buffer, "       Combi: %s (DEF: +%d)", suit->nom, suit->defense);
            printf("│   │%-69s│   │\n", buffer);

            printf("│   │%-69s│   │\n", " ");

            sprintf(buffer, "     PERLES: %d", p->perles);
            printf("│   │%-69s│   │\n", buffer);
            printf("│   │%-69s│   │\n", " ");

            // CLES
            sprintf(buffer, "     Clef: %d", p->cle);
            printf("│   │%-69s│   │\n", buffer);
            printf("│   │%-69s│   │\n", " ");

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
            
            printf("│   │%-69s│   │\n", " ");
            
            break;
        }
        //Tresor
        case 4:{
            ItemTemplate *item_coffre = get_item_template(g_id_item_coffre);
            printf("│                                                             ╰───────────────┤\n");
            printf("│   ╭─────────────────────────────── Trésor ──────────────────────────────╮   │\n");
            printf("│   │                                                                     │   │\n");
            printf("│   │                       #######################                       │   │\n");
            printf("│   │               ############     #####      ############              │   │\n");
            printf("│   │          #######                ###                #######          │   │\n");
            printf("│   │           ##   #                                   #   ##           │   │\n");
            printf("│   │            ##   #  ##  ## #  ## # # ##  # ##  ##  #   ##            │   │\n");
            printf("│   │             ## #        ╭─────────────────╮        # ##             │   │\n");
            printf("│   │              ##         │%-17s│         ##              │   │\n", item_coffre->nom);
            printf("│   │               ##########│%-17s│##########               │   │\n", item_coffre->description);
            if(item_coffre->type == ITEM_TYPE_WEAPON) {
                printf("│   │              ##         │Degats : %-2d-%-2d   │         ##              │   │\n", item_coffre->atk_min, item_coffre->atk_max);
                printf("│   │             ## #        │Conso : %-2d       │        # ##             │   │\n", item_coffre->o2_cost_atk);
            } else {
                printf("│   │              ##         │                 │         ##              │   │\n");
                printf("│   │             ## #        │Defense : +%-2d    │        # ##             │   │\n", item_coffre->defense);
            }
            printf("│   │            ##   #       ╰─────────────────╯       #   ##            │   │\n");
            printf("│   │           ##     #                               #     ##           │   │\n");
            printf("│   │          #################################################          │   │\n");
            printf("│   │           ######################   ######################           │   │\n");
            printf("│   │            #                   #####                   #            │   │\n");
            printf("│   │             #                                         #             │   │\n");
            printf("│   │              #########################################              │   │\n");
            printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
            printf("│                                                                             │\n");
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
        case 12:{
            printf("├─────────────────────────────────────────────────────────────────────────────┤\n");
            printf("│  Appuyez sur [Q] pour continuer...                                          │\n");
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
            printf("│                    [Appuyez sur [Q] pour continuer]                         │\n");
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

 
