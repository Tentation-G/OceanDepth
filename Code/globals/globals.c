#include "globals.h"
#include <stddef.h> // For NULL

int largeur = 69;  // screen main
int hauteur = 19;

int map_largeur = 10;  // nb screen de navigation
int map_hauteur = 10;

// remplissage du fond des grilles
char fill = ' ';
char fill_background[] = {'*','.','~',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' '};

    // 0 = Exploration
   // 1 = combat
  // 20 = Selection de la carte | 21 = Carte 1 | 22 = Carte 2 | 23 = Carte 3 | 24 = Carte 4
 // 3 = inventaire
// 4 = tresor
int screen_status = 0;
int previous_screen_status = 0;

// Axes pour conversion coords
const char *X_AXIS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!?@#$^&";
const char *Y_AXIS = "0123456789ABCDEFGHI";

// init info bull (str)
char* info = " ";

// Creatures
CreatureMarine *g_creatures_en_combat = NULL;
int g_nbr_creatures_en_combat = 0;
int g_creature_tour_index = 0;


// Variable Competance
int cuirasse = 0; // pas active
int souffle = 0;
int elan = 0;