#include "inventaire.h"
#include <stddef.h> // Pour NULL
#include "../globals/globals.h"
#include "../input/input.h"
#include "../player/player.h"

// tempo
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



ItemTemplate g_item_database[] = {
    // ID 0 = Objet Vide
    {0, ITEM_TYPE_NONE, "Vide", "", 0, 0, 0, 0, 0, EFFECT_NONE, 0, 0},

    // Armes
    {1, ITEM_TYPE_WEAPON, "Harpon Rouille", "ATK: 12-18", 12, 18, 2, 0, 0, EFFECT_NONE, 0, 1},
    {2, ITEM_TYPE_WEAPON, "Couteau de P.", "ATK: 8-14", 8, 14, 1, 0, 0, EFFECT_NONE, 0, 1},
    {3, ITEM_TYPE_WEAPON, "Harpon Titane", "ATK: 25-35", 25, 35, 2, 0, 0, EFFECT_NONE, 0, 1},
    // Nouvelles Armes
    {4, ITEM_TYPE_WEAPON, "Harpon Electrique", "ATK: 28-42", 28, 42, 4, 0, 0, EFFECT_NONE, 0, 1},
    {5, ITEM_TYPE_WEAPON, "Trident Ancien", "ATK: 30-45", 30, 45, 3, 0, 0, EFFECT_NONE, 0, 1},
    {6, ITEM_TYPE_WEAPON, "Lance Sonique", "ATK: 35-50", 35, 50, 5, 0, 0, EFFECT_NONE, 0, 1},
    {7, ITEM_TYPE_WEAPON, "Arbalete Sous-Marine", "ATK: 22-38", 22, 38, 3, 0, 0, EFFECT_NONE, 0, 1},

    // combinaison
    {100, ITEM_TYPE_SUIT, "Neoprene Basic", "DEF: +5", 0, 0, 0, 5, 1, EFFECT_NONE, 0, 1},
    {101, ITEM_TYPE_SUIT, "Combinaison Ren.", "DEF: +15", 0, 0, 0, 15, 1, EFFECT_NONE, 0, 1},
    // Nouvelles combinison
    {102, ITEM_TYPE_SUIT, "Armure Abyssale", "DEF: +25", 0, 0, 0, 25, 1, EFFECT_NONE, 0, 1},
    {103, ITEM_TYPE_SUIT, "Combinaison Titan", "DEF: +30", 0, 0, 0, 30, 2, EFFECT_NONE, 0, 1},
    {104, ITEM_TYPE_SUIT, "Scaphandre Antique", "DEF: +20", 0, 0, 0, 20, 1, EFFECT_NONE, 0, 1},

    // Consommable
    {200, ITEM_TYPE_CONSUMABLE, "Capsule O2", "+50 O2", 0, 0, 0, 0, 0, EFFECT_RESTORE_O2, 50, 5},
    {201, ITEM_TYPE_CONSUMABLE, "Trousse Soin", "+25 PV", 0, 0, 0, 0, 0, EFFECT_HEAL_HP, 25, 5},
    {202, ITEM_TYPE_CONSUMABLE, "Stimulant", "-20 Fatigue", 0, 0, 0, 0, 0, EFFECT_REDUCE_FATIGUE, 20, 5},
    {203, ITEM_TYPE_CONSUMABLE, "Antidote", "Guerit poison", 0, 0, 0, 0, 0, EFFECT_CURE_POISON, 0, 5},
    // New
    {204, ITEM_TYPE_CONSUMABLE, "Mega Capsule O2", "+100 O2", 0, 0, 0, 0, 0, EFFECT_RESTORE_O2, 100, 3},
    {205, ITEM_TYPE_CONSUMABLE, "Kit Medical Complet", "+50 PV", 0, 0, 0, 0, 0, EFFECT_HEAL_HP, 50, 3},
    {206, ITEM_TYPE_CONSUMABLE, "Super Stimulant", "-40 Fatigue", 0, 0, 0, 0, 0, EFFECT_REDUCE_FATIGUE, 40, 3},
    

};

// taille de bd
int g_item_database_size = sizeof(g_item_database) / sizeof(g_item_database[0]);

// Trouve un objet par son ID
ItemTemplate* get_item_template(int item_id)
{
    for (int i = 0; i < g_item_database_size; i++)
    {
        if (g_item_database[i].id == item_id)
        {
            return &g_item_database[i];
        }
    }
    return &g_item_database[0]; // Retourne "Vide" si non trouvé
}

// Initialise l'inventaire du joueur au début du jeu
void init_player_inventory(Plongeur* p)
{
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

    // Remplir cle
    p->cle = 0;

    // Initialiser le reste des slots comme "Vide"
    for (int i = 3; i < INVENTORY_SIZE; i++)
    {
        p->inventaire[i].item_id = 0;
        p->inventaire[i].quantite = 0;
    }
}


void ajouter_item(Plongeur* p, int item_id, int quantite)
{
    ItemTemplate* item = get_item_template(item_id);

    // verifier si (arme ou equipement) deja possede
    if (item_id == p->equip_weapon.item_id || item_id == p->equip_suit.item_id)
    {
        printf("vous possedez deja cette item\n");
        info="vous possedez deja cette item dans (equipement)!";
        return;
    }

    if (item->type == ITEM_TYPE_WEAPON || item->type == ITEM_TYPE_CONSUMABLE)
    {
         for (int i = 0; i < INVENTORY_SIZE; i++)
            {   
                if (p->inventaire[i].item_id == item_id)
                {   
                    printf("vous possedez deja cette item\n");
                    info="Vous possedez deja cette item dans l'inventaire!";
                    return;
                }
            }
    }
    
    
    // 1. Essayer d'empiler sur un slot existant
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        
        if (p->inventaire[i].item_id == item_id)
        {   
            int total = p->inventaire[i].quantite + quantite;

            if (p->inventaire[i].quantite + quantite <= item->max_stack)
            {
                p->inventaire[i].quantite += quantite;
                return;
            }
        }
    }
    // 2. Trouver un slot vide
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (p->inventaire[i].item_id == 0)
        {
            p->inventaire[i].item_id = item_id;
            p->inventaire[i].quantite = quantite;
            return;
        }
    }
}


// Logique inventaire
void gerer_inventaire(char cmd, Plongeur* plongeur)
{

    printf("Previous screen en combat inventaire: %d", previous_screen_status);
    if (cmd == 'Q' || cmd == 'q')
    {
        screen_status = previous_screen_status;
        if (screen_status == 1)
        {
            info = "Combat!";
        }
        else if (screen_status == 0)
        {
            info = "Exploration!";
        }
        else
        {
            info = "";
        }
    }
    else if (cmd == '1')
    {
        int slot = prompt_for_inventory_slot("Utiliser quel objet ?");
        player_use_item(plongeur, slot);
    }
    else if (cmd == '2')
    {
        int slot = prompt_for_inventory_slot("Equiper quel objet ?");
        player_equip_item(plongeur, slot);
    }
}

// Generer un coffre (retrourner un id)
int generer_coffre()
{
    // 50% arme, 50% combinaison
    int type = rand() % 2;

    // Liste des armes 
    int armes[] = {1, 3, 4, 5, 6, 7};
    int nb_armes = sizeof(armes) / sizeof(armes[0]);

    // Liste des combinaisons =
    int suits[] = {100, 101, 102, 103, 104};
    int nb_suits = sizeof(suits) / sizeof(suits[0]);

    if (type == 0)  // Arme
    {
        int index = rand() % nb_armes;
        return armes[index];
    }
    else  // Combinaison
    {
        int index = rand() % nb_suits;
        return suits[index];
    }
}