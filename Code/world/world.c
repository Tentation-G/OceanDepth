#include <stdlib.h>
#include "world.h"
#include "map_lt.h"
#include "deco.h"
#include "../globals/globals.h"

// === murs + fleches
static char arrow_spaces = ' ';
static char fill_wall_rd[] = {'#',' '};
static size_t n = sizeof(fill_wall_rd) / sizeof(fill_wall_rd[0]); // rand

// Pour poser les murs (pour bordures de map et zones)
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

void build_all_mur(Zone zone, int y, int x, int zone_h, int zone_l){
    build_mur_haut(zone, y, x, zone_h, zone_l);
    build_mur_bas(zone, y, x, zone_h, zone_l);
    build_mur_gauche(zone, y, x, zone_h, zone_l);
    build_mur_droit(zone, y, x, zone_h, zone_l);
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

void spawn_mob_on_zone(Zone zone, int number) {
    int placed = 0;

    while (placed < number) {
        int y = rand() % hauteur;
        int x = rand() % largeur;

        if (zone[y][x] == ' ') {
            zone[y][x] = 'E';
            placed++;
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
    update_boss_pres(w, p->map_pos_y, p->map_pos_x);
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
//  Definit le type de la zone passé en param
void world_set_zone_type(World *w, int zone_y, int zone_x, ZoneType type) {
    if (!w) return; // secu
    if (zone_y < 0 || zone_y >= w->zone_h || zone_x < 0 || zone_x >= w->zone_l) return;
    w->types[zone_y][zone_x] = type;
}

// Si c'est l'un des type de grotte, ca renvoie GROTTE, sinon ca renvoie son zonetype
static ZoneType zone_type_Norm(ZoneType type) {
    switch (type) {
    case ZoneType_GROTTE:
    case ZoneType_GROTTE_NORD:
    case ZoneType_GROTTE_SUD:
    case ZoneType_GROTTE_OUEST:
    case ZoneType_GROTTE_EST:
        return ZoneType_GROTTE;
    default:
        return type;
    }
}

static void update_boss_pres(World *w, int zone_y, int zone_x) {

    if (w->types[zone_y][zone_x] != ZoneType_BOSS) {
        return; // pas une zone boss
    }

    Zone zone = w->zones[zone_y][zone_x];
    int y = hauteur / 2;
    int x = largeur / 2;

    char c = ' '; // par défaut, rien

    switch (zone_y) {
    case 4: // profondeur 1 ?
        if (boss_1 == 0) c = 'B';    // boss présent
        else             c = ' ';   // boss mort/deja mort -> vide
        break;

    case 7: // profondeur 2 ?
        if (boss_2 == 0) c = 'B';
        else             c = ' ';
        break;

    case 9: // ou 10 selon ta map
        if (boss_3 == 0) c = 'B';
        else             c = ' ';
        break;
    default:
        break;
    }

    zone[y][x] = c;
}

void decorate_zone_base_borders(Zone zone, int y, int x, int zone_h, int zone_l, ZoneType type) {
    ZoneType norm = zone_type_Norm(type);
    int is_grotte = (norm == ZoneType_GROTTE);

    // placer un K temporaire pour tester la cle (Profondeur 3)
    if(y == 6 && x == 9){
        zone[hauteur-1][largeur-4] = 'K';
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
    if (!is_grotte) {
        if (y > 0) {
            build_fleche_haut(zone);
        }
        if (y < zone_h - 1) {
            build_fleche_bas(zone);
        }
        if (x > 0) {
            build_fleche_gauche(zone);
        }
        if (x < zone_l - 1) {
            build_fleche_droit(zone);
        }
    }
}

void decorate_zone_typed(Zone zone, int y, int x, int zone_h, int zone_l, ZoneType type) {
      // couche(s) de base (bords + flèches) (pas pour grottes, c'est gerer par la deco grotte)
     //decorate_zone_base_borders(zone, y, x, zone_h, zone_l, type); => mis a la fin pour avoir les
    //                                                                fleches de sortie sur la deco

    // habillage selon le type
    switch (zone_type_Norm(type)) {
        // Zones Chill
        case ZoneType_BASE:
            // Deco Base
            break;
        case ZoneType_BATEAU:
        case ZoneType_SOUS_MARIN:
            zone[hauteur / 2][largeur / 2] = 'M';
            break;
        // Les GROTTES
        case ZoneType_GROTTE:
            //build_all_mur(zone, y, x, zone_h, zone_l);

            switch (type){
                case ZoneType_GROTTE_NORD:
                    build_all_mur(zone, y, x, zone_h, zone_l);
                    build_fleche_haut(zone);

                    break;
                case ZoneType_GROTTE_SUD:
                    build_all_mur(zone, y, x, zone_h, zone_l);
                    build_fleche_bas(zone);

                    break;
                case ZoneType_GROTTE_OUEST:
                    build_all_mur(zone, y, x, zone_h, zone_l);
                    build_fleche_gauche(zone);
                    break;
                case ZoneType_GROTTE_EST:
                    build_all_mur(zone, y, x, zone_h, zone_l);
                    build_fleche_droit(zone);
                    break;
                default:;
                }

            break;

         // Zones pas chill
        // Si vide => 1/2 qu'il y ait deco poisson 1/2 qu'il y ait des mobs
        case ZoneType_VIDE:{
            int probaP = rand() % 100;
            int probaM = rand() % 100;

            if (probaP < 70) build_poisson_on_zone(zone);
            if (probaM < 50) spawn_mob_on_zone(zone, 3);
            }

        case ZoneType_RECIF: // ya pas
            spawn_mob_on_zone(zone, 3);
            break;

        case ZoneType_FORET_ALGUES:
            build_floor_sloop(zone);
            build_decor_on_floor(zone, 1);
            build_poisson_on_zone(zone);
            spawn_mob_on_zone(zone, 3);
            break;

        case ZoneType_JARDIN_CORALLIEN:
            build_floor_sloop(zone);
            build_decor_on_floor(zone, 2);
            build_poisson_on_zone(zone);
            spawn_mob_on_zone(zone, 3);
            break;
        // Zone pas Chill du tout
        case ZoneType_BOSS:
             // Place un boss au milieu
            // Geré dynamiquement par la recup de zone

            break;
        default:
            break;
    }


    decorate_zone_base_borders(zone, y, x, zone_h, zone_l, type);
}

void decorate_connect_zone_to_grotte(const World *w){
    for (int y = 0; y < w->zone_h; y++) {
        for (int x = 0; x < w->zone_l; x++) {

            ZoneType type = w->types[y][x];

            switch (type){
                case ZoneType_GROTTE_NORD:
                    if (y > 0) {
                        Zone z_up = w->zones[y - 1][x];
                        build_mur_bas(z_up, y - 1, x, w->zone_h, w->zone_l);
                        build_fleche_bas(z_up);
                    }
                    if (x > 0) {
                        Zone z_left = w->zones[y][x - 1];
                        build_mur_droit(z_left, y, x - 1, w->zone_h, w->zone_l);
                    }
                    if (y < w->zone_h - 1) {
                        Zone z_down = w->zones[y + 1][x];
                        build_mur_haut(z_down, y + 1, x, w->zone_h, w->zone_l);
                    }
                    if (x < w->zone_l - 1) {
                        Zone z_right = w->zones[y][x + 1];
                        build_mur_gauche(z_right, y, x + 1, w->zone_h, w->zone_l);
                    }
                    break;
                case ZoneType_GROTTE_SUD:
                    if (y < w->zone_h - 1) {
                        Zone z_down = w->zones[y + 1][x];
                        build_mur_haut(z_down, y + 1, x, w->zone_h, w->zone_l);
                        build_fleche_haut(z_down);
                    }
                    if (x > 0) {
                        Zone z_left = w->zones[y][x - 1];
                        build_mur_droit(z_left, y, x - 1, w->zone_h, w->zone_l);
                    }
                    if (y > 0) {
                        Zone z_up = w->zones[y - 1][x];
                        build_mur_bas(z_up, y - 1, x, w->zone_h, w->zone_l);
                    }
                    if (x < w->zone_l - 1) {
                        Zone z_right = w->zones[y][x + 1];
                        build_mur_gauche(z_right, y, x + 1, w->zone_h, w->zone_l);
                    }
                    break;
                case ZoneType_GROTTE_OUEST:
                    if (x > 0) {
                        Zone z_left = w->zones[y][x - 1];
                        build_mur_droit(z_left, y, x - 1, w->zone_h, w->zone_l);
                        build_fleche_droit(z_left);
                    }
                    if (y > 0) {
                        Zone z_up = w->zones[y - 1][x];
                        build_mur_bas(z_up, y - 1, x, w->zone_h, w->zone_l);
                    }
                    if (y < w->zone_h - 1) {
                        Zone z_down = w->zones[y + 1][x];
                        build_mur_haut(z_down, y + 1, x, w->zone_h, w->zone_l);
                    }
                    if (x < w->zone_l - 1) {
                        Zone z_right = w->zones[y][x + 1];
                        build_mur_gauche(z_right, y, x + 1, w->zone_h, w->zone_l);
                    }
                    break;
                case ZoneType_GROTTE_EST:
                    if (x < w->zone_l - 1) {
                        Zone z_right = w->zones[y][x + 1];
                        build_mur_gauche(z_right, y, x + 1, w->zone_h, w->zone_l);
                        build_fleche_gauche(z_right);
                    }
                    if (y > 0) {
                        Zone z_up = w->zones[y - 1][x];
                        build_mur_bas(z_up, y - 1, x, w->zone_h, w->zone_l);
                    }
                    if (y < w->zone_h - 1) {
                        Zone z_down = w->zones[y + 1][x];
                        build_mur_haut(z_down, y + 1, x, w->zone_h, w->zone_l);
                    }
                    if (x > 0) {
                        Zone z_left = w->zones[y][x - 1];
                        build_mur_droit(z_left, y, x - 1, w->zone_h, w->zone_l);
                    }
                    break;

                case ZoneType_FORET_ALGUES:
                case ZoneType_JARDIN_CORALLIEN:
                    if (y < w->zone_h - 1) {
                        Zone z_down = w->zones[y + 1][x];
                        build_roof_sloop(z_down);
                    }
                default: break;
            }
        }
    }
}

// Creation quasi divine du monde
World* init_world(int map_h, int map_l) {
    World *w = malloc(sizeof(World));
    if (!w) return NULL;

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

            ZoneType type = ZoneType_RECIF;
            if (y < MAP_H && x < MAP_L) {
                type = map_to_type(MAP[y][x]);
            }

            w->types[y][x] = type;
            // décor selon type
            decorate_zone_typed(w->zones[y][x], y, x, map_h, map_l, type);
        }
    }
    //Faire une deuxieme passe sur les zones pour connecter les grottes
    decorate_connect_zone_to_grotte(w);

    return w;
}

static void free_zone(Zone z) {
    if (!z) return;
    for (int i = 0; i < hauteur; i++) {
        free(z[i]);
    }
    free(z);
}

void free_world(World *w) {
    if (!w) return;
    if (w->zones) {
        for (int y = 0; y < w->zone_h; y++) {
            if (w->zones[y]) {
                for (int x = 0; x < w->zone_l; x++) {
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
