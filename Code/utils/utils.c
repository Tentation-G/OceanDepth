#include <string.h>
#include <stdlib.h>
#include "utils.h"

int index_in_axis(char c, const char *axis) {
    const char *p = strchr(axis, c);
    if (p != NULL) { return (int)(p - axis); }
    return -1;
}

char to_upper_ascii(char c) {
    if (c >= 'a' && c <= 'z') return (char)(c - ('a' - 'A'));
    return c;
}

int clamp(int valeur, int max) {
    if (valeur < 0) return 0;
    if (valeur > max) return max;
    return valeur;
}

int in_screen_lim(int y, int x){
    extern int hauteur, largeur;
    return (y >= 0 && y < hauteur && x >= 0 && x < largeur);
}

int convert_to_percent(int val, int val_max) {
    double val_in_pc = ((double)val / val_max) * 100;
    return (int)val_in_pc;
}

char* convert_to_visual_bar(int stat, int stat_max){
    stat = convert_to_percent(stat, stat_max);

    int full_blocks = stat / 10;
    int semi_block = (stat % 10 != 0);
    int empty_blocks = 10 - full_blocks - semi_block;

    char *bar = malloc(10 * 3 + 1);
    if (!bar) return NULL;
    bar[0] = '\0';

    for (int i = 0; i < full_blocks; i++) strcat(bar, "█");
    if (semi_block) strcat(bar, "▓");
    for (int i = 0; i < empty_blocks; i++) strcat(bar, "░");
    return bar;
}

int sign_int(int v)
{
    return (v>0) - (v<0);
}