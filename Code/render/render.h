#ifndef RENDER_H
#define RENDER_H

#include "../types/types.h"

void print_screen(char **screen);

void screen_header(Plongeur *p, char* pv_bar, char* oxy_bar, char* fatigue_bar, char* info);
void screen_main(World *w, Plongeur *p, CreatureMarine *c, char** screen);
void screen_footer();

void full_screen(World *w, Plongeur *p, CreatureMarine *c, char **screen, char* info);

#endif // RENDER_H
