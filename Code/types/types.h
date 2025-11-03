#ifndef TYPES_H
#define TYPES_H

typedef enum {
    ZoneType_UNKNOWN = 0,
    ZoneType_VIDE,

    ZoneType_BASE,
    ZoneType_PLAGE,
    ZoneType_BATEAU,
    ZoneType_EPAVE,

    //Definie la dir de sortie/entrée de la grotte
    ZoneType_GROTTE,
    ZoneType_GROTTE_NORD,
    ZoneType_GROTTE_SUD,
    ZoneType_GROTTE_OUEST,
    ZoneType_GROTTE_EST,

    ZoneType_RECIF,
    ZoneType_FORET_ALGUES,
    ZoneType_JARDIN_CORALLIEN,
    ZoneType_BOSS
} ZoneType;

typedef char** Zone;

typedef struct {
    int zone_h;             // nombre de zones en Y
    int zone_l;            // nombre de zones en X
    Zone **zones;         // [zone_h][zone_l]
    char **visited;      // [zone_h][zone_l] : 0/1
    ZoneType **types;   // [zone_h][zone_l] type de chaque zone
} World;

typedef struct {
    int points_de_vie;
    int points_de_vie_max;

    int niveau_oxygene;
    int niveau_oxygene_max;

    int niveau_fatigue;
    int niveau_fatigue_max;

    int defense;
    int vitesse;

    int perles;

    int id_arme_equipe;
    int id_equipement_equipe;

    int pos_y;
    int pos_x;
    int last_pos_y;
    int last_pos_x;
    int map_pos_y;
    int map_pos_x;


} Plongeur;

typedef struct {
    int id;
    char nom[30];

    int points_de_vie_max;
    int points_de_vie_actuels;

    int attaque_minimale;
    int attaque_maximale;

    int defense;
    int vitesse;

    char effet_special[20];
    int est_vivant;
} CreatureMarine;

typedef struct {
    int id;
    int id_type;
    char nom[30];

    int rarete;

    int degats_min;
    int degats_max;
    int consomation;

    int special;
} Arme;

typedef struct {
    int id;
    char nom[30];
    int rarete;
    int defense;
} Equipement;

#endif // TYPES_H
