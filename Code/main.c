#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "save/save.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "globals/globals.h"
#include "types/types.h"
#include "world/world.h"
#include "render/render.h"
#include "input/input.h"
#include "player/player.h"

#include "creature/creature.h"
#include "combat/combat.h"
#include "inventaire/inventaire.h"

/**
 *  Info screen_status :
 *
 *  1 : Combat
 *      10 : Combat Actif
 *      11 : Competences
 *
 *  2 : Cartes
 *      20 : Choix Carte
 *      21 : Carte 1
 *      22 : Carte 2
 *      23 : Carte 3
 *      24 : Carte 4
 *
 *  3 :
 *      3 : Inventaire
 *
 *  4 :
 *      4 : Tresor
 *
 *  5 :
 *      50 :
 *      51 :
 *
 *
 */
int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // affichage console wd
#endif
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

        .pos_y      = 5,
        .pos_x      = 10,
        .last_pos_y = 5,
        .last_pos_x = 10,

        .map_pos_y  = 1,
        .map_pos_x  = 0,

    };

    init_player_inventory(&plongeur);


    World *world = init_world(map_hauteur, map_largeur); // h, l
    marquer_zone_as_visited(world, plongeur.map_pos_y, plongeur.map_pos_x); // zone depart => visited
    // Pas le top, mais j'ai fais comme ca au debut, et c'est trop tard pour changer.
    // ────────── Boucle de gameplay ──────────
    int running = 1;
    while (running) {

        // Recup zone a afficher => zone dans laquelle est le joueur
        char **screen = world_current_zone(world, &plongeur);

        // Affichage
        full_screen(world, &plongeur, g_creatures_en_combat, screen, info);

        // Récup input user validé selon l'écran courant
        char cmd = prompt_for_command(world, &plongeur, screen_status);
        if (cmd == '\0') break; // fin d'entrée

        // if (screen_status != 3) {
        //     previous_screen_status = screen_status;
        // }


        switch (screen_status) {

            // ── Exploration ─────────────────────────────────────────────
            case 0: {
                if (cmd == 'C' || cmd == 'c') {
                    screen_status = 20; // Carte
                }
                else if (cmd == 'I' || cmd == 'i') {
                    previous_screen_status = 0;
                    screen_status = 3; // Inventaire
                }
                else if (cmd == 'D' || cmd == 'd') {
                    demander_player_for_coords(screen, &plongeur, world);
                    
                }
                else if (cmd == 'S' || cmd == 's') {
                    info = "Progression sauvegarde";
                    sauvegarder(world, &plongeur, 1);
                    //screen_status = 50;

                }
                else if (cmd == 'G') {
                    info = "Charger";
                    //charger(&plongeur);
                }
                else if (cmd == 'E') {
                    screen_status = 1;
                    
                }
                break;
            }

            // ── Combat ─────────────────────────────────────────────────
            case 10: {
                // Logique combat
                previous_screen_status = screen_status;
                gerer_tour_combat(&plongeur, cmd, screen);
                break;
            }
            case 11:{ //Competences
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 10;  // Retour combat
                    info = "Retour au combat";
                }
                // c'est quoi ce raffut que vous me pondez encore
                else if (cmd >= '1' && cmd <= '4') {
                    appliquer_competence(&plongeur, cmd);
                    screen_status = 10;  // Retour combat
                }
                break;
            }

            // ── Carte ──────────────────────────────────────────────────
            case 20: { // ecran selection
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                else if (cmd == '1') {
                    screen_status = 21; // go Carte 1
                }
                else if (cmd == '2') {
                    screen_status = 22; // go Carte 2
                }
                else if (cmd == '3') {
                    screen_status = 23; // go Carte 3
                }
                else if (cmd == '4') {
                    screen_status = 24; // go Carte 4
                }
                break;
            }
            case 21: { // Carte 1
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                else if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }
            case 22: { // Carte 2
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                else if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }
            case 23: { // Carte 3
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                else if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }
            case 24: { // Carte 4
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                else if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }

            // ── Inventaire ─────────────────────────────────────────────
            case 3: {
                gerer_inventaire(cmd, &plongeur);
                break;
            }

            // ── Trésor ─────────────────────────────────────────────────
            case 4: {
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0;
                }
                break;
            }

            // ── Sauvegarde ─────────────────────────────────────────────
            case 50: {

                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0;
                }
                else if (cmd == '1') {
                    sauvegarder(world, &plongeur , 1);
                }
                else if (cmd == '2') {
                    sauvegarder(world, &plongeur , 2);
                }
                else if (cmd == '3') {
                    sauvegarder(world, &plongeur , 3);
                }
                break;
            }

            default:
                screen_status = 0;
                break;
        }

        printf("\n"); // petite séparation
    }

    free_world(world);
    if (g_creatures_en_combat) free(g_creatures_en_combat);
    return 0;
}
