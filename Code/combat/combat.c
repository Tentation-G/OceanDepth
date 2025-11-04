#include <string.h>
#include "combat.h"
#include "../globals/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "combat.h"
#include "../globals/globals.h"
#include "../input/input.h"


#define FATIGUE_MAX 100
#define COUT_ATT_LEGERE 10
#define COUT_ATT_LOURDE 20
#define COUT_ATT_COMPETENCE 30
#define FATIGUE_REPOS 30
#define LIMITE_FATIGUE 90

// un peu foireux, mais jolie et temporaire (ca risque de rester)
// draw en ascii la silouhete du plongeur de dos
void ajout_joueur_combat_screen(char** screen){
    char remp = '@';
    for(int i = 1; i < 21; i++){ screen[18][i] = remp;}
    for(int i = 3; i < 20; i++){ screen[17][i] = remp;}
    for(int i = 5; i < 19; i++){ screen[16][i] = remp;}
    for(int i = 7; i < 17; i++){ screen[15][i] = remp;}
    for(int i = 9; i < 15; i++){ screen[14][i] = remp;}
    for(int i = 9; i < 16; i++){ screen[13][i] = remp;}
    for(int i = 8; i < 18; i++){ screen[12][i] = remp;}
    for(int i = 8; i < 19; i++){ screen[11][i] = remp;}
    for(int i = 7; i < 19; i++){ screen[10][i] = remp;}
    for(int i = 7; i < 18; i++){ screen[9][i]  = remp;}
    for(int i = 8; i < 17; i++){ screen[8][i]  = remp;}
    for(int i = 10; i < 16; i++){ screen[7][i] = remp;}
}

// pour l'instant ça place le nom avec [id] de la creature
// (fonctionne que pour une, à mettre à jour pour le 1 v n)
void ajout_creature_combat_screen(char** screen, CreatureMarine *c) {
    int taille_nom = (int)strlen(c->nom);
    int indice_depart = largeur - 8 - taille_nom;
    if (indice_depart < 0) indice_depart = 0;

    screen[0][indice_depart] = '[';
    screen[0][indice_depart+1] = (char)('0' + c->id);
    screen[0][indice_depart+2] = ']';

    int pos = indice_depart + 3;
    for (int k = 0; k < taille_nom && (pos + k) < largeur; ++k) {
        screen[0][pos + k] = c->nom[k];
    }
}

// Affiche les créatures avec des barres de vie
void ajout_creatures_combat_screen(char** screen, CreatureMarine *creatures, int nbr_creatures) {
    int start_col = largeur - 20;
    int ligne = 0;
    
    for (int i = 0; i < nbr_creatures && ligne < hauteur; i++) {
        if (creatures[i].est_vivant) {
            CreatureMarine *c = &creatures[i];
            
            // Nom
            int pos = start_col;
            for (int k = 0; c->nom[k] != '\0' && pos < largeur - 10; k++) {
                screen[ligne][pos] = c->nom[k];
                pos++;
            }
            
            // " - PV:"
            screen[ligne][pos] = ' '; pos++;
            screen[ligne][pos] = '-'; pos++;
            screen[ligne][pos] = ' '; pos++;
            screen[ligne][pos] = 'P'; pos++;
            screen[ligne][pos] = 'V'; pos++;
            screen[ligne][pos] = ':'; pos++;
            
            // Points de vie
            int pv = c->points_de_vie_actuels;
            if (pv >= 100) {
                screen[ligne][pos] = (pv / 100) + '0'; pos++;
                pv %= 100;
            }
            if (pv >= 10) {
                screen[ligne][pos] = (pv / 10) + '0'; pos++;
                pv %= 10;
            }
            screen[ligne][pos] = pv + '0';
            
            ligne++;
        }
    }
}

