#include "../globals/globals.h"
#include "../player/player.h"
#include "../input/input.h"
#include "../inventaire/inventaire.h"
#include <stdio.h>





// typedef struct {
//     int item_id;
//     int prix_perles;
//     int stock; 
// } MarchandItem;


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
    {204, 40, 2},   // Carte Trésor
};

// taille de bd
int g_item_marchand_db_size = sizeof(g_item_marchand_db) / sizeof(g_item_marchand_db[0]);

// Afficher le menu du marchand
void afficher_marchand(Plongeur *p) {
    int choix = -1;
    int quantite = 0;

    // Choix de l’objet
    printf("Choisissez un article (ID menu) : \n");
    scanf("%d", &choix);

    if (choix == 0) {
        printf("Vous quittez la boutique.\n");
        return;
    }

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
        printf("Quantité invalide.\n");
        screen_status = 0;
        return;
    }

    // Vérifier le stock
    if (quantite > obj->stock) {
        printf("Le marchand n’a que %d exemplaire(s) en stock.\n", obj->stock);
        return;
    }
    if (quantite > itm->max_stack)
    {
        quantite = itm->max_stack;
    }
    

    // Vérifier les perles du joueur
    int cout_total = obj->prix_perles * quantite;
    if (p->perles < cout_total) {
        printf("Vous n’avez pas assez de perles ! Il faut %d perles.\n", cout_total);
        return;
    }

    // Achat validé
    p->perles -= cout_total;
    obj->stock -= quantite;

    ajouter_item(p, obj->item_id, quantite);

    ItemTemplate *item = get_item_template(obj->item_id);
    printf("✅ Vous avez acheté %d × %s !\n", quantite, item->nom);
    printf("💠 Perles restantes : %d\n", p->perles);
}
