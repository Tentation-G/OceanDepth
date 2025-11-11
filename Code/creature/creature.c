#include <stdio.h>
#include "../types/types.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

//Le mot-clé static limite la portée de cette variable au fichier courant
static int prochain_id = 1;

CreatureMarine cree_creature(const char *nom) {
    CreatureMarine c;
    c.est_vivant = 1;
    c.id = prochain_id++;

    strcpy(c.nom, nom);

    // On adapte les stats selon le nom
    if (strcmp(nom, "Meduse") == 0) {
        c.points_de_vie_max = rand() % 21 + 20;
        c.attaque_minimale = 8;
        c.attaque_maximale = 15;
        c.vitesse = 6;
        strcpy(c.effet_special, "paralysie");//Réduit les attaques du joueur de 1 au prochain tour
        c.niveaux_danger = 1;
    }
    else if (strcmp(nom, "Poisson-Epee") == 0) {
        c.points_de_vie_max = rand() % 21 + 70;
        c.attaque_minimale = 18;
        c.attaque_maximale = 28;
        c.vitesse = 3;
        strcpy(c.effet_special, "Charge"); //Ignore 2 points de défense
        c.niveaux_danger = 1;
    }
    else if (strcmp(nom, "Requin") == 0) {
        c.points_de_vie_max = rand() % 41 + 60;
        c.attaque_minimale = 15;
        c.attaque_maximale = 25;
        c.vitesse = 5;
        strcpy(c.effet_special, "Frenesie");//+30% dégâts si PV < 50%
        c.niveaux_danger = 2;
    }
    else if (strcmp(nom, "Kraken") == 0) {
        c.points_de_vie_max = rand() % 61 + 120;
        c.attaque_minimale = 25;
        c.attaque_maximale = 40;
        c.vitesse = 2; // 2 a la base
        strcpy(c.effet_special, "Etreinte");//2 attaques consécutives
        c.niveaux_danger = 4;
        
    }
    else if (strcmp(nom, "CrabeGeant") == 0) {
        c.points_de_vie_max = rand() % 41 + 80;
        c.attaque_minimale = 12;
        c.attaque_maximale = 20;
        c.vitesse = 1;
        strcpy(c.effet_special, "Carapace"); //Réduit tous les dégâts subis de 20%
        c.niveaux_danger = 3;
    }

    c.points_de_vie_actuels = c.points_de_vie_max;
    c.defense = 5;

    return c;
}

CreatureMarine *cree_creatures(int Profondeur){

    const char *listes_creatures[] = {"Meduse", "Poisson-Epee", "Requin", "Kraken", "CrabeGeant"};
    int nbr_mobs;
    double dificulte;
    char nom[30];
    
    if (Profondeur == 1){
        // combat 1v1
        nbr_mobs = 1;
        dificulte = 0.1;
    
    }else if(Profondeur == 2){
        // combat 1v2
        nbr_mobs = 2;
        dificulte = 0.2;
    }else if (Profondeur ==3){
        // combat 1v3
        nbr_mobs = 3;
        dificulte = 0.5;

    }else if(Profondeur >= 4){
        nbr_mobs = 4;
        dificulte = 0.9;
    }
    
    CreatureMarine *creatures = malloc(sizeof(CreatureMarine) * (nbr_mobs));

    for (int i = 0; i < nbr_mobs; i++) {
        int x = rand() % 11;
        double r = x*0.1; // 0, 0.1, 0.2, ....,1
        int random_number;

        if (r > dificulte) {
            // mob facile
            random_number = rand() % 2; // 0 or 1
            strcpy(nom, listes_creatures[random_number]);
        } else {
            // mob difficile
            random_number = 2 + rand() % 3; // 2, 3, 4
            strcpy(nom, listes_creatures[random_number]);
        }

        creatures[i] = cree_creature(nom);
    }
    return creatures;
}

void afficher_etat_creature(CreatureMarine c) {
    printf("%s : %d/%d PV\n", c.nom, c.points_de_vie_actuels, c.points_de_vie_max);
}

CreatureMarine *trier_creatures(CreatureMarine *creatures, int nbr_mobs)
{
    for (int i = 0; i < nbr_mobs - 1; i++)
    {
        for (int j = i + 1; j < nbr_mobs; j++)
        {
            if (creatures[j].vitesse > creatures[i].vitesse)
            {
                // On échange les deux éléments
                CreatureMarine temp = creatures[i];
                creatures[i] = creatures[j];
                creatures[j] = temp;
            }
        }
    }

    return creatures;
}

// Fonction pour generation des bosses
CreatureMarine cree_boss(int profondeur) {
    CreatureMarine boss;
    boss.est_vivant = 1;
    boss.id = prochain_id++;
    boss.defense = 4 + profondeur * 3;  // 7 -> 19

    switch (profondeur) {
        case 1:
            strcpy(boss.nom, "CETUS");
            boss.points_de_vie_max = 200 + rand() % 50;  // 200-249 
            boss.attaque_minimale = 18;
            boss.attaque_maximale = 28;
            boss.vitesse = 7;
            strcpy(boss.effet_special, "RAGE");
            break;

        case 2:
            strcpy(boss.nom, "SCYLLA");
            boss.points_de_vie_max = 300 + rand() % 50;   // 300-349
            boss.attaque_minimale = 25;
            boss.attaque_maximale = 40;
            boss.vitesse = 6;
            strcpy(boss.effet_special, "MULTI");
            break;

        case 3:
            strcpy(boss.nom, "JORMUNGAND");
            boss.points_de_vie_max = 400 + rand() % 50;   // 400-449
            boss.attaque_minimale = 35;
            boss.attaque_maximale = 55;
            boss.vitesse = 8;
            strcpy(boss.effet_special, "VENOM");
            break;

        case 4:
            strcpy(boss.nom, "CHARYBDE");
            boss.points_de_vie_max = 500 + rand() % 50;   // 500-549
            boss.attaque_minimale = 45;
            boss.attaque_maximale = 70;
            boss.vitesse = 4;
            boss.defense += 10;
            strcpy(boss.effet_special, "VORTEX");
            break;

        case 5:
            strcpy(boss.nom, "TIAMAT");
            boss.points_de_vie_max = 600 + rand() % 50;   // 600-649
            boss.attaque_minimale = 60;
            boss.attaque_maximale = 95;
            boss.vitesse = 7;
            boss.defense += 15;
            strcpy(boss.effet_special, "CHAOS");
            break;

        default:
            strcpy(boss.nom, "ERROR");
            boss.points_de_vie_max = 100;
            break;
    }

    boss.points_de_vie_actuels = boss.points_de_vie_max;
    return boss;
}