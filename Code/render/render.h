#ifndef RENDER_H
#define RENDER_H

#include "../types/types.h"

void print_screen(char **screen);

void screen_header(World *w,Plongeur *p, char* pv_bar, char* oxy_bar, char* fatigue_bar, char* info);
void screen_main(World *w, Plongeur *p, CreatureMarine *creatures, char** screen);
void screen_footer(World *w, Plongeur *p);

void full_screen(World *w, Plongeur *p, CreatureMarine *creatures, char **screen, char* info);

#endif // RENDER_H
