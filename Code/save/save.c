#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"

#include "../globals/globals.h"
#include "../types/types.h"
#include "../input/input.h"

// Save name | default
char saveName1[18] = "Vide";
char saveName2[18] = "Vide";
char saveName3[18] = "Vide";

 // slot => 1 / 2 / 3
// mode => w / r
FILE* open_slot(int slot, char* mode){
    switch (slot) {
    case 1: return fopen("../Code/saveGameFiles/save1.txt", mode);
    case 2: return fopen("../Code/saveGameFiles/save2.txt", mode);
    case 3: return fopen("../Code/saveGameFiles/save3.txt", mode);
    default: return NULL;
    }
}

 // Recup ce qu'il y a dans le fichier saveNames
// Et le met dans les variables
void open_names_slot(){
    FILE *f = fopen("../Code/save/saveNames.txt", "r");
    if (!f) {
         // Si pas de fichier, on en creer un (c'est au cazou)
        // fwd -> file write default
        FILE *fwd = fopen("../Code/save/saveNames.txt", "w");

        fprintf(fwd, "-------------------------\n");
        fprintf(fwd, "Save 1 : Vide\n");
        fprintf(fwd, "-------------------------\n");
        fprintf(fwd, "Save 2 : Vide\n");
        fprintf(fwd, "-------------------------\n");
        fprintf(fwd, "Save 3 : Vide\n");
        fprintf(fwd, "-------------------------\n");
        fclose(fwd);

        return;
    }

    fscanf(f, "-------------------------\n");
    fscanf(f, "Save 1 : %17s\n", saveName1);
    fscanf(f, "-------------------------\n");
    fscanf(f, "Save 2 : %17s\n", saveName2);
    fscanf(f, "-------------------------\n");
    fscanf(f, "Save 3 : %17s\n", saveName3);
    fscanf(f, "-------------------------\n");

    fclose(f);
}

 // Recup ce qu'il y a dans les variables
// Et le met dans le fichier saveNames
void save_names_slot() {

    FILE *f = fopen("../Code/save/saveNames.txt", "w");
    if (!f) return;

    fprintf(f, "-------------------------\n");
    fprintf(f, "Save 1 : %s\n", saveName1);
    fprintf(f, "-------------------------\n");
    fprintf(f, "Save 2 : %s\n", saveName2);
    fprintf(f, "-------------------------\n");
    fprintf(f, "Save 3 : %s\n", saveName3);
    fprintf(f, "-------------------------\n");

    fclose(f);
}

void new_save_slot(int slot, Plongeur *p){

    // Verif si il y a une save sur le slot choisi
    const char *currentName = NULL;
    switch (slot) {
    case 1: currentName = saveName1; break;
    case 2: currentName = saveName2; break;
    case 3: currentName = saveName3; break;
    default: return;
    }

    // Si pas "Vide"
    if (strcmp(currentName, "Vide") != 0) {
        printf("La sauvegarde '%s' sera effacé.\n", currentName);

        // Demande de confirmation
        if (!ask_yes_no("Continuer ?")) {
            printf("Nouvelle sauvegarde annulée.\n");
            return;
        }
    }

    char *name = prompt_for_save_name();

    switch (slot) {
    case 1:
        strncpy(saveName1, name, sizeof(saveName1) - 1);
        saveName1[sizeof(saveName1) - 1] = '\0';
        break;
    case 2:
        strncpy(saveName2, name, sizeof(saveName2) - 1);
        saveName2[sizeof(saveName2) - 1] = '\0';
        break;
    case 3:
        strncpy(saveName3, name, sizeof(saveName3) - 1);
        saveName3[sizeof(saveName3) - 1] = '\0';
        break;
    default:break;
    }

    save_names_slot();


    active_save = slot;

    screen_status = 0;
}

