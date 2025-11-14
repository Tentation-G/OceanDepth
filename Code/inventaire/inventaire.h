#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include "../types/types.h"

// Déclaration de la base de données globale
extern ItemTemplate g_item_database[];
extern int g_item_database_size;

// Fonctions utilitaires
ItemTemplate* get_item_template(int item_id);
void init_player_inventory(Plongeur *p);
void ajouter_item(Plongeur *p, int item_id, int quantite);

void gerer_inventaire(char cmd, Plongeur *plongeur);
int generer_coffre();

#endif