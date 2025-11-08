#ifndef OCEANDEPTH_SAVE_H
#define OCEANDEPTH_SAVE_H
#include "../types/types.h"

extern char saveName1[18];
extern char saveName2[18];
extern char saveName3[18];

FILE* open_slot(int slot, char* mode);

void open_names_slot();
void save_names_slot();

void sauvegarder(World *w, Plongeur *p, int slot);
void charger(Plongeur *p, int slot);

#endif // OCEANDEPTH_SAVE_H
