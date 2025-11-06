#ifndef INPUT_H
#define INPUT_H
#include "../world/world.h"
#include "../types/types.h"

char* saisies_utilisateur_autorise(int status);
char prompt_for_command(World *w, Plongeur *p, int status);
int ask_yes_no(char *question);
int char_in(char *liste, char c);

int prompt_for_target(int nbr_mobs, CreatureMarine *creatures);
int prompt_for_inventory_slot(const char* action_prompt);
int prompt_for_competence(const char* action_prompt);

#endif // INPUT_H