// Fonction pour les calculs des degats
int degats_infliges(int attaque_min, int attaque_max, int defense, char effet_mob[])
{

    int chance_effet = rand() % 100; // 0 a 99
    int effet_active = 0;

    if (chance_effet < 50)
    {
        effet_active = 1;
    }

    int base = attaque_min + rand() % (attaque_max - attaque_min + 1);

    if (strcmp(effet_mob, "Carapace") == 0 && effet_active == 1)
    { // Réduit tous les dégâts subis de 20%
        base = base - (int)(base * .2);
        printf("EFFET: CARAPACE\n");
        printf("Reduit tous les degats subis de 20 \n");
    }
    else if (strcmp(effet_mob, "Charge") == 0 && effet_active == 1)
    {
        printf("EFFET: Charge perforante\n");
        printf("Ignore 2 points de defense\n");
        defense = defense - 2;
        if (defense < 0)
            defense = 0;
    }

    int degats = base - defense;
    if (degats < 1)
    {
        degats = 1;
    }
    return degats;
}

// Fonction pour l'attaque du joueur sur creature
void attaquer_creature(Plongeur *p, CreatureMarine *c, int type)
{

    // type = 4 (attaque competence)

    int degats;
    int cout_fatigue;

    if (type == 1)
    {
        cout_fatigue = COUT_ATT_LEGERE;
    }
    else if (type == 2)
    {
        cout_fatigue = COUT_ATT_LOURDE;
    }
    else if (type == 4)
    {
        cout_fatigue = COUT_ATT_COMPETENCE;
    }

    if (type == 1)
    {                                                                   // attaque légère
        degats = degats_infliges(20, 30, c->defense, c->effet_special); // 8, 14
        printf("Vous effectuez une attaque legere.\n");
    }
    else if (type == 2)
    {                                                                   // attaque lourde
        degats = degats_infliges(30, 50, c->defense, c->effet_special); // a ala base 15, 25
        printf("Vous effectuez une attaque lourde.\n");
    }
    else
    {
        degats = degats_infliges(40, 60, c->defense, c->effet_special);
        printf("Vous effectuez une attaque competence.\n");
    }

    //===========================================
    // if (strcmp(arm, "gold") == 0)
    // {
    //     printf("degats avant: %d\n", degats);
    //     printf("Plongeur est equiper d'une armure gold\n");
    //     degats += 10;
    //     printf("degats apres: %d\n", degats);
    // }
    // else if (strcmp(arm, "argent") == 0)
    // {
    //     degats += 5;
    // }
    // else if (strcmp(arm, "bronze") == 0)
    // {
    //     degats += 2;
    // }

    p->niveau_fatigue += cout_fatigue; // ajout à chaque tour de la fatigue en fonction du type d'attaque (+10% si attaque legere ou +20% si attaque lourde)

    if (p->niveau_fatigue > FATIGUE_MAX)
    {
        p->niveau_fatigue = FATIGUE_MAX;
    }

    c->points_de_vie_actuels -= degats;
    if (c->points_de_vie_actuels < 0)
    {
        c->points_de_vie_actuels = 0;
    }
    printf("Vous infligez %d degats a %s !\n", degats, c->nom);
}

// Fonction de l'attaque de la créature sur joueur
void attaquer_plongeur(CreatureMarine *c, Plongeur *p)
{
    // {"paralysie", "Charge", "Frenesie", "Etreinte", "Carapace"};
    int degats;
    int chance_effet = rand() % 100; // 0 a 99
    int effet_active = 0;

    if (chance_effet < 50)
    {
        effet_active = 1;
    }

    // Requin : "Frénésie sanguinaire" → +30% dégâts si PV < 50%
    if (strcmp(c->effet_special, "Frenesie") == 0 && effet_active == 1)
    {
        if (c->points_de_vie_actuels < (c->points_de_vie_max / 2))
        {
            printf("EFFET SPECIALE UTILISER: FRENESIE (+30%% degats)\n");
            degats = degats_infliges(c->attaque_minimale, c->attaque_maximale, 0, c->effet_special);
            degats = degats + (int)(degats * .3);
        }
        else
        {
            degats = degats_infliges(c->attaque_minimale, c->attaque_maximale, 0, c->effet_special);
        }
    }
    else
    {
        degats = degats_infliges(c->attaque_minimale, c->attaque_maximale, 0, c->effet_special);
        // if (strcmp(armure, "gold") == 0)
        // {
        //     printf("degats avant: %d\n", degats);
        //     printf("Plongeur est equiper d'une armure gold\n");
        //     degats -= 10;
        //     printf("degats apres: %d\n", degats);
        // }
        // else if (strcmp(armure, "argent") == 0)
        // {
        //     degats -= 5;
        // }
        // else if (strcmp(armure, "bronze") == 0)
        // {
        //     degats -= 2;
        // }
    }

    p->points_de_vie -= degats;
    if (p->points_de_vie < 0)
    {
        p->points_de_vie = 0;
    }

    // attaque subie fait perdre 1-2 oxygene (stress)
    int stress = rand() % 2 + 1;
    p->niveau_oxygene -= stress;
    printf("STRESS: -%d O2\n", stress);

    printf("%s vous attaque et inflige %d degats !\n", c->nom, degats);
}

