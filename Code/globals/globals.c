#include "globals.h"

int largeur = 69;  // screen main
int hauteur = 19;

int map_largeur = 10;  // nb screen de navigation
int map_hauteur = 10;

// remplissage du fond des grilles
char fill = ' ';
char fill_background[] = {'*','.','~',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' ',' ',' ', ' '};

// 0 = Exploration | 1 = combat | 2 = map | 3 = inventaire | 4 = tresor
int screen_status = 0;
int previous_screen_status = 0;

// Axes pour conversion coords
const char *X_AXIS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!?@#$^&";
const char *Y_AXIS = "0123456789ABCDEFGHI";

// init info bull (str)
char* info = " ";


