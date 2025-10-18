#include <stdlib.h>
#include "world.h"
#include "../globals/globals.h"

// === murs : choix aléatoires locaux au fichier
static char fill_wall_rd[] = {'#',' '};
static size_t n = sizeof(fill_wall_rd) / sizeof(fill_wall_rd[0]); // rand

// == Pour poser les murs (pour bordures de map et zones) ==
void build_mur_haut(Zone zone, int y, int x, int zone_h, int zone_l) { // Haut
    for (int j = 0; j < largeur; j++) {
        zone[0][j] = '#';
        zone[1][j] = fill_wall_rd[rand() % n];
        // Assurer la continuité des coins
        zone[1][0] = '#';
        zone[1][1] = '#';
        zone[1][2] = '#';
        zone[1][3] = '#';
        zone[1][largeur - 4] = '#';
        zone[1][largeur - 3] = '#';
        zone[1][largeur - 2] = '#';
        zone[1][largeur - 1] = '#';
    }
}
void build_mur_bas(Zone zone, int y, int x, int zone_h, int zone_l) { // Bas
    for (int j = 0; j < largeur; j++) {
        zone[hauteur - 1][j] = '#';
        zone[hauteur - 2][j] = fill_wall_rd[rand() % n];
        // Assurer la continuité des coins
        zone[hauteur - 2][0] = '#';
        zone[hauteur - 2][1] = '#';
        zone[hauteur - 2][2] = '#';
        zone[hauteur - 2][3] = '#';
        zone[hauteur - 2][largeur - 4] = '#';
        zone[hauteur - 2][largeur - 3] = '#';
        zone[hauteur - 2][largeur - 2] = '#';
        zone[hauteur - 2][largeur - 1] = '#';
    }
}
void build_mur_gauche(Zone zone, int y, int x, int zone_h, int zone_l) { // gauche
    for (int i = 0; i < hauteur; i++){
        zone[i][0] = '#';
        zone[i][1] = '#';
        zone[i][2] = fill_wall_rd[rand() % n];
        // Assurer la continuité des coins
        zone[0][2] = '#';
        zone[1][2] = '#';
        zone[hauteur - 2][2] = '#';
        zone[hauteur - 1][2] = '#';
    }
}
void build_mur_droit(Zone zone, int y, int x, int zone_h, int zone_l) { // droit
    for (int i = 0; i < hauteur; i++){
        zone[i][largeur - 1] = '#';
        zone[i][largeur - 2] = '#';
        zone[i][largeur - 3] = fill_wall_rd[rand() % n];
        // Assurer la continuité des coins
        zone[0][largeur - 3] = '#';
        zone[1][largeur - 3] = '#';
        zone[hauteur - 2][largeur - 3] = '#';
        zone[hauteur - 1][largeur - 3] = '#';
    }
}

char** init_screen(){
    // Allocation dynamique du tab d'ecran — rempli de ' '
    char **screen = malloc(hauteur * sizeof(char*));
    for (int i = 0; i < hauteur; i++){
        screen[i] = malloc((largeur) * sizeof(char));
        for (int j = 0; j < largeur; j++) {
            screen[i][j] = fill;
        }
    }
    return screen;
}

void clear_screen_grid(char **screen, char fill_char){
    for (int i = 0; i < hauteur; i++)
        for (int j = 0; j < largeur; j++)
            screen[i][j] = fill_char;
}

// return null si pas de zone (ex : si pas de sortie dans une dir)
Zone world_current_zone(World *w, const Plongeur *p) {
    if (!w) return NULL;
    // si hors limites
    if (p->map_pos_y < 0 || p->map_pos_y >= w->zone_h) return NULL;
    if (p->map_pos_x < 0 || p->map_pos_x >= w->zone_l) return NULL;
    return w->zones[p->map_pos_y][p->map_pos_x];
}

//  Vérifie si zone deja visitée
int zone_already_visited(const World *w, int zone_y, int zone_x) {
    return w->visited[zone_y][zone_x] != 0;
}

//  Marque une zone visitée
void marquer_zone_as_visited(World *w, int zone_y, int zone_x) {
    w->visited[zone_y][zone_x] = 1;
}

