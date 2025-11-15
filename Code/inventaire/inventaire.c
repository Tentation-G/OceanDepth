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
    {1, ITEM_TYPE_WEAPON, "Harpon Rouille",       "Commun",      5, 10, 2, 0, 0, EFFECT_NONE, 0, 1},
    {2, ITEM_TYPE_WEAPON, "Couteau de Poche",     "Commun",     12, 18, 1, 0, 0, EFFECT_NONE, 0, 1},
    {3, ITEM_TYPE_WEAPON, "Arbalete Marine",      "Marin",      22, 38, 3, 0, 0, EFFECT_NONE, 0, 1},
    {4, ITEM_TYPE_WEAPON, "Harpon Titane",        "Marin",      25, 35, 2, 0, 0, EFFECT_NONE, 0, 1},
    {5, ITEM_TYPE_WEAPON, "Harpon Electrique",    "Oceanique",  28, 42, 4, 0, 0, EFFECT_NONE, 0, 1},
    {6, ITEM_TYPE_WEAPON, "Trident Ancien",       "Abyssal",    30, 45, 3, 0, 0, EFFECT_NONE, 0, 1},
    {7, ITEM_TYPE_WEAPON, "Lance Sonique",        "Abyssal",    35, 50, 5, 0, 0, EFFECT_NONE, 0, 1},

    // combinaisons
    {100, ITEM_TYPE_SUIT, "Neoprene Basic",       "Commun",      0, 0, 0,  5, 1, EFFECT_NONE, 0, 1},
    {101, ITEM_TYPE_SUIT, "Combinaison pro",      "Marin",       0, 0, 0, 15, 1, EFFECT_NONE, 0, 1},
    {102, ITEM_TYPE_SUIT, "Combinaison Titan",    "Oceanique",   0, 0, 0, 25, 1, EFFECT_NONE, 0, 1},
    {103, ITEM_TYPE_SUIT, "Armure Abyssale",      "Abyssal",     0, 0, 0, 30, 2, EFFECT_NONE, 0, 1},
    {104, ITEM_TYPE_SUIT, "Scaphandr Antique",    "Abyssal",     0, 0, 0, 20, 1, EFFECT_NONE, 0, 1},
    {105, ITEM_TYPE_SUIT, "Exosuit Hadès",        "Mythique",    0, 0, 0, 40, 2, EFFECT_NONE, 0, 1},
    {106, ITEM_TYPE_SUIT, "Spectrale",            "Légendaire",  0, 0, 0, 35, 1, EFFECT_NONE, 0, 1},

    // Consommable
    {200, ITEM_TYPE_CONSUMABLE, "Capsule O2",      "+50 O2",        0, 0, 0, 0, 0, EFFECT_RESTORE_O2,     50, 5},
    {201, ITEM_TYPE_CONSUMABLE, "Trousse Soin",    "+25 PV",        0, 0, 0, 0, 0, EFFECT_HEAL_HP,        25, 5},
    {202, ITEM_TYPE_CONSUMABLE, "Stimulant",       "-20 Fatigue",   0, 0, 0, 0, 0, EFFECT_REDUCE_FATIGUE, 20, 5},
    {203, ITEM_TYPE_CONSUMABLE, "Antidote",        "Guerit poison", 0, 0, 0, 0, 0, EFFECT_CURE_POISON,     0, 5},
    {204, ITEM_TYPE_CONSUMABLE, "Mega Capsule O2", "+100 O2",       0, 0, 0, 0, 0, EFFECT_RESTORE_O2,    100, 3},
    {205, ITEM_TYPE_CONSUMABLE, "Kit Medical",     "+50 PV",        0, 0, 0, 0, 0, EFFECT_HEAL_HP,        50, 3},
    {206, ITEM_TYPE_CONSUMABLE, "Super Stimulant", "-40 Fatigue",   0, 0, 0, 0, 0, EFFECT_REDUCE_FATIGUE, 40, 3},
    

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
    
    
    // Essayer d'empiler sur un slot existant
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
    // Trouver un slot vide
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
int generer_coffre(int profondeur)
{
    int choix =0;
    switch (profondeur)
    {
    case 1:{
        choix = rand() % 2;
        break;
    }
    case 2:{
        choix = rand() % 3;
        break;
    }
    case 3:{
        choix = 1+ rand() % 4;
        break;
    }
    case 4:{
        choix = 3 + rand() % 3;
        break;
    }
    case 5:{
        choix = 4 +rand() % 2;
        break;
    }
    default:
        break;
    }
    
    // 50% arme, 50% combinaison
    int type = rand() % 2;

    // index des armes et combinaison
    int armes[] = {1, 3, 4, 5, 6, 7};
    int suits[] = {100, 101, 102, 103, 104, 105};
    

    if (type == 0)  // Arme
    {
        return armes[choix];
    }
    else  // Combinaison
    {
        return suits[choix];
    }
}
