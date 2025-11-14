#ifndef GLOBALS_H
#define GLOBALS_H

#include "../types/types.h"

// Variables globales (def dans le .C)
extern int largeur;
extern int hauteur;

extern int map_largeur;
extern int map_hauteur;

extern char fill;
extern char fill_background[];

extern int active_save;

extern int screen_status;
extern int previous_screen_status;

extern const char *X_AXIS;
extern const char *Y_AXIS;

extern char *info;

extern CreatureMarine *g_creatures_en_combat;
extern int g_nbr_creatures_en_combat;
extern int g_creature_tour_index;

// Variable Competance
extern int cuirasse; // 0 ou 1
extern int souffle;
// extern int elan;
extern int brume;

// TYPE DE COMBAT => 0: creatures , 1: BOSS
extern int type_combat;

// Etat combat
extern int g_victoire;
extern int g_perles_gagnees;
extern int g_perles_perdues;

//id_item_coffre
extern int g_id_item_coffre;

#endif // GLOBALS_H
