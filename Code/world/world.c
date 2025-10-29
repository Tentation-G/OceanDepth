#include <stdlib.h>
#include "world.h"
#include "../globals/globals.h"

// === murs + fleches
static char arrow_spaces = ' ';
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

void build_fleche_haut(Zone zone){
    zone[0][largeur / 2 + 2] = arrow_spaces; zone[0][largeur / 2 - 2] = arrow_spaces;
    zone[0][largeur / 2 + 1] = arrow_spaces; zone[0][largeur / 2 - 1] = arrow_spaces;
    zone[0][largeur / 2] = '^';              zone[1][largeur / 2]     = arrow_spaces;
    zone[1][largeur / 2 + 1] = arrow_spaces; zone[1][largeur / 2 - 1] = arrow_spaces;
    zone[1][largeur / 2 + 2] = arrow_spaces; zone[1][largeur / 2 - 2] = arrow_spaces;
}
void build_fleche_bas(Zone zone){
    zone[hauteur - 1][largeur / 2 + 2] = arrow_spaces; zone[hauteur - 2][largeur / 2 + 2] = arrow_spaces;
    zone[hauteur - 1][largeur / 2 + 1] = arrow_spaces; zone[hauteur - 2][largeur / 2 + 1] = arrow_spaces;
    zone[hauteur - 1][largeur / 2] = 'v';              zone[hauteur - 2][largeur / 2]     = arrow_spaces;
    zone[hauteur - 1][largeur / 2 - 1] = arrow_spaces; zone[hauteur - 2][largeur / 2 - 1] = arrow_spaces;
    zone[hauteur - 1][largeur / 2 - 2] = arrow_spaces; zone[hauteur - 2][largeur / 2 - 2] = arrow_spaces;
}
void build_fleche_gauche(Zone zone){
    zone[hauteur / 2 - 1][0] = arrow_spaces; zone[hauteur / 2 - 1][1] = arrow_spaces; zone[hauteur / 2 - 1][2] = arrow_spaces;
    zone[hauteur / 2][0] = '<';              zone[hauteur / 2][1]     = arrow_spaces; zone[hauteur / 2][2]     = arrow_spaces;
    zone[hauteur / 2 + 1][0] = arrow_spaces; zone[hauteur / 2 + 1][1] = arrow_spaces; zone[hauteur / 2 + 1][2] = arrow_spaces;
}
void build_fleche_droit(Zone zone){
    zone[hauteur / 2 - 1][largeur - 1] = arrow_spaces; zone[hauteur / 2 - 1][largeur - 2] = arrow_spaces; zone[hauteur / 2 - 1][largeur - 3] = arrow_spaces;
    zone[hauteur / 2][largeur - 1] = '>';              zone[hauteur / 2][largeur - 2]     = arrow_spaces; zone[hauteur / 2][largeur - 3]     = arrow_spaces;
    zone[hauteur / 2 + 1][largeur - 1] = arrow_spaces; zone[hauteur / 2 + 1][largeur - 2] = arrow_spaces; zone[hauteur / 2 + 1][largeur - 3] = arrow_spaces;
}

// === Item sur la map / Rochers / alges / faut que je trouve des trucs
// == Rochers : plusieurs types (de plusieurs formes) a placer sur la (les) map
/*
void build_cailloux(Zone zone, int y, int x, int zone_h, int zone_l){
    //origine cailloux
    int y_org = zone_h;
    int x_org = zone_l;
    // 0 : Origine | C : Coeur | # : fill random {'#',' '}
    // ##C##
    // #COC#
    // ##C##

    //origine
    zone[y_org][x_org] = '#';
    //coeur
    zone[y_org][x_org + 1] = '#';
    zone[y_org][x_org - 1] = '#';
    zone[y_org + 1][x_org] = '#';
    zone[y_org - 1][x_org] = '#';
    //contour (fill random) | G et D de Origine
    zone[y_org][x_org + 2] = fill_wall_rd[rand() % n];;
    zone[y_org][x_org - 2] = fill_wall_rd[rand() % n];;
    // Premiere ligne
    zone[y_org + 1][x_org + 1] = fill_wall_rd[rand() % n];;
    zone[y_org + 1][x_org - 1] = fill_wall_rd[rand() % n];;
    zone[y_org + 1][x_org + 2] = fill_wall_rd[rand() % n];;
    zone[y_org + 1][x_org - 2] = fill_wall_rd[rand() % n];;
    // Derniere ligne
    zone[y_org - 1][x_org + 1] = fill_wall_rd[rand() % n];;
    zone[y_org - 1][x_org - 1] = fill_wall_rd[rand() % n];;
    zone[y_org - 1][x_org + 2] = fill_wall_rd[rand() % n];;
    zone[y_org - 1][x_org - 2] = fill_wall_rd[rand() % n];;
}
*/

