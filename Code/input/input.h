#ifndef INPUT_H
#define INPUT_H
#include "../world/world.h"

char* saisies_utilisateur_autorise(int status);
char prompt_for_command(World *w, Plongeur *p, int status);
int ask_yes_no(char *question);
int char_in(char *liste, char c);

#endif // INPUT_H
