#ifndef TYPES_H
#define TYPES_H

#define INVENTORY_SIZE 8

typedef enum {
    ZoneType_UNKNOWN = 0,
    ZoneType_VIDE,

    // Zone Chill
    ZoneType_BASE,
    ZoneType_PLAGE,
    ZoneType_BATEAU,
    ZoneType_EPAVE,

     // Definie la dir de
    // sortie/entrée de la grotte
    ZoneType_GROTTE,
    ZoneType_GROTTE_NORD,
    ZoneType_GROTTE_SUD,
    ZoneType_GROTTE_OUEST,
    ZoneType_GROTTE_EST,

    // Zone Pas chill
    ZoneType_RECIF,
    ZoneType_FORET_ALGUES,
    ZoneType_JARDIN_CORALLIEN,
    // Zone pas Chill du tout
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

// Case d'inventaire (Slot)
typedef struct {
    int item_id;  // 0 = Vide, sinon ID de l'ItemTemplate
    int quantite;
} InventorySlot;

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

    int pos_y;
    int pos_x;
    int last_pos_y;
    int last_pos_x;
    int map_pos_y;
    int map_pos_x;

    InventorySlot equip_weapon;
    InventorySlot equip_suit;

    InventorySlot inventaire[INVENTORY_SIZE];


} Plongeur;

typedef struct {
    int id;
    char nom[30];
    int niveaux_danger; // 0: pas danger -> 5:trop dangereux

    int points_de_vie_max;
    int points_de_vie_actuels;

    int attaque_minimale;
    int attaque_maximale;

    int defense;
    int vitesse;

    char effet_special[20];
    int est_vivant;
} CreatureMarine;

// types d'objets
typedef enum {
    ITEM_TYPE_NONE,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_SUIT,
    ITEM_TYPE_CONSUMABLE,
    ITEM_TYPE_MATERIAL // Pour les perles ou autres
} ItemType;

typedef enum {
    EFFECT_NONE,
    EFFECT_HEAL_HP,       // Soigne PV
    EFFECT_RESTORE_O2,    // Restaure Oxygène
    EFFECT_REDUCE_FATIGUE, // Réduit Fatigue
    EFFECT_CURE_POISON    // Guérit poison
} ItemEffect;

// définir un objet
typedef struct {
    int id;
    ItemType type;
    const char* nom;
    const char* description;
    
    // stats pour Armes
    int atk_min;
    int atk_max;
    int o2_cost_atk;

    // stats pour Combinaisons 
    int defense;
    int o2_cost_turn;

    // stats pour Consommables 
    ItemEffect effet;       
    int puissance_effet; 
    int max_stack;       
} ItemTemplate;


typedef struct {
    int item_id;
    int prix_perles;
    int stock; 
} MarchandItem;

#endif // TYPES_H
