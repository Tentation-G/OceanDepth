#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "../globals/globals.h"
#include "../world/world.h"
#include "../utils/utils.h"
#include "../input/input.h"


void layer_player(Plongeur *p, Zone zone) {
    // Mémoire persistante du dernier rendu du joueur
    static Zone prev_zone = NULL;
    static int  prev_y = -1, prev_x = -1;
    static char saved_tile = ' ';
    static int  has_prev = 0;

    // Si '@' (joueur) deja draw => restaurer ancienne case
    if (has_prev) {
        // Si changement de zone => restaurer dans ancienne zone
        if (prev_zone && (zone != prev_zone)) {
            if (prev_y >= 0 && prev_y < hauteur && prev_x >= 0 && prev_x < largeur) {
                prev_zone[prev_y][prev_x] = saved_tile;
            }
        }
        // Même zone => restaurer seulement si la case a changé
        else if (zone == prev_zone) {
            if ((prev_y != p->pos_y) || (prev_x != p->pos_x)) {
                if (prev_y >= 0 && prev_y < hauteur && prev_x >= 0 && prev_x < largeur) {
                    zone[prev_y][prev_x] = saved_tile;
                }
            }
        }
    }

    // Choper le contenu de la case actuelle avant de draw '@' (joueur)
    // Dessiner le joueur si coordonnées valides
    if (p->pos_y >= 0 && p->pos_y < hauteur && p->pos_x >= 0 && p->pos_x < largeur) {
        char cur = zone[p->pos_y][p->pos_x];

        // Patch bug :
        // Ne pas écraser saved_tile avec '@' si on est sur meme case de meme zone
        // que la dernière fois (Car probleme si deplacement sur meme case et changement d'ecran).
        if (!(cur == '@' && has_prev && zone == prev_zone &&
              prev_y == p->pos_y && prev_x == p->pos_x)) {
            saved_tile = cur;
        }

        zone[p->pos_y][p->pos_x] = '@';
        prev_y    = p->pos_y;
        prev_x    = p->pos_x;
        prev_zone = zone;
        has_prev  = 1;
    }
}

int distance_entre_pos(Plongeur *p){
    int dist_y = abs(p->last_pos_y - p->pos_y);
    int dist_x = abs(p->last_pos_x - p->pos_x);
    double dist = sqrt(dist_y*dist_y + dist_x*dist_x);
    dist = (int)(dist + 0.5);
    return (int)dist;
}
// return le signe, (ex : v=2 => 1 | v=0 => 0 | v=-2 => -1)
int vecteur_dir(int valeur){
    if (valeur > 0) return 1;
    if (valeur < 0) return -1;
    return 0;
}

