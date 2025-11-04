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
#include "../inventaire/inventaire.h"

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
        case 1: { // Combat
            printf("│                                                              ╰──────────────┤\n");
            printf("│   ╭───────────────────────────── Combat ────────────────────────────────╮   │\n");
            
            if (g_nbr_creatures_en_combat == 1) {
                CreatureMarine *c1 = &creatures[0];
                char *creature1_pv_bar = convert_to_visual_bar(c1->points_de_vie_actuels, c1->points_de_vie_max);
                
                printf("│   │                                                   %10s  %3d%%       │   │\n", 
                    creature1_pv_bar, 
                    convert_to_percent(c1->points_de_vie_actuels, c1->points_de_vie_max));
                printf("│   │                                                   [%d]%12s       │   │\n", 
                    c1->id, c1->nom);
                printf("│   │                                                                     │   │\n");
                printf("│   │                                                   ##########        │   │\n");
                printf("│   │                                                 ##############      │   │\n");
                printf("│   │                                                   #########         │   │\n");
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
            else if (g_nbr_creatures_en_combat == 2) {
                CreatureMarine *c1 = &creatures[0];
                CreatureMarine *c2 = &creatures[1];
                char *creature1_pv_bar = convert_to_visual_bar(c1->points_de_vie_actuels, c1->points_de_vie_max);
                char *creature2_pv_bar = convert_to_visual_bar(c2->points_de_vie_actuels, c2->points_de_vie_max);
                
                printf("│   │                        %10s  %3d%%               %10s  %3d%%        │   │\n", 
                    creature1_pv_bar, 
                    convert_to_percent(c1->points_de_vie_actuels, c1->points_de_vie_max),
                    creature2_pv_bar, 
                    convert_to_percent(c2->points_de_vie_actuels, c2->points_de_vie_max));
                printf("│   |                             [%d]%12s                         [%d]%12s          │   │\n", 
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
            else if (g_nbr_creatures_en_combat == 3) {
                CreatureMarine *c1 = &creatures[0];
                CreatureMarine *c2 = &creatures[1];
                CreatureMarine *c3 = &creatures[2];
                char *creature1_pv_bar = convert_to_visual_bar(c1->points_de_vie_actuels, c1->points_de_vie_max);
                char *creature2_pv_bar = convert_to_visual_bar(c2->points_de_vie_actuels, c2->points_de_vie_max);
                char *creature3_pv_bar = convert_to_visual_bar(c3->points_de_vie_actuels, c3->points_de_vie_max);
                
                printf("│   │   %10s  %3d%%      %10s  %3d%%          %10s  %3d%%     │   │\n", 
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
            
            printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n");
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
            printf("│  [A] Attaque légère  [B] Attaque Lourde  [C] Compétence  [E] Économiser     │\n");
            printf("│  [I] Inventaire  [Q] Quitter                                                │\n");
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
            printf("│  [1] Utiliser Objet  [2] Equiper Objet  [Q] Retour                          │\n");
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

 