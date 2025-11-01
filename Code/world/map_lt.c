#include <string.h>
#include "map_lt.h"
#include "../types/types.h"

const char* MAP[MAP_H][MAP_L] = {
    { "BASE"  , "PLAGE" , " "     , " "     , " "     , " "     , " "     , "BATEAU", " "     , " "      },                // Surface  Y[0]
    //----------------------------------------------------------------------------------------------------------------------------------------
    { "GROT_S",  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },              // Depth 1    Y[1]
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },             // Depth 1     Y[2]
    //----------------------------------------------------------------------------------------------------------------------------------------
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , "GROT_E", " "      },           // Depth 2       Y[3]
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },          // Depth 2        Y[4]
    //----------------------------------------------------------------------------------------------------------------------------------------
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },        // Depth 3          Y[5]
    { "GROT_E",  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },       // Depth 3           Y[6]
    //----------------------------------------------------------------------------------------------------------------------------------------
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },     // Depth 4             Y[7]
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },    // Depth 4              Y[8]
    //----------------------------------------------------------------------------------------------------------------------------------------
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      },  // Depth 5                Y[9]
    { " "     ,  " "    , " "     , " "     , " "     , " "     , " "     , " "     , " "     , " "      }  // Depth 5                Y[10]
};

// conversion zone Y en lvl (pour savoir quelle ligne appartient à quel niveau de profondeur :
/*

for(int i = 0; i < 10; i++){
    int depth_lvl = (i+1)/2; // div entiere
    return depth_lvl;
}

*/

ZoneType map_to_type(const char *map_info_type) {
    if (!map_info_type) return ZoneType_UNKNOWN;

    if      (strcmp(map_info_type, "BASE") == 0)    return ZoneType_BASE;
    else if (strcmp(map_info_type, "BATEAU") == 0)  return ZoneType_BATEAU;
    else if (strcmp(map_info_type, "PLAGE") == 0)   return ZoneType_PLAGE;

    else if (strcmp(map_info_type, "GROTTE") == 0)  return ZoneType_GROTTE;
    else if (strcmp(map_info_type, "GROT_N") == 0)  return ZoneType_GROTTE_NORD;
    else if (strcmp(map_info_type, "GROT_S") == 0)  return ZoneType_GROTTE_SUD;
    else if (strcmp(map_info_type, "GROT_O") == 0)  return ZoneType_GROTTE_OUEST;
    else if (strcmp(map_info_type, "GROT_E") == 0)  return ZoneType_GROTTE_EST;

    else if (strcmp(map_info_type, "RECIF") == 0)   return ZoneType_RECIF;
    else if (strcmp(map_info_type, "BOSS") == 0)    return ZoneType_BOSS;

    else if (strcmp(map_info_type, " ") == 0)  return ZoneType_VIDE;


    return ZoneType_UNKNOWN;
}

const char* zone_type_to_string(const ZoneType type) {s
    switch (type) {
    case ZoneType_BASE:       return "Base";
    case ZoneType_PLAGE:      return "Plage";
    case ZoneType_BATEAU:     return "Bateau";

    case ZoneType_GROTTE:     return "Grotte";
    case ZoneType_GROTTE_NORD:     return "Grotte";
    case ZoneType_GROTTE_SUD:     return "Grotte";
    case ZoneType_GROTTE_OUEST:     return "Grotte";
    case ZoneType_GROTTE_EST:     return "Grotte";

    case ZoneType_VIDE:       return "Rien";

    case ZoneType_RECIF:      return "Recif";
    case ZoneType_BOSS:       return "Boss";
    default:                  return "????";
    }
}