void demander_player_for_coords(char **screen, Plongeur *p, World *w) {
    char input[8];

    for (;;) {
        printf("Entrez des coordonnées [↓][→] : ");
        if (scanf(" %7s", input) != 1) {
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

        // Traitement de l'info sur arrivé de la case
        action_apres_deplacement(p, /*c=*/NULL, y, x, screen, w, screen); // (screen est une Zone zone)
        break;
    }
}

void action_apres_deplacement(Plongeur *p, CreatureMarine *c, int y, int x, char **screen, World *w, Zone zone){
    int dist = distance_entre_pos(p);
    int cout_oxy;
    if (dist != 0){
        cout_oxy = dist/5;
        if (cout_oxy < 1){ cout_oxy = 1;}
    } else {
        cout_oxy = 0;
    }
    p->niveau_oxygene = clamp(p->niveau_oxygene - cout_oxy, p->niveau_oxygene_max);

    int min_mt_recup_fatigue = 0;
    int max_mt_recup_fatigue = 20;
    int montant_fatigue_recup = min_mt_recup_fatigue + rand() % (max_mt_recup_fatigue - min_mt_recup_fatigue + 1);
    p->niveau_fatigue = clamp(p->niveau_fatigue - montant_fatigue_recup, p->niveau_fatigue_max);

    if (screen_status == 0) { // Exploration
        switch (screen[y][x]) {
        case '^': {
            info = "Sortie NORD";
            if (ask_yes_no("Monter vers la zone au NORD ?")) {
                p->map_pos_y -= 1;
                p->pos_y = hauteur - 2;
                if (p->pos_x < 0) p->pos_x = 0;
                if (p->pos_x >= largeur) p->pos_x = largeur - 1;
                p->last_pos_y = p->pos_y;
                p->last_pos_x = p->pos_x;
                int first_time = !zone_already_visited(w, p->map_pos_y, p->map_pos_x);
                marquer_zone_as_visited(w, p->map_pos_y, p->map_pos_x);

                if (first_time) { info = "Nouvelle zone exploree."; }
                else { info = "Vous avez changé de zone (NORD)."; }
            } else {
                info = "Vous restez dans la zone.";
            }
            break;
        }

        case 'v': {
            info = "Sortie SUD";
            if (ask_yes_no("Descendre vers la zone au SUD ?")) {
                p->map_pos_y += 1;
                p->pos_y = 1;
                if (p->pos_x < 0) p->pos_x = 0;
                if (p->pos_x >= largeur) p->pos_x = largeur - 1;
                p->last_pos_y = p->pos_y;
                p->last_pos_x = p->pos_x;

                int first_time = !zone_already_visited(w, p->map_pos_y, p->map_pos_x);
                marquer_zone_as_visited(w, p->map_pos_y, p->map_pos_x);

                if (first_time) { info = "Nouvelle zone exploree."; }
                else { info = "Vous avez changé de zone (SUD)."; }
            } else {
                info = "Vous restez dans la zone.";
            }
            break;
        }

        case '<': {
            info = "Sortie OUEST";
            if (ask_yes_no("Aller vers la zone à l'OUEST ?")) {
                p->map_pos_x -= 1;
                p->pos_x = largeur - 2;
                if (p->pos_y < 0) p->pos_y = 0;
                if (p->pos_y >= hauteur) p->pos_y = hauteur - 1;
                p->last_pos_y = p->pos_y;
                p->last_pos_x = p->pos_x;

                int first_time = !zone_already_visited(w, p->map_pos_y, p->map_pos_x);
                marquer_zone_as_visited(w, p->map_pos_y, p->map_pos_x);

                if (first_time) { info = "Nouvelle zone exploree."; }
                else { info = "Vous avez changé de zone (OUEST)."; }
            } else {
                info = "Vous restez dans la zone.";
            }
            break;
        }

        case '>': {
            info = "Sortie EST";
            if (ask_yes_no("Aller vers la zone à l'EST ?")) {
                p->map_pos_x += 1;
                p->pos_x = 1;
                if (p->pos_y < 0) p->pos_y = 0;
                if (p->pos_y >= hauteur) p->pos_y = hauteur - 1;
                p->last_pos_y = p->pos_y;
                p->last_pos_x = p->pos_x;

                int first_time = !zone_already_visited(w, p->map_pos_y, p->map_pos_x);
                marquer_zone_as_visited(w, p->map_pos_y, p->map_pos_x);

                if (first_time) { info = "Nouvelle zone exploree."; }
                else { info = "Vous avez changé de zone (EST)."; }
            } else {
                info = "Vous restez dans la zone.";
            }
            break;
        }

        case '?':
            info = "Qu'est-ce que c'est que ce raffut ?";
            break;

        case 'T':
            printf("Declancher fonction pour le tresord (affichage ecran tresor => generation du loot => affichage du loot)\n");
            break;

        case '#': {
            //info = "Tu as heurté un rocher, -5 PV.";
            p->points_de_vie -= 5;

            // dir brute
            int dir_y_brut = p->pos_y - p->last_pos_y;
            int dir_x_brut = p->pos_x - p->last_pos_x;
            // Normal de la dir
            int dir_y = vecteur_dir(dir_y_brut);
            int dir_x = vecteur_dir(dir_x_brut);

            // si pas de mouvement
            if (dir_y == 0 && dir_x == 0) {
                if (in_screen_lim(p->last_pos_y, p->last_pos_x) &&
                    zone[p->last_pos_y][p->last_pos_x] == ' ') {
                    p->pos_y = p->last_pos_y;
                    p->pos_x = p->last_pos_x;
                    p->last_pos_y = p->pos_y;  // évite double cout O2
                    p->last_pos_x = p->pos_x;
                    info = "Vous avez heurte une paroi, -5 hp";
                } else {
                    info = "Vous avez heurte une paroi, -5 hp";
                }
                break;
            }

            // Var pour reculer dans la dir opposé
            int back_y = -dir_y;
            int back_x = -dir_x;

            int moved = 0;

            // Reculer dans la dite dir until on tombe sur une case libre (ou sortie d'ecran)
            while (1) {
                // prochaine pos y et x a tester
                int proch_pos_y = p->pos_y + back_y;
                int proch_pos_x = p->pos_x + back_x;

                if (!in_screen_lim(proch_pos_y, proch_pos_x)){  // Si hors ecran => on break
                    break;
                }
                if (zone[proch_pos_y][proch_pos_x] == ' ') { // trouvé une case libre
                    p->pos_y = proch_pos_y;
                    p->pos_x = proch_pos_x;
                    moved = 1;
                    break;
                }
                if (zone[proch_pos_y][proch_pos_x] != '#'){ // si on tombe sur autre chose qu'un mur
                    break;
                }

                // Continuer a reculer
                p->pos_y = proch_pos_y;
                p->pos_x = proch_pos_x;
            }

            // Au cazou, on revient sur la case depart
            if (!moved && in_screen_lim(p->last_pos_y, p->last_pos_x) && zone[p->last_pos_y][p->last_pos_x] == ' ') {
                p->pos_y = p->last_pos_y;
                p->pos_x = p->last_pos_x;
                moved = 1;
            }

            if (moved) {
                p->last_pos_y = p->pos_y;   // évite coût O2 au prochain tick
                p->last_pos_x = p->pos_x;
                info = "Vous avez heurte une paroi, -5 hp";
            } else {
                info = "Vous avez heurte une paroi, -5 hp";
            }

            break;
        }

        case 'E':
            printf("Declancher fonction combat\n");
            info = "Combat";
            screen_status = 1;
            break;

        default:
            info = "Rien par ici";
            break;
        }
    }
}