void sauvegarder(World *w, Plongeur *p, int slot) {
    // Select la bonne file
    FILE *f = open_slot(slot, "w");


    fprintf(f, "-------------------------\n");
    fprintf(f, "Numero de Save      : %d\n", slot);
    fprintf(f, "-------------------------\n");
    fprintf(f, "points_de_vie       : %d\n", p->points_de_vie);
    fprintf(f, "points_de_vie_max   : %d\n", p->points_de_vie_max);
    fprintf(f, "-------------------------\n");
    fprintf(f, "niveau_oxygene      : %d\n", p->niveau_oxygene);
    fprintf(f, "niveau_oxygene_max  : %d\n", p->niveau_oxygene_max);
    fprintf(f, "-------------------------\n");
    fprintf(f, "niveau_fatigue      : %d\n", p->niveau_fatigue);
    fprintf(f, "niveau_fatigue_max  : %d\n", p->niveau_fatigue_max);
    fprintf(f, "-------------------------\n");
    fprintf(f, "perles              : %d\n", p->perles);
    fprintf(f, "-------------------------\n");
    fprintf(f, "pos_y               : %d\n", p->pos_y);
    fprintf(f, "pos_x               : %d\n", p->pos_x);
    fprintf(f, "last_pos_y          : %d\n", p->last_pos_y);
    fprintf(f, "last_pos_x          : %d\n", p->last_pos_x);
    fprintf(f, "-------------------------\n");
    fprintf(f, "map_pos_y           : %d\n", p->map_pos_y);
    fprintf(f, "map_pos_x           : %d\n", p->map_pos_x);
    fprintf(f, "-------------------------\n");
    fprintf(f, "equip_weapon        : %d %d\n", p->equip_weapon.item_id, p->equip_weapon.quantite);
    fprintf(f, "equip_suit          : %d %d\n", p->equip_suit.item_id,  p->equip_suit.quantite);
    fprintf(f, "cle                 : %d %d\n",   p->cle_test);
    // Inventaire
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fprintf(f, "slot_%d              : %d %d\n",
                i, p->inventaire[i].item_id, p->inventaire[i].quantite);
    }
    fprintf(f, "-------------------------\n");

    // Map (save d'exploration de la map)
    fprintf(f, "visited:\n");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fputc(w->visited[i][j] ? '1' : '0', f);
        }
        fputc('\n', f);
    }
    fprintf(f, "-------------------------\n");

    fclose(f);
}

void charger(World *w, Plongeur *p, int slot) {

    // Verif si il y a une save sur le slot choisi
    const char *currentName = NULL;
    switch (slot) {
    case 1: currentName = saveName1; break;
    case 2: currentName = saveName2; break;
    case 3: currentName = saveName3; break;
    default: return;
    }

    // Si "Vide"
    if (strcmp(currentName, "Vide") == 0) {
        printf("La sauvegarde %d est vide.\n", slot);
        if (ask_yes_no("Voulez vous creer une nouvelle sauvegarde ?")) {

            new_save_slot(slot, p);

            return;
        }
        // Retour à l'écran de charge de save
        screen_status = 52;
        return;
    }

    // Select la bonne file
    FILE *f = open_slot(slot, "r");

    fscanf(f, "-------------------------\n");
    fscanf(f, "Numero de Save      : %d\n", &slot);
    fscanf(f, "-------------------------\n");
    fscanf(f, "points_de_vie       : %d\n", &p->points_de_vie);
    fscanf(f, "points_de_vie_max   : %d\n", &p->points_de_vie_max);
    fscanf(f, "-------------------------\n");
    fscanf(f, "niveau_oxygene      : %d\n", &p->niveau_oxygene);
    fscanf(f, "niveau_oxygene_max  : %d\n", &p->niveau_oxygene_max);
    fscanf(f, "-------------------------\n");
    fscanf(f, "niveau_fatigue      : %d\n", &p->niveau_fatigue);
    fscanf(f, "niveau_fatigue_max  : %d\n", &p->niveau_fatigue_max);
    fscanf(f, "-------------------------\n");
    fscanf(f, "perles              : %d\n", &p->perles);
    fscanf(f, "-------------------------\n");
    fscanf(f, "pos_y               : %d\n", &p->pos_y);
    fscanf(f, "pos_x               : %d\n", &p->pos_x);
    fscanf(f, "last_pos_y          : %d\n", &p->last_pos_y);
    fscanf(f, "last_pos_x          : %d\n", &p->last_pos_x);
    fscanf(f, "-------------------------\n");
    fscanf(f, "map_pos_y           : %d\n", &p->map_pos_y);
    fscanf(f, "map_pos_x           : %d\n", &p->map_pos_x);
    fscanf(f, "-------------------------\n");
    fscanf(f, "equip_weapon        : %d %d\n",
           &p->equip_weapon.item_id, &p->equip_weapon.quantite);
    fscanf(f, "equip_suit          : %d %d\n",
           &p->equip_suit.item_id,  &p->equip_suit.quantite);
    fscanf(f, "cle                 : %d\n", &p->cle_test);

    char line[16];
    // Slots
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int idx; // slot_idx <- on se cale sur le nb du slot
        fscanf(f, "slot_%d              : %d %d\n",
               &idx, &p->inventaire[i].item_id, &p->inventaire[i].quantite);
    }
    fscanf(f, "-------------------------\n");
    // Map
    fscanf(f, "visited\n");
    // lecture matrice visited
    for (int i = 0; i < 10; i++) {
        fgets(line, sizeof(line), f);
        for (int j = 0; j < 10; j++) {
            w->visited[i][j] = (line[j] == '1');
        }
    }
    fscanf(f, "-------------------------\n");

    fclose(f);
    active_save = slot;
    screen_status = 0;
}