void decorate_zone(Zone zone, int y, int x, int zone_h, int zone_l) {
    char arrow_spaces = ' ';

    // Ajoute les murs en bordures
    if (y == zone_h - 1) { // bord bas
        build_mur_bas(zone, y, x, zone_h, zone_l);
    }
    if (x == 0 && y != 0) { // bord gauche (pas [0][0])
        build_mur_gauche(zone, y, x, zone_h, zone_l);
    }
    if (x == zone_h - 1) { // bord droit (NB: ton code utilisait zone_h - 1 ici)
        build_mur_droit(zone, y, x, zone_h, zone_l);
    }

    if( x == 2 && y == 2){
        build_mur_haut(zone, y, x, zone_h, zone_l);
        build_mur_bas(zone, y, x, zone_h, zone_l);
        build_mur_gauche(zone, y, x, zone_h, zone_l);
        build_mur_droit(zone, y, x, zone_h, zone_l);
    }

    // Ajoute fleches si pas en bordure ( + vides autour des fleches)
    // Sortie NORD
    if (y > 0) {
        zone[0][largeur / 2 + 2] = arrow_spaces ; zone[0][largeur / 2 - 2] = arrow_spaces;
        zone[0][largeur / 2 + 1] = arrow_spaces ; zone[0][largeur / 2 - 1] = arrow_spaces;
        zone[0][largeur / 2] = '^';               zone[1][largeur / 2] = arrow_spaces;
        zone[1][largeur / 2 + 1] = arrow_spaces ; zone[1][largeur / 2 - 1] = arrow_spaces;
        zone[1][largeur / 2 + 2] = arrow_spaces ; zone[1][largeur / 2 - 2] = arrow_spaces;
    }
    // Sortie SUD
    if (y < zone_h - 1) {
        zone[hauteur - 1][largeur / 2 + 2] = arrow_spaces; zone[hauteur - 2][largeur / 2 + 2] = arrow_spaces;
        zone[hauteur - 1][largeur / 2 + 1] = arrow_spaces; zone[hauteur - 2][largeur / 2 + 1] = arrow_spaces;
        zone[hauteur - 1][largeur / 2] = 'v';              zone[hauteur - 2][largeur / 2] = arrow_spaces;
        zone[hauteur - 1][largeur / 2 - 1] = arrow_spaces; zone[hauteur - 2][largeur / 2 - 1] = arrow_spaces;
        zone[hauteur - 1][largeur / 2 - 2] = arrow_spaces; zone[hauteur - 2][largeur / 2 - 2] = arrow_spaces;
    }
    // Sortie OUEST
    if (x > 0) {
        zone[hauteur / 2 - 1][0] = arrow_spaces; zone[hauteur / 2 - 1][1] = arrow_spaces; zone[hauteur / 2 - 1][2] = arrow_spaces;
        zone[hauteur / 2][0] = '<';              zone[hauteur / 2][1] = arrow_spaces;     zone[hauteur / 2][2] = arrow_spaces;
        zone[hauteur / 2 + 1][0] = arrow_spaces; zone[hauteur / 2 + 1][1] = arrow_spaces; zone[hauteur / 2 + 1][2] = arrow_spaces;
    }
    // Sortie EST
    if (x < zone_l - 1) {
        zone[hauteur / 2 - 1][largeur - 1] = arrow_spaces;     zone[hauteur / 2 - 1][largeur - 2] = arrow_spaces;     zone[hauteur / 2 - 1][largeur - 3] = arrow_spaces;
        zone[hauteur / 2][largeur - 1] = '>';                  zone[hauteur / 2][largeur - 2] = arrow_spaces;          zone[hauteur / 2][largeur - 3] = arrow_spaces;
        zone[hauteur / 2 + 1][largeur - 1] = arrow_spaces;     zone[hauteur / 2 + 1][largeur - 2] = arrow_spaces;     zone[hauteur / 2 + 1][largeur - 3] = arrow_spaces;
    }
}

World* init_world(int map_h, int map_l) {
    World *w = malloc(sizeof(World));

    w->zone_h = map_h;
    w->zone_l = map_l;
    w->zones = malloc((size_t)map_h * sizeof(Zone*));
    w->visited = malloc((size_t)map_h * sizeof(char*));

    for (int y = 0; y < map_h; ++y) {
        w->zones[y] = malloc((size_t)map_l * sizeof(Zone));
        w->visited[y] = malloc((size_t)map_l * sizeof(char));
        for (int x = 0; x < map_l; ++x) {
            w->zones[y][x] = init_screen();
            w->visited[y][x] = 0;
            decorate_zone(w->zones[y][x], y, x, map_h, map_l);
        }
    }
    return w;
}

static void free_zone(Zone z) {
    if (!z) return;
    for (int i = 0; i < hauteur; ++i) {
        free(z[i]);
    }
    free(z);
}

void free_world(World *w) {
    if (!w) return;
    if (w->zones) {
        for (int y = 0; y < w->zone_h; ++y) {
            if (w->zones[y]) {
                for (int x = 0; x < w->zone_l; ++x) {
                    if (w->zones[y][x]) {
                        free_zone(w->zones[y][x]);
                    }
                }
                free(w->zones[y]);
                free(w->visited[y]);
            }
        }
        free(w->zones);
        free(w->visited);
    }
    free(w);
}
