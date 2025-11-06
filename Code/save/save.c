#include <stdio.h>
#include <stdlib.h>
#include "save.h"
#include "../types/types.h"

void sauvegarder(Plongeur *p) {
    FILE *f = fopen("../Code/saveGameFiles/save.txt", "w");

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

    fclose(f);
}

void charger(Plongeur *p) {
    FILE *f = fopen("../Code/saveGameFiles/save.txt", "r");

    // On saute les lignes de séparation et on lit précisément dans ton format
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

    fclose(f);
}