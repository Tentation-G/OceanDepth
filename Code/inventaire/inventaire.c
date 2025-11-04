#include "inventaire.h"
#include "../globals/globals.h"
#include <stddef.h> // Pour NULL
#include "../player/player.h"
#include "../input/input.h"


ItemTemplate g_item_database[] = {
    // ID 0 = Objet Vide
    {0, ITEM_TYPE_NONE, "Vide", "", 0, 0, 0, 0, 0, EFFECT_NONE, 0, 0},
    
    
    {1, ITEM_TYPE_WEAPON, "Harpon Rouille", "ATK: 12-18", 12, 18, 2, 0, 0, EFFECT_NONE, 0, 1},
    {2, ITEM_TYPE_WEAPON, "Couteau de P.", "ATK: 8-14", 8, 14, 1, 0, 0, EFFECT_NONE, 0, 1},

    
    {100, ITEM_TYPE_SUIT, "Neoprene Basic", "DEF: +5", 0, 0, 0, 5, 1, EFFECT_NONE, 0, 1},

    
    {200, ITEM_TYPE_CONSUMABLE, "Capsule O2", "+50 O2", 0, 0, 0, 0, 0, EFFECT_RESTORE_O2, 50, 5},
    {201, ITEM_TYPE_CONSUMABLE, "Trousse Soin", "+25 PV", 0, 0, 0, 0, 0, EFFECT_HEAL_HP, 25, 5},
    {202, ITEM_TYPE_CONSUMABLE, "Stimulant", "-20 Fatigue", 0, 0, 0, 0, 0, EFFECT_REDUCE_FATIGUE, 20, 5},
    {203, ITEM_TYPE_CONSUMABLE, "Antidote", "Guerit poison", 0, 0, 0, 0, 0, EFFECT_CURE_POISON, 0, 5}
};

// taille de bd
int g_item_database_size = sizeof(g_item_database) / sizeof(g_item_database[0]);

// Trouve un objet par son ID
ItemTemplate* get_item_template(int item_id) {
    for (int i = 0; i < g_item_database_size; i++) {
        if (g_item_database[i].id == item_id) {
            return &g_item_database[i];
        }
    }
    return &g_item_database[0]; // Retourne "Vide" si non trouvé
}

// Initialise l'inventaire du joueur au début du jeu
void init_player_inventory(Plongeur *p) {
    // Équiper les objets de départ
    p->equip_weapon.item_id = 1; // Harpon Rouillé
    p->equip_weapon.quantite = 1;
    p->equip_suit.item_id = 100; // Néoprène Basic
    p->equip_suit.quantite = 1;

    // Remplir le sac à dos
    p->inventaire[0].item_id = 200; // 3 Capsules O2
    p->inventaire[0].quantite = 3;
    p->inventaire[1].item_id = 201; // 2 Trousses de Soin
    p->inventaire[1].quantite = 2;
    p->inventaire[2].item_id = 202; // 1 Stimulant
    p->inventaire[2].quantite = 1;
    
    // Initialiser le reste des slots comme "Vide"
    for (int i = 3; i < INVENTORY_SIZE; i++) {
        p->inventaire[i].item_id = 0;
        p->inventaire[i].quantite = 0;
    }
}


void ajouter_item(Plongeur *p, int item_id, int quantite) {
    ItemTemplate* item = get_item_template(item_id);
    
    // 1. Essayer d'empiler sur un slot existant
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventaire[i].item_id == item_id) {
            if (p->inventaire[i].quantite + quantite <= item->max_stack) {
                p->inventaire[i].quantite += quantite;
                return;
            }
        }
    }
    // 2. Trouver un slot vide
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventaire[i].item_id == 0) {
            p->inventaire[i].item_id = item_id;
            p->inventaire[i].quantite = quantite;
            return; 
        }
    }
    
}


// Logique inventaire
void gerer_inventaire(char cmd, Plongeur *plongeur){
    if(cmd == 'Q' || cmd == 'q'){
        screen_status = previous_screen_status;
        info = "";
    }
    else if(cmd == '1'){
        int slot = prompt_for_inventory_slot("Utiliser quel objet ?");
        player_use_item(plongeur, slot);
    }
    else if (cmd == '2'){
        int slot = prompt_for_inventory_slot("Equiper quel objet ?");
        player_equip_item(plongeur, slot);
    }
}