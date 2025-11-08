#include "../globals/globals.h"
#include "../player/player.h"
#include "../input/input.h"
#include "../inventaire/inventaire.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


bool acheter_item(Plongeur* p, int item_id, int quantite)
{
    ItemTemplate* item = get_item_template(item_id);

    // 1. Essayer d'empiler sur un slot existant
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (p->inventaire[i].item_id == item_id)
        {
            int total = p->inventaire[i].quantite + quantite ;
            if (total <= item->max_stack)
            {
                p->inventaire[i].quantite += quantite;
                
                printf("Vous avez acheter %d de %s", quantite, item->nom);
                info = "Achat Validé";
                return 1;
            }else
            {
                printf("Vous pouvez pas achetez %d de %s (max_stack depasse)\n", quantite, item->nom);
                info = "Achat Non Validé";
                return 0;
            }
            
        }
    }
    // 2. Trouver un slot vide
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (p->inventaire[i].item_id == 0) // trouver un slot vide = (id =>0)
        {   
            if(quantite <= item->max_stack){
                p->inventaire[i].item_id = item_id;
                p->inventaire[i].quantite = quantite;
                printf("Vous avez acheter %d de %s", quantite, item->nom);
                info = "Achat Validé";;
                return 1;
            }else
            {
                printf("Vous pouvez pas achetez %d de %s (max_stack depasse)\n", quantite, item->nom);
                info = "Achat Non Validé";
                return 0;
            }  
        }
    }
}

MarchandItem g_item_marchand_db[] = {
    
    {1, 15, 10},    // Harpon Rouille
    {2, 10, 8},     // Couteau de Plongée
    {3, 40, 4},     // Harpon Titane

    
    {100, 25, 6},   // Néoprène Basique
    {101, 55, 3},   // Combinaison Renforcée

 
    {200, 12, 10},  // Capsule O2
    {201, 18, 10},  // Trousse de Soin
    {202, 22, 5},   // Stimulant Marin
    {203, 25, 5},   // Antidote
    // {204, 40, 2},   // Carte Trésor
};

// taille de bd
int g_item_marchand_db_size = sizeof(g_item_marchand_db) / sizeof(g_item_marchand_db[0]);

// Afficher le menu du marchand
void afficher_marchand(Plongeur *p, char cmd) {
    
    int quantite = 0;  
    if (cmd == 'Q' || cmd == 'q') {
        screen_status = 0;
        info="Retour a l'exploration";
        return; 
    }else if(cmd == 'I' || cmd == 'i'){
        screen_status = 3;
        previous_screen_status = 99;
        info="Inventaire!";
        return;
    }
    int choix = cmd -'0';

    // Vérifier la validité du choix
    int index = choix - 1;
    if (index < 0 || index >= g_item_marchand_db_size) {
        printf("Choix invalide.\n");
        return;
    }

    MarchandItem *obj = &g_item_marchand_db[index];
    ItemTemplate *itm = get_item_template(obj->item_id);

    printf("→ Entrez la quantité voulue : ");
    scanf("%d", &quantite);

    if (quantite <= 0) {
        info = "Quantité invalide.";
        return;
    }

    // Vérifier le stock
    if (quantite > obj->stock) {
        printf("Le marchand n’a que %d exemplaire(s) en stock.\n", obj->stock);
        info="Stock insuffisant!";
        return;
    }

    // Vérifier les perles du joueur
    int cout_total = obj->prix_perles * quantite;
    if (p->perles < cout_total) {
        printf("Vous n’avez pas assez de perles ! Il faut %d perles.\n", cout_total);
        info = "Pas assez de perles";
        return;
    }

    int achat = acheter_item(p, obj->item_id, quantite);
    // Achat validé
    if(achat){
        p->perles -= cout_total;
        obj->stock -= quantite;
        ItemTemplate *item = get_item_template(obj->item_id);
        printf("Vous avez acheté %d × %s !\n", quantite, item->nom);
        printf("Perles restantes : %d\n", p->perles);
    }else{
        printf("Achat non validé\n");
        info="Achat non validé";
    }
    
}