void build_cailloux(Zone zone, int y, int x, int zone_h, int zone_l, int taille) {
    // "zone" = sous-grille
    // (y, x) = coordonnées de la sous-grille
    // (zone_h, zone_l) = coordonnées internes dans la sous-grille
    // => placement du centre du rocher ici

    if (taille < 4) taille = 4; // taille minimale

    int hauteur_rocher = taille;
    int largeur_rocher = 2.5 * hauteur_rocher;   // respect du ratio ASCII
    int demi_hauteur = hauteur_rocher / 2;
    int demi_largeur = largeur_rocher / 2;

    int bord_haut = zone_h - demi_hauteur;
    int bord_bas = bord_haut + hauteur_rocher - 1;
    int bord_gauche = zone_l - demi_largeur;
    int bord_droite = bord_gauche + largeur_rocher - 1;

    for (int ligne_actuelle = bord_haut; ligne_actuelle <= bord_bas; ++ligne_actuelle) {
        if (ligne_actuelle < 0 || ligne_actuelle >= hauteur) continue;

        for (int colonne_actuelle = bord_gauche; colonne_actuelle <= bord_droite; ++colonne_actuelle) {
            if (colonne_actuelle < 0 || colonne_actuelle >= largeur) continue;

            int dist_haut    = ligne_actuelle - bord_haut;
            int dist_bas     = bord_bas - ligne_actuelle;
            int dist_gauche  = colonne_actuelle - bord_gauche;
            int dist_droite  = bord_droite - colonne_actuelle;

            int distance_du_bord = dist_haut;
            if (dist_bas < distance_du_bord) distance_du_bord = dist_bas;
            if (dist_gauche < distance_du_bord) distance_du_bord = dist_gauche;
            if (dist_droite < distance_du_bord) distance_du_bord = dist_droite;

            // 2 couches extérieures aléatoires, intérieur plein
            if (distance_du_bord <= 2) {
                zone[ligne_actuelle][colonne_actuelle] = fill_wall_rd[rand() % n];
            } else {
                zone[ligne_actuelle][colonne_actuelle] = '#';
            }
        }
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

//  Return type de la zone passer en zy zx
ZoneType world_get_zone_type(const World *w, int zone_y, int zone_x) {
    if (!w) return ZoneType_UNKNOWN;
    if (zone_y < 0 || zone_y >= w->zone_h || zone_x < 0 || zone_x >= w->zone_l) return ZoneType_UNKNOWN;
    return w->types[zone_y][zone_x];
}
//  Definit le type de la zone passer en param
void world_set_zone_type(World *w, int zone_y, int zone_x, ZoneType type) {
    if (!w) return;
    if (zone_y < 0 || zone_y >= w->zone_h || zone_x < 0 || zone_x >= w->zone_l) return;
    w->types[zone_y][zone_x] = type;
}

void decorate_zone_base_borders(Zone zone, int y, int x, int zone_h, int zone_l, ZoneType type) {

    //ajout monstre zone de depart pour test pour les coco
    if (y == 1 && x == 0){
        zone[10][10] = 'E';
    }

    // Ajoute les murs en bordures
    if (y == zone_h - 1) { // bord bas
        build_mur_bas(zone, y, x, zone_h, zone_l);
    }
    if (x == 0 && y != 0) { // bord gauche (pas [0][0])
        build_mur_gauche(zone, y, x, zone_h, zone_l);
    }
    if (x == zone_l - 1) { // bord droit
        build_mur_droit(zone, y, x, zone_h, zone_l);
    }

    // pas de fleches pour les grottes, gerer par la fn de deco des grottes
    if (y > 0 && type != ZoneType_GROTTE) {
        build_fleche_haut(zone);
    }
    if (y < zone_h - 1 && type != ZoneType_GROTTE) {
        build_fleche_bas(zone);
    }
    if (x > 0 && type != ZoneType_GROTTE) {
        build_fleche_gauche(zone);
    }
    if (x < zone_l - 1 && type != ZoneType_GROTTE) {
        build_fleche_droit(zone);
    }
}

void decorate_zone_typed(Zone zone, int y, int x, int zone_h, int zone_l, ZoneType type) {
    // couche(s) de base (bords + flèches) (pas pour grottes, c'est gerer par la deco grotte)
    decorate_zone_base_borders(zone, y, x, zone_h, zone_l, type);

    // habillage selon le type
    switch (type) {
    case ZoneType_BASE:
        // Deco Base
        break;

    case ZoneType_BATEAU:
        // Deco zone bateau
        break;

    case ZoneType_GROTTE:
        // murs haut + bas + quelques rochers
        build_mur_haut(zone, y, x, zone_h, zone_l);
        build_mur_bas(zone, y, x, zone_h, zone_l);
        build_mur_gauche(zone, y, x, zone_h, zone_l);
        build_mur_droit(zone, y, x, zone_h, zone_l);

        build_fleche_haut(zone);
        break;

    case ZoneType_BOSS:

        break;

    case ZoneType_RECIF:
    default:
        // quelques cailloux random (enft non, faut que je regasse les cailloux)
        //build_cailloux(zone, y, x, hauteur/2, largeur/2, 5);
        break;
    }
}
/*
void decorate_zone(Zone zone, int y, int x, int zone_h, int zone_l) {
    char arrow_spaces = ' ';

    // Ajoute les murs en bordures
    if (y == zone_h - 1) { // bord bas
        build_mur_bas(zone, y, x, zone_h, zone_l);
    }
    if (x == 0 && y != 0) { // bord gauche (pas [0][0])
        build_mur_gauche(zone, y, x, zone_h, zone_l);
    }
    if (x == zone_h - 1) { // bord droit
        build_mur_droit(zone, y, x, zone_h, zone_l);
    }

    // mettre 4 murs pour la zone 2 2 (pour test)
    if( x == 2 && y == 2){
        build_mur_haut(zone, y, x, zone_h, zone_l);
        build_mur_bas(zone, y, x, zone_h, zone_l);
        build_mur_gauche(zone, y, x, zone_h, zone_l);
        build_mur_droit(zone, y, x, zone_h, zone_l);
        //test build cailloux
        build_cailloux(zone, y, x, 10, 40, 6);
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
        zone[hauteur / 2][largeur - 1] = '>';                  zone[hauteur / 2][largeur - 2] = arrow_spaces;         zone[hauteur / 2][largeur - 3] = arrow_spaces;
        zone[hauteur / 2 + 1][largeur - 1] = arrow_spaces;     zone[hauteur / 2 + 1][largeur - 2] = arrow_spaces;     zone[hauteur / 2 + 1][largeur - 3] = arrow_spaces;
    }
}
*/

// Creation quasi divine du monde
World* init_world(int map_h, int map_l) {
    World *w = malloc(sizeof(World));

    w->zone_h = map_h;
    w->zone_l = map_l;

    w->zones = malloc((size_t)map_h * sizeof(Zone*));
    w->visited = malloc((size_t)map_h * sizeof(char*));
    w->types   = malloc((size_t)map_h * sizeof(ZoneType*));

    for (int y = 0; y < map_h; y++) {
        w->zones[y] = malloc((size_t)map_l * sizeof(Zone));
        w->visited[y] = malloc((size_t)map_l * sizeof(char));
        w->types[y]   = malloc((size_t)map_l * sizeof(ZoneType));

        for (int x = 0; x < map_l; x++) {
            w->zones[y][x]   = init_screen();
            w->visited[y][x] = 0;

            // --- Tres tres temporaire, changer ca par un jolie matrice beaucoup sympa a regarder
            ZoneType type = ZoneType_RECIF;
            if (y == 0 && x == 3)             type = ZoneType_BATEAU; // surface bateau
            else if (y == 0 && x == 0)        type = ZoneType_BASE;   // base
            //else if (y == map_h-1 && x == 0) type = ZoneType_BOSS; // coin bas-gauche : boss
            //else if ((y+x) % 3 == 0)          type = ZoneType_GROTTE; // quelques grottes éparses
            else if (y == 1 && x == 1)          type = ZoneType_GROTTE_S_G; // quelques grottes éparses

            w->types[y][x] = type;

            // décor selon type
            decorate_zone_typed(w->zones[y][x], y, x, map_h, map_l, type);
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
                free(w->types[y]);
            }
        }
        free(w->zones);
        free(w->visited);
        free(w->types);
    }
    free(w);
}