// Fonction pour calculer la consommation O2
void consommation_o2(Plongeur *p, int type_attack, int profondeur)
{
    int consommation = 0;

    // Base selon type d'attaque
    if (type_attack == 1 || type_attack == 2)
    {
        // Attaque normale : -2 à -4 selon profondeur
        consommation = 2 + (rand() % 3); // 2, 3 ou 4
    }
    else if (type_attack == 4)
    {
        // Compétence spéciale : -5 à -8
        consommation = 5 + (rand() % 4); // 5, 6, 7, 8
    }

    // Plus on descend, plus on consomme d’oxygène
    if (profondeur == 2)
    {
        consommation += 1;
    }
    else if (profondeur == 3)
    {
        consommation += 2;
    }
    else if (profondeur >= 4)
    {
        consommation += 3;
    }

    p->niveau_oxygene -= consommation;
    if (p->niveau_oxygene < 0)
        p->niveau_oxygene = 0;
}

// Fonction pour s'économiser pour recuperer de la fatigue
void seconomiser(Plongeur *p)
{
    printf("\nVous decidez de vous economiser ce tour\n");
    p->niveau_fatigue -= FATIGUE_REPOS; // enlève -30% de fatigue
    if (p->niveau_fatigue < 0)
    {
        p->niveau_fatigue = 0;
    }
    Sleep(1000);
    printf(" Votre fatigue diminue a %d%%\n", p->niveau_fatigue);
}

// Afficher les creatures restants (temporaire)
void creatures_restants(CreatureMarine *creatures, int nbr_mobs)
{
    printf("CREATURES RESTANTS :\n");
    for (int i = 0; i < nbr_mobs; i++)
    {
        if (creatures[i].est_vivant)
        {
            printf("%d - %s (VITESSE: %d) (PV: %d)\n",
                   i + 1,
                   creatures[i].nom,
                   creatures[i].vitesse,
                   creatures[i].points_de_vie_actuels);
        }
    }
    printf("=========================================\n");
}

