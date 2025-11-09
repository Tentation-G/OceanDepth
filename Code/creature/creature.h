#ifndef CREATURES_H
#define CREATURES_H

#include "../types/types.h"
 
CreatureMarine cree_creature(const char *nom);
CreatureMarine *cree_creatures(int Profondeur);
void afficher_etat_creature(CreatureMarine c);
CreatureMarine *trier_creatures(CreatureMarine *creatures, int nbr_mobs);
CreatureMarine cree_boss(int profondeur);

#endif