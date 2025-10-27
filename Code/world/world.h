#ifndef WORLD_H
#define WORLD_H

#include "../types/types.h"

World* init_world(int map_h, int map_l);
void   free_world(World *w);

Zone world_current_zone(World *w, const Plongeur *p);

void decorate_zone(Zone zone, int y, int x, int map_h, int map_l);

// murs
void build_mur_haut(Zone zone, int y, int x, int zone_h, int zone_l);
void build_mur_bas(Zone zone, int y, int x, int zone_h, int zone_l);
void build_mur_gauche(Zone zone, int y, int x, int zone_h, int zone_l);
void build_mur_droit(Zone zone, int y, int x, int zone_h, int zone_l);
// Rochers
void build_cailloux(Zone zone, int y, int x, int zone_h, int zone_l, int taille);

// visited
int  zone_already_visited(const World *w, int zone_y, int zone_x);
void marquer_zone_as_visited(World *w, int zone_y, int zone_x);

// écran brut (zone)
char **init_screen();
void   clear_screen_grid(char **screen, char fill_char); // obsolete

#endif // WORLD_H