// LOGIQUE COMBAT GLOBAL
void gerer_tour_combat(Plongeur *p, char cmd, char **screen) {
    extern CreatureMarine *g_creatures_en_combat;
    extern int g_nbr_creatures_en_combat;
    extern int g_creature_tour_index;
    extern char* info;
    
    int choix_action = 0;  // 1=légère, 2=lourde, 3=économiser, 4=compétence
    int tour_joueur_effectue = 0;
    int profondeur = p->map_pos_y;

    
    if (cmd == 'A' || cmd == 'a') choix_action = 1;
    else if (cmd == 'B' || cmd == 'b') choix_action = 2;
    else if (cmd == 'E' || cmd == 'e') choix_action = 3;
    else if (cmd == 'C' || cmd == 'c') choix_action = 4;
    else if (cmd == 'Q' || cmd == 'q') {
        // Fuite
        info = "Vous avez fui le combat.";
        free(g_creatures_en_combat);
        g_creatures_en_combat = NULL;
        g_nbr_creatures_en_combat = 0;

        screen_status = 0;
        return;
    }
    else if (cmd == 'I' || cmd == 'i') {
        screen_status = 3;
        
    }


    // Verification Fatigue
    if (choix_action == 1 && p->niveau_fatigue + COUT_ATT_LEGERE > FATIGUE_MAX) {
        choix_action = 0;
        info = "Trop fatigue pour une attaque legere !";
    }
    if (choix_action == 2 && p->niveau_fatigue + COUT_ATT_LOURDE > FATIGUE_MAX) {
        choix_action = 0;
        info = "Trop fatigue pour une attaque lourde !";
    }
    if (choix_action == 4 && p->niveau_fatigue + COUT_ATT_COMPETENCE > FATIGUE_MAX) {
        choix_action = 0;
        info = "Trop fatigue pour une competence !";
    }
    
    if (p->niveau_fatigue == FATIGUE_MAX && (choix_action == 1 || choix_action == 2 || choix_action == 4)) {
        info = "Vous etes trop fatigue pour attaquer !";
        choix_action = 0;
    }

    // Action Plongeur
    if (choix_action == 1 || choix_action == 2 || choix_action == 4) {
        // Demander la cible
        int target_index = prompt_for_target(g_nbr_creatures_en_combat, g_creatures_en_combat);
        CreatureMarine* cible = &g_creatures_en_combat[target_index];

        attaquer_creature(p, cible, choix_action);
        consommation_o2(p, choix_action, profondeur);
        
        if (cible->points_de_vie_actuels <= 0) {
            cible->est_vivant = 0;
            info = "Vous avez vaincu une creature !";

            // ici pour supprimer la creature de l'affichage (pas encore)
            // g_nbr_creatures_en_combat--;
            // if (g_nbr_creatures_en_combat < 0)
            // {
            //     g_nbr_creatures_en_combat = 0;
            // }
            
        }
        tour_joueur_effectue = 1;

    } else if (choix_action == 3) {
        // S'économiser
        seconomiser(p);
        info = "Vous recuperez de la fatigue.";
        tour_joueur_effectue = 1;
        
    } else {
        if (choix_action != 0) {
            info = "Action invalide.";
        }
    }

    // Verification victoire
    
    int toutes_mortes = 1;
    for (int i = 0; i < g_nbr_creatures_en_combat; i++) {
        if (g_creatures_en_combat[i].est_vivant) {
            toutes_mortes = 0;
            break;
        }
    }
    
    if (toutes_mortes) {
        info = "VICTOIRE ! Toutes les creatures sont vaincues.";
        free(g_creatures_en_combat);
        g_creatures_en_combat = NULL;
        g_nbr_creatures_en_combat = 0;
        
        screen_status = 0;
        return;
        
    }

    // tour des creatures  
    if (tour_joueur_effectue) {
        // Trouver la prochaine creature vivante
        while (g_creatures_en_combat[g_creature_tour_index].est_vivant == 0) {
            g_creature_tour_index++;
            if (g_creature_tour_index >= g_nbr_creatures_en_combat) {
                g_creature_tour_index = 0;
            }
        }

        CreatureMarine* mob_attaquant = &g_creatures_en_combat[g_creature_tour_index];
        
        printf("\n%s attaque!\n", mob_attaquant->nom);
        
        // Gérer les effets spéciaux
        int chance_effet = rand() % 100;
        int effet_active = (chance_effet < 50);

        if (strcmp(mob_attaquant->effet_special, "Etreinte") == 0 && effet_active) {
            printf("EFFET SPECIALE UTILISER: 2 attaques consecutives\n");
            for (int j = 0; j < 2; j++) {
                attaquer_plongeur(mob_attaquant, p);
            }
        } 
        else if (strcmp(mob_attaquant->effet_special, "paralysie") == 0 && effet_active) {
            printf("EFFET SPECIALE : PARALYSIE\n");
            printf("Fatigue reduite de 10\n");
            p->niveau_fatigue -= 10;
            if (p->niveau_fatigue < 0) p->niveau_fatigue = 0;
            attaquer_plongeur(mob_attaquant, p);
        }
        else {
            attaquer_plongeur(mob_attaquant, p);
        }

        // Passer a la créature suivante
        g_creature_tour_index++;
        if (g_creature_tour_index >= g_nbr_creatures_en_combat) {
            g_creature_tour_index = 0;
        }

        // verification defaite
        
        if (p->points_de_vie <= 0) {
            info = "DEFAITE... Vous avez ete vaincu.";
            free(g_creatures_en_combat);
            g_creatures_en_combat = NULL;
            g_nbr_creatures_en_combat = 0;
            
            screen_status = 0;
            return;
        }
    }

}