#include <string.h>
#include "combat.h"
#include "../globals/globals.h"

// un peu foireux, mais jolie et temporaire (ca risque de rester)
// draw en ascii la silouhete du plongeur de dos
void ajout_joueur_combat_screen(char** screen){
    char remp = '@';
    for(int i = 1; i < 21; i++){ screen[18][i] = remp;}
    for(int i = 3; i < 20; i++){ screen[17][i] = remp;}
    for(int i = 5; i < 19; i++){ screen[16][i] = remp;}
    for(int i = 7; i < 17; i++){ screen[15][i] = remp;}
    for(int i = 9; i < 15; i++){ screen[14][i] = remp;}
    for(int i = 9; i < 16; i++){ screen[13][i] = remp;}
    for(int i = 8; i < 18; i++){ screen[12][i] = remp;}
    for(int i = 8; i < 19; i++){ screen[11][i] = remp;}
    for(int i = 7; i < 19; i++){ screen[10][i] = remp;}
    for(int i = 7; i < 18; i++){ screen[9][i]  = remp;}
    for(int i = 8; i < 17; i++){ screen[8][i]  = remp;}
    for(int i = 10; i < 16; i++){ screen[7][i] = remp;}
}

// pour l'instant ca place le nom avec [id] de la creature
// (fonctionne que pour une, a mettre a jour pour le 1 v n)
void ajout_creature_combat_screen(char** screen, CreatureMarine *c) {
    int taille_nom = (int)strlen(c->nom);
    int indice_depart = largeur - 8 - taille_nom;
    if (indice_depart < 0) indice_depart = 0;

    screen[0][indice_depart] = '[';
    screen[0][indice_depart+1] = (char)('0' + c->id);
    screen[0][indice_depart+2] = ']';

    int pos = indice_depart + 3;
    for (int k = 0; k < taille_nom && (pos + k) < largeur; ++k) {
        screen[0][pos + k] = c->nom[k];
    }
}
