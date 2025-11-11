#ifndef OCEANDEPTH_DECO_H
#define OCEANDEPTH_DECO_H

// Poisson
void add_poisson_1_1(char** zone, int zone_h, int zone_l);
void add_poisson_1_2(char** zone, int zone_h, int zone_l);

void add_poisson_2_1(char** zone, int zone_h, int zone_l);
void add_poisson_2_2(char** zone, int zone_h, int zone_l);

void add_poisson_3_1(char** zone, int zone_h, int zone_l);
void add_poisson_3_2(char** zone, int zone_h, int zone_l);

// Algues
void add_algue_1(char** zone, int zone_h, int zone_l);
void add_algue_2(char** zone, int zone_h, int zone_l);
void add_algue_3(char** zone, int zone_h, int zone_l);
void add_algue_4(char** zone, int zone_h, int zone_l);
void add_algue_5(char** zone, int zone_h, int zone_l);
void add_algue_6(char** zone, int zone_h, int zone_l);

// Corail
void add_corail_1(char** zone, int zone_h, int zone_l);
void add_corail_2(char** zone, int zone_h, int zone_l);
void add_corail_3(char** zone, int zone_h, int zone_l);

// Sloop plafond
void build_roof_sloop_left(char** zone);
void build_roof_sloop_right(char** zone);
void build_roof_sloop(char** zone);

// Sloop sol
void build_floor_sloop_left(char** zone);
void build_floor_sloop_right(char** zone);
void build_floor_sloop(char** zone);

// Gen flore | mode 1 : Algues | mode 2 : Corail
void build_decor_on_floor(char** zone, int mode);

// Gen poisson
void build_poisson_on_zone(char** zone);

#endif // OCEANDEPTH_DECO_H
