#pragma once
#include "../types/types.h"

#define MAP_H 11
#define  MAP_L 10

extern const char* MAP[MAP_H][MAP_L];

ZoneType map_to_type(const char *map_info_type);
int convert_y_to_depth_lvl(int y);
const char* zone_type_to_string(ZoneType type);
const char* zone_type_to_string_four_char(ZoneType type);