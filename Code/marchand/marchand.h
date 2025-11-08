#include "../globals/globals.h"
#include "../player/player.h"
#include "../input/input.h"


// Déclaration de la base de données globale
extern MarchandItem g_item_marchand_db[];
extern int g_item_marchand_db_size;

// Afficher le menu du marchand
void afficher_marchand(Plongeur *p);