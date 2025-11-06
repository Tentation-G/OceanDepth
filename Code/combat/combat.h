#ifndef COMBAT_H
#define COMBAT_H

#include "../types/types.h"

void ajout_joueur_combat_screen(char **screen);


// fonction de combat
int degats_infliges(int attaque_min, int attaque_max, int defense, char effet_mob[]);
void attaquer_creature(Plongeur *p, CreatureMarine *c, int type);
void attaquer_plongeur(CreatureMarine *c, Plongeur *p);
void consommation_o2(Plongeur *p, int type_attack, int profondeur);
void seconomiser(Plongeur *p);
void creatures_restants(CreatureMarine *creatures, int nbr_mobs);

void verifier_oxygene_critique(Plongeur *p);


// Tour Combat (tout)
void gerer_tour_combat(Plongeur *p, char cmd, char **screen);

// competences
void appliquer_competence(Plongeur *p, char car);
int choisir_competence();

#endif // COMBAT_H
 