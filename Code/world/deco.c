#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "deco.h"
#include "world.h"
#include "../globals/globals.h"

// Poissons
void add_poisson_1_1(char** zone, int zone_h, int zone_l){
/*

 <><

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 1) ? 1 : zone_l;
    zone_l = (zone_l > largeur - 2) ? largeur - 2 : zone_l;

    // Origine poisson
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + ligne
    zone[y_org][x_org - 1] = '<';
    zone[y_org][x_org]     = '>';
    zone[y_org][x_org + 1] = '<';

}
void add_poisson_1_2(char** zone, int zone_h, int zone_l){
/*

 ><>

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 1) ? 1 : zone_l;
    zone_l = (zone_l > largeur - 2) ? largeur - 2 : zone_l;

    // Origine poisson
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + ligne
    zone[y_org][x_org - 1] = '>';
    zone[y_org][x_org]     = '<';
    zone[y_org][x_org + 1] = '>';

}
void add_poisson_2_1(char** zone, int zone_h, int zone_l){
/*

 <o(((><

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 3) ? 3 : zone_l;
    zone_l = (zone_l > largeur - 4) ? largeur - 4 : zone_l;

    // Origine poisson
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + ligne
    zone[y_org][x_org - 3] = '<';
    zone[y_org][x_org - 2] = 'o';
    zone[y_org][x_org - 1] = '(';
    zone[y_org][x_org]     = '(';
    zone[y_org][x_org + 1] = '(';
    zone[y_org][x_org + 2] = '>';
    zone[y_org][x_org + 3] = '<';

}
void add_poisson_2_2(char** zone, int zone_h, int zone_l){
/*

 ><)))o>

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 3) ? 3 : zone_l;
    zone_l = (zone_l > largeur - 4) ? largeur - 4 : zone_l;

    // Origine poisson
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + ligne
    zone[y_org][x_org - 3] = '>';
    zone[y_org][x_org - 2] = '<';
    zone[y_org][x_org - 1] = ')';
    zone[y_org][x_org]     = ')';
    zone[y_org][x_org + 1] = ')';
    zone[y_org][x_org + 2] = 'o';
    zone[y_org][x_org + 3] = '>';

}
void add_poisson_3_1(char** zone, int zone_h, int zone_l){
/*

 <*)))==<

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 3) ? 3 : zone_l;
    zone_l = (zone_l > largeur - 5) ? largeur - 5 : zone_l;

    // Origine poisson
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + ligne
    zone[y_org][x_org - 3] = '<';
    zone[y_org][x_org - 2] = '*';
    zone[y_org][x_org - 1] = ')';
    zone[y_org][x_org]     = ')';
    zone[y_org][x_org + 1] = ')';
    zone[y_org][x_org + 2] = '=';
    zone[y_org][x_org + 3] = '=';
    zone[y_org][x_org + 4] = '<';

}
void add_poisson_3_2(char** zone, int zone_h, int zone_l){
/*

 >==(((*>

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 3) ? 3 : zone_l;
    zone_l = (zone_l > largeur - 5) ? largeur - 5 : zone_l;

    // Origine poisson
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + ligne
    zone[y_org][x_org - 3] = '>';
    zone[y_org][x_org - 2] = '=';
    zone[y_org][x_org - 1] = '=';
    zone[y_org][x_org]     = '(';
    zone[y_org][x_org + 1] = '(';
    zone[y_org][x_org + 2] = '(';
    zone[y_org][x_org + 3] = '*';
    zone[y_org][x_org + 4] = '>';

}

// Algues
void add_algue_1(char** zone, int zone_h, int zone_l){
/*

  \\ //
   \Y/
    \\
    //
    ||

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 2) ? 2 : zone_l;
    zone_l = (zone_l > largeur - 3) ? largeur - 3 : zone_l;

    // Origine algue
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + couche 1
    zone[y_org][x_org] = '|';
    zone[y_org][x_org + 1] = '|';
    // Couche 2
    zone[y_org - 1][x_org] = '/';
    zone[y_org - 1][x_org  + 1] = '/';
    // Couche 3
    zone[y_org - 2][x_org] = '\\';
    zone[y_org - 2][x_org + 1] = '\\';
    // Couche 4
    zone[y_org - 3][x_org - 1] = '\\';
    zone[y_org - 3][x_org] = 'Y';
    zone[y_org - 3][x_org + 1] = '/';
    // Couche 5
    zone[y_org - 4][x_org - 2] = '\\';
    zone[y_org - 4][x_org - 1] = '\\';

    zone[y_org - 4][x_org + 2] = '/';
    zone[y_org - 4][x_org + 1] = '/';
}
void add_algue_2(char** zone, int zone_h, int zone_l){
/*

    \\
     \\
     //
    //
    ||
    ||

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 0) ? 0 : zone_l;
    zone_l = (zone_l > largeur - 3) ? largeur - 3 : zone_l;

    // Origine algue
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + couche 1
    zone[y_org][x_org] = '|';
    zone[y_org][x_org + 1] = '|';
    // Couche 2
    zone[y_org - 1][x_org] = '|';
    zone[y_org - 1][x_org  + 1] = '|';
    // Couche 3
    zone[y_org - 2][x_org] = '/';
    zone[y_org - 2][x_org + 1] = '/';
    // Couche 4
    zone[y_org - 3][x_org + 1] = '/';
    zone[y_org - 3][x_org + 2] = '/';
    // Couche 5
    zone[y_org - 4][x_org + 1] = '\\';
    zone[y_org - 4][x_org + 2] = '\\';
    // Couche 6
    zone[y_org - 5][x_org] = '\\';
    zone[y_org - 5][x_org + 1] = '\\';
}
void add_algue_3(char** zone, int zone_h, int zone_l){
/*

     \\
     ||
    //
    ||

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 0) ? 0 : zone_l;
    zone_l = (zone_l > largeur - 3) ? largeur - 3 : zone_l;

    // Origine algue
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + couche 1
    zone[y_org][x_org] = '|';
    zone[y_org][x_org + 1] = '|';
    // Couche 2
    zone[y_org - 1][x_org] = '/';
    zone[y_org - 1][x_org + 1] = '/';
    // Couche 3
    zone[y_org - 2][x_org + 1] = '|';
    zone[y_org - 2][x_org + 2] = '|';
    // Couche 4
    zone[y_org - 3][x_org + 1] = '\\';
    zone[y_org - 3][x_org + 2] = '\\';
}
void add_algue_4(char** zone, int zone_h, int zone_l){
/*

      \\
   \\ //
   || \\
   \\ //
    \Y/
    //

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 1) ? 1 : zone_l;
    zone_l = (zone_l > largeur - 4) ? largeur - 4 : zone_l;

    // Origine algue
    int y_org = zone_h;
    int x_org = zone_l;

    // Origine + Couche 1
    zone[y_org][x_org] = '/';
    zone[y_org][x_org + 1] = '/';
    // Couche 2
    zone[y_org - 1][x_org] = '\\';
    zone[y_org - 1][x_org + 1] = 'Y';
    zone[y_org - 1][x_org + 2] = '/';
    // Couche 3
    zone[y_org - 2][x_org - 1] = '\\';
    zone[y_org - 2][x_org] = '\\';

    zone[y_org - 2][x_org + 2] = '/';
    zone[y_org - 2][x_org + 3] = '/';
    // Couche 4
    zone[y_org - 3][x_org - 1] = '|';
    zone[y_org - 3][x_org] = '|';

    zone[y_org - 3][x_org + 2] = '\\';
    zone[y_org - 3][x_org + 3] = '\\';
    // Couche 4
    zone[y_org - 4][x_org - 1] = '\\';
    zone[y_org - 4][x_org] = '\\';

    zone[y_org - 4][x_org + 2] = '/';
    zone[y_org - 4][x_org + 3] = '/';
    // Couche 5
    zone[y_org - 5][x_org + 2] = '\\';
    zone[y_org - 5][x_org + 3] = '\\';

}
void add_algue_5(char** zone, int zone_h, int zone_l){
/*

    ||
    \\
     ||
     //
     ||
     ||
    //
    \\
     ||
    //
    ||

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 0) ? 0 : zone_l;
    zone_l = (zone_l > largeur - 3) ? largeur - 3 : zone_l;

    // Origine algue
    int y_org = zone_h;
    int x_org = zone_l;

    // origine + couche 1
    zone[y_org][x_org] = '|';
    zone[y_org][x_org + 1] = '|';
    // Couche 2
    zone[y_org - 1][x_org] = '/';
    zone[y_org - 1][x_org + 1] = '/';
    // Couche 3
    zone[y_org - 2][x_org + 1] = '|';
    zone[y_org - 2][x_org + 2] = '|';
    // Couche 4
    zone[y_org - 3][x_org] = '\\';
    zone[y_org - 3][x_org + 1] = '\\';
    // Couche 5
    zone[y_org - 4][x_org] = '/';
    zone[y_org - 4][x_org + 1] = '/';
    // Couche 6
    zone[y_org - 5][x_org + 1] = '|';
    zone[y_org - 5][x_org + 2] = '|';
    // Couche 7
    zone[y_org - 6][x_org + 1] = '|';
    zone[y_org - 6][x_org + 2] = '|';
    // Couche 8
    zone[y_org - 7][x_org + 1] = '/';
    zone[y_org - 7][x_org + 2] = '/';
    // Couche 9
    zone[y_org - 8][x_org + 1] = '|';
    zone[y_org - 8][x_org + 2] = '|';
    // Couche 10
    zone[y_org - 9][x_org] = '\\';
    zone[y_org - 9][x_org + 1] = '\\';
    // Couche 11
    zone[y_org - 10][x_org] = '|';
    zone[y_org - 10][x_org + 1] = '|';
}
void add_algue_6(char** zone, int zone_h, int zone_l){
/*

           \\
           ||
       //  //
       \\ //
        \Y/
        //
    ||  ||
    \\ //
     \Y/
     //
    ||
    ||

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 0) ? 0 : zone_l;
    zone_l = (zone_l > largeur - 9) ? largeur - 9 : zone_l;

    // Origine algue
    int y_org = zone_h;
    int x_org = zone_l;

    // Origine + Couche 1
    zone[y_org][x_org] = '|';
    zone[y_org][x_org + 1] = '|';
    // Couche 2
    zone[y_org - 1][x_org] = '|';
    zone[y_org - 1][x_org + 1] = '|';
    // Couche 3
    zone[y_org - 2][x_org + 1] = '/';
    zone[y_org - 2][x_org + 2] = '/';
    // Couche 4
    zone[y_org - 3][x_org + 1] = '\\';
    zone[y_org - 3][x_org + 2] = 'Y';
    zone[y_org - 3][x_org + 3] = '/';
    // Couche 5
    zone[y_org - 4][x_org] = '\\';
    zone[y_org - 4][x_org + 1] = '\\';

    zone[y_org - 4][x_org + 3] = '/';
    zone[y_org - 4][x_org + 4] = '/';
    // Couche 6
    zone[y_org - 5][x_org] = '|';
    zone[y_org - 5][x_org + 1] = '|';

    zone[y_org - 5][x_org + 4] = '|';
    zone[y_org - 5][x_org + 5] = '|';
    // Couche 7
    zone[y_org - 6][x_org + 4] = '/';
    zone[y_org - 6][x_org + 5] = '/';
    // Couche 8
    zone[y_org - 7][x_org + 4] = '\\';
    zone[y_org - 7][x_org + 5] = 'Y';
    zone[y_org - 7][x_org + 6] = '/';
    // Couche 9
    zone[y_org - 8][x_org + 3] = '\\';
    zone[y_org - 8][x_org + 4] = '\\';

    zone[y_org - 8][x_org + 6] = '/';
    zone[y_org - 8][x_org + 7] = '/';
    // Couche 10
    zone[y_org - 9][x_org + 3] = '/';
    zone[y_org - 9][x_org + 4] = '/';

    zone[y_org - 9][x_org + 7] = '/';
    zone[y_org - 9][x_org + 8] = '/';
    // Courche 11
    zone[y_org - 10][x_org + 7] = '|';
    zone[y_org - 10][x_org + 8] = '|';
    // Couche 12
    zone[y_org - 11][x_org + 7] = '\\';
    zone[y_org - 11][x_org + 8] = '\\';

}

// Corail
void add_corail_1(char** zone, int zone_h, int zone_l){
/*

    \|/
   \ | /
  \  |  /
   \ | /
    \|/
     |

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 3) ? 3 : zone_l;
    zone_l = (zone_l > largeur - 4) ? largeur - 4 : zone_l;

    // Origine corail
    int y_org = zone_h;
    int x_org = zone_l;

    // Origine + Couche 1
    zone[y_org][x_org] = '|';
    // Couche 2
    zone[y_org - 1][x_org - 1] = '\\';
    zone[y_org - 1][x_org]     = '|';
    zone[y_org - 1][x_org + 1] = '/';
    // Couche 3
    zone[y_org - 2][x_org - 2] = '\\';
    zone[y_org - 2][x_org]     = '|';
    zone[y_org - 2][x_org + 2] = '/';
    // Couche 4
    zone[y_org - 3][x_org - 3] = '\\';
    zone[y_org - 3][x_org]     = '|';
    zone[y_org - 3][x_org + 3] = '/';
    // Couche 5
    zone[y_org - 4][x_org - 2] = '\\';
    zone[y_org - 4][x_org]     = '|';
    zone[y_org - 4][x_org + 2] = '/';
    // Couche 6
    zone[y_org - 5][x_org - 1] = '\\';
    zone[y_org - 5][x_org]     = '|';
    zone[y_org - 5][x_org + 1] = '/';
}
void add_corail_2(char** zone, int zone_h, int zone_l){
/*

     (o)
    (ooo)
    (ooo)

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 2) ? 2 : zone_l;
    zone_l = (zone_l > largeur - 3) ? largeur - 3 : zone_l;

    // Origine corail
    int y_org = zone_h;
    int x_org = zone_l;

    // Origine + Couche 1
    zone[y_org][x_org - 2] = '(';
    zone[y_org][x_org - 1] = '0';
    zone[y_org][x_org]     = '0';
    zone[y_org][x_org + 1] = '0';
    zone[y_org][x_org + 2] = ')';
    // Couche 2
    zone[y_org - 1][x_org - 2] = '(';
    zone[y_org - 1][x_org - 1] = '0';
    zone[y_org - 1][x_org]     = '0';
    zone[y_org - 1][x_org + 1] = '0';
    zone[y_org - 1][x_org + 2] = ')';
    // Couche 3
    zone[y_org - 2][x_org - 1] = '(';
    zone[y_org - 2][x_org]     = '0';
    zone[y_org - 2][x_org + 1] = ')';

}
void add_corail_3(char** zone, int zone_h, int zone_l){
/*

    ||o
    \||o/
    ||o
    |||

*/
    // Contenir dans les limites ecran
    zone_l = (zone_l < 1) ? 1 : zone_l;
    zone_l = (zone_l > largeur - 4) ? largeur - 4 : zone_l;

    // Origine corail
    int y_org = zone_h;
    int x_org = zone_l;

    // Origine + Couche 1
    zone[y_org][x_org - 1] = '|';
    zone[y_org][x_org]     = '|';
    zone[y_org][x_org + 1] = '|';
    // Couche 2
    zone[y_org - 1][x_org - 1] = '|';
    zone[y_org - 1][x_org]     = '|';
    zone[y_org - 1][x_org + 1] = 'o';
    // Couche 3
    zone[y_org - 2][x_org - 1] = '\\';
    zone[y_org - 2][x_org]     = '|';
    zone[y_org - 2][x_org + 1] = '|';
    zone[y_org - 2][x_org + 2] = 'o';
    zone[y_org - 2][x_org + 3] = '/';
    // Couche 4
    zone[y_org - 3][x_org - 1] = '|';
    zone[y_org - 3][x_org]     = '|';
    zone[y_org - 3][x_org + 1] = 'o';

}

// Sloop plafond
void build_roof_sloop_left(char** zone) {
    int max_fs_first_layer  = rand() % ((largeur / 2) - 30 + 1) + 30;
    int max_fs_second_layer = rand() % (25 - 20 + 1) + 20;
    int max_fs_third_layer  = rand() % (10 - 5 + 1) + 5;

    // Couche 1 (tout en haut)
    for(int i = 0; i < max_fs_first_layer; i++){
        zone[0][i] = '#';
    }
    // Couche 2
    for(int i = 0; i < max_fs_second_layer; i++){
        zone[1][i] = '#';
    }
    // Couche 3
    for(int i = 0; i < max_fs_third_layer; i++){
        zone[2][i] = '#';
    }
}
void build_roof_sloop_right(char** zone){
    int max_fs_first_layer  = rand() % ((largeur / 2) - 30 + 1) + 30;
    int max_fs_second_layer = rand() % (25 - 20 + 1) + 20;
    int max_fs_third_layer  = rand() % (10 - 5 + 1) + 5;

    // Couche 1 (tout en haut)
    for(int i = 0; i < max_fs_first_layer; i++){
        zone[0][largeur - 1 - i] = '#';
    }
    // Couche 2
    for(int i = 0; i < max_fs_second_layer; i++){
        zone[1][largeur - 1 - i] = '#';
    }
    // Couche 3
    for(int i = 0; i < max_fs_third_layer; i++){
        zone[2][largeur - 1 - i] = '#';
    }
}
void build_roof_sloop(char** zone){
    build_roof_sloop_left(zone);
    build_roof_sloop_right(zone);
}

// Sloop Sol
void build_floor_sloop_left(char** zone){
    //rand() % (max - min + 1) + min;
    int max_fs_first_layer  = rand() % ((largeur / 2 - 2) - 30 + 1) + 30;
    int max_fs_second_layer = rand() % (25 - 20 + 1) + 20;
    int max_fs_third_layer  = rand() % (10 - 5 + 1) + 5;

    // Couche 1
    for(int i = 0; i < max_fs_first_layer; i++){
        zone[hauteur - 1][i] = '#';
    }
    // Couche 2
    for(int i = 0; i < max_fs_second_layer; i++){
        zone[hauteur - 2][i] = '#';
    }
    // Couche 3
    for(int i = 0; i < max_fs_third_layer; i++){
        zone[hauteur - 3][i] = '#';
    }
}
void build_floor_sloop_right(char** zone){
    // rand() % (max - min + 1) + min;
    int max_fs_first_layer  = rand() % ((largeur / 2 - 2) - 30 + 1) + 30;
    int max_fs_second_layer = rand() % (25 - 20 + 1) + 20;
    int max_fs_third_layer  = rand() % (10 - 5 + 1) + 5;

    // Couche 1
    for(int i = 0; i < max_fs_first_layer; i++){
        zone[hauteur - 1][largeur - 1 - i] = '#';
    }
    // Couche 2
    for(int i = 0; i < max_fs_second_layer; i++){
        zone[hauteur - 2][largeur - 1 - i] = '#';
    }
    // Couche 3
    for(int i = 0; i < max_fs_third_layer; i++){
        zone[hauteur - 3][largeur - 1 - i] = '#';
    }
}
void build_floor_sloop(char** zone){
    build_floor_sloop_left(zone);
    build_floor_sloop_right(zone);
    build_fleche_bas(zone);
}

// Gen Flore
void build_decor_on_floor(char** zone, int mode){
    int y_org;
    int x_org;
    // nombre de decor a placer
    int nb_el_decor = rand() % (10 - 3 + 1) + 3;

    for(int i = 0; i < nb_el_decor; i++){
        // trouver un x random
        int x_placement;
        do {
            x_placement = rand() % largeur;
        } while (x_placement >= 32 && x_placement <= 38);

        for(int j = hauteur - 1; j >= hauteur - 4 ; j--){
            if(zone[j][x_placement] != '#'){
                y_org = j;
                x_org = x_placement;

                 // mode 1 = Foret d'algue -> 80% Algues, 20% Corail
                // mode 2 = Recif Coraillien -> 20% Algues, 80% Corail
                int proba_algue = (mode == 1) ? 80 : 20;
                int proba = rand() % 100;

                if (proba < proba_algue) {
                    // Algues
                    int type_algue = (rand() % 6) + 1;
                    switch(type_algue){
                        case 1: add_algue_1(zone, y_org, x_org); break;
                        case 2: add_algue_2(zone, y_org, x_org); break;
                        case 3: add_algue_3(zone, y_org, x_org); break;
                        case 4: add_algue_4(zone, y_org, x_org); break;
                        case 5: add_algue_5(zone, y_org, x_org); break;
                        case 6: add_algue_3(zone, y_org, x_org); break;
                    }
                } else {
                    // Corail
                    int type_corail = (rand() % 3) + 1;
                    switch(type_corail){
                        case 1: add_corail_1(zone, y_org, x_org); break;
                        case 2: add_corail_2(zone, y_org, x_org); break;
                        case 3: add_corail_3(zone, y_org, x_org); break;
                    }
                }

                break;
            }
        }
    }
}
// Gen Faune local AOP
void build_poisson_on_zone(char** zone){
    int y_org;
    int x_org;

    // nombre de petit poisson | 7 - 10
    int nb_petit_poisson = rand() % (10 - 7 + 1) + 7;

    for(int i = 0; i < nb_petit_poisson; i++){
        int y_placement = rand() % hauteur;
        int x_placement = rand() % ((largeur-3) - 3 + 1) + 3;

        if(zone[y_placement][x_placement] != '#'){
            y_org = y_placement;
            x_org = x_placement;

            int type_petit_poisson = (rand() % 2) + 1;
            switch(type_petit_poisson){
                case 1: add_poisson_1_1(zone, y_org, x_org); break;
                case 2: add_poisson_1_2(zone, y_org, x_org); break;
            }
        }
    }

    // nombre de poisson | 1 - 2
    int nb_poisson = (rand() % 2) + 1;

    for(int i = 0; i < nb_poisson; i++){
        int y_placement = rand() % hauteur;
        int x_placement = rand() % ((largeur-3) - 3 + 1) + 3;

        if(zone[y_placement][x_placement] != '#'){
            y_org = y_placement;
            x_org = x_placement;

            int type_poisson = (rand() % 4) + 1;
            switch(type_poisson){
                case 1: add_poisson_2_1(zone, y_org, x_org); break;
                case 2: add_poisson_2_2(zone, y_org, x_org); break;
                case 3: add_poisson_3_1(zone, y_org, x_org); break;
                case 4: add_poisson_3_2(zone, y_org, x_org); break;

            }
        }
    }
}

/*
int main(){
    char** screen = init_screen();
    srand((unsigned)time(NULL));



    build_floor_sloop(screen);
    //build_roof_sloop(screen);

     // mode 1 : algue 80%
    // mod 2 : corail 80%
    build_decor_on_floor(screen, 1);
    build_poisson_on_zone(screen);

    return 0;
}
*/