#ifndef PLAYER_H
#define PLAYER_H

#include "../types/types.h"

void layer_player(Plongeur *p, Zone zone);

void demander_player_for_coords(char **screen, Plongeur *p, World *w);
void action_apres_deplacement(Plongeur *p, CreatureMarine *c, int y, int x, char **screen, World *w, Zone zone);

int distance_entre_pos(Plongeur *p);
int sign_int(int v);

//inventaire
void player_use_item(Plongeur *p, int slot_index);
void player_equip_item(Plongeur *p, int slot_index);

#endif // PLAYER_H
