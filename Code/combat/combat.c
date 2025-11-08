#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

#define COUT_OX_VAGUE 25
#define COUT_OX_CUIRASSE 20
#define COUT_OX_SOUFLE 10
#define COUT_OX_BRUME 20



// Duree competences
int  duree_cuirasse = 3;
int duree_souffle = 3;
int duree_brume = 2;


// Verification O2 (Alerte ou -PV)
void verifier_oxygene_critique(Plongeur *p) {
    
    if (p->niveau_oxygene < 0) {
        p->niveau_oxygene = 0;
    }
    
    if (p->niveau_oxygene == 0) {
        printf("\n");
        printf("╔════════════════════════════════════════╗\n");
        printf("║    ! VOUS N'AVEZ PLUS D'O2             ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("Vous perdez 5 PV par manque d'oxygene !\n");
        
        p->points_de_vie -= 5;
        if (p->points_de_vie < 0) {
            p->points_de_vie = 0;
        }
        
        info = "-5 PV !";
    }
    
    else if (p->niveau_oxygene <= 10) {
        printf("\n");
        printf("╔════════════════════════════════════════╗\n");
        printf("║       ALERTE OXYGENE CRITIQUE          ║\n");
        printf("║         Il vous reste %2d%% O2 !         ║\n", p->niveau_oxygene);
        printf("╚════════════════════════════════════════╝\n");
        
        info = "ALERTE CRITIQUE : O2 tres bas !";
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
        // A revoir ici (j'ai pas bien compris)
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

    int degats;
    int cout_fatigue;

    if (type == 1) // Attack legere
    {
        cout_fatigue = COUT_ATT_LEGERE;
    }
    else if (type == 2)
    {
        cout_fatigue = COUT_ATT_LOURDE;
    }

    // COMPÉTENCE SOUFFLE
    if (souffle && duree_souffle > 0) {
        
        if (p->niveau_oxygene >= COUT_OX_SOUFLE) {
            printf("Competence active : Souffle maitrise (fatigue /2)\n");
            cout_fatigue = cout_fatigue / 2;
            
            p->niveau_oxygene -= COUT_OX_SOUFLE;
            duree_souffle--;
            
            printf("Souffle consomme %d oxygen\n", COUT_OX_SOUFLE);
            
            if (duree_souffle == 0) {
                souffle = 0;
                printf("FIN Souffle\n");
            }
        }
        else {
            printf("Pas assez d'O2 pour Souffle ! (besoin: %d)\n", COUT_OX_SOUFLE);
            souffle = 0;
            duree_souffle = 0;
        }
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

    // Verification O2 critique
    verifier_oxygene_critique(p);
}

// Fonction de l'attaque de la créature sur joueur
void attaquer_plongeur(CreatureMarine *c, Plongeur *p)
{
    // {"paralysie", "Charge", "Frenesie", "Etreinte", "Carapace"};
    int degats;
    int chance_effet = rand() % 100; // 0 a 99
    int effet_active = 0;

    // COMPÉTENCE BRUME MARINE
    if (brume && duree_brume > 0) {
        if (p->niveau_oxygene >= COUT_OX_BRUME) {
            int chance_esquive = rand() % 100;
            if (chance_esquive < 50) { // 50% de chance d'éviter l'attaque
                printf("Brume marine : vous esquivez l'attaque !\n");
                p->niveau_oxygene -= COUT_OX_BRUME;
                duree_brume--;
                if (duree_brume == 0) {
                    brume = 0;
                    printf("Brume marine s'est desactive.\n");
                }
                return; // On esquive complètement, pas de dégâts
            } else {
                printf("Brume marine a echoue cette fois...\n");
                p->niveau_oxygene -= COUT_OX_BRUME;
                duree_brume--;
                if (duree_brume == 0) {
                    brume = 0;
                    printf("Brume marine s'est dissipee.\n");
                }
            }
        } else {
            printf("Plus assez d'O2 pour Brume marine !\n");
            brume = 0;
            duree_brume = 0;
        }
    }
    

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
    }
    printf("Degats AVANT: %d\n", degats);
    
    // COMPÉTENCE CUIRASSE
    if (cuirasse && duree_cuirasse > 0) {
        // Vérifier O2 AVANT de consommer
        if (p->niveau_oxygene >= COUT_OX_CUIRASSE) {
            printf("Competence active : Cuirasse aquatique (-30%% degats)\n");
            degats = degats - (int)(degats * 0.3);
            
            p->niveau_oxygene -= COUT_OX_CUIRASSE;
            duree_cuirasse--;
            
            printf("Cuirasse Degats Apres: %d\n", degats);
            printf("Cuirasse consomme %d oxygen\n", COUT_OX_CUIRASSE);
            
            if (duree_cuirasse == 0) {
                cuirasse = 0;
                printf("Cuirasse aquatique s'est desactive.\n");
            }
        }
        else {
            printf("Pas assez d'O2 pour Cuirasse ! (besoin: %d)\n", COUT_OX_CUIRASSE);
            // Désactiver la compétence si plus d'O2
            cuirasse = 0;
            duree_cuirasse = 0;
        }
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
    verifier_oxygene_critique(p);
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
    if (p->niveau_oxygene < 0){
        p->niveau_oxygene = 0;
    }
    verifier_oxygene_critique(p);
        
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



void appliquer_competence(Plongeur *p, char car) {
    int choix = car - '0';
    switch (choix) {
        case 1: // Élan marin

            if (duree_brume == 0 && brume == 1) {
                brume = 0;
            }

            if (p->niveau_oxygene < COUT_OX_BRUME) {
                printf("Niveau d'oxygene insuffisant (besoin: %d)\n", COUT_OX_BRUME);
                info = "Pas assez d'O2 !";
            }
            else if (brume == 1) {
                printf("Brume marine est deja active, il reste %d tours\n", duree_brume);
                info = "Brume deja active !";
            }
            else {
                brume = 1;
                duree_brume = 2;
                p->niveau_oxygene -= COUT_OX_BRUME;
                printf("Brume marine activee : 50%% d'esquive pendant 2 tours !\n");
                info = "Brume marine activee !";
            }
            break;

        case 2: 

            // Vérifier si la durée est expirée 
            if (duree_cuirasse == 0 && cuirasse == 1) {
                cuirasse = 0;  // Réinitialiser l'état
            }
            
            if (p->niveau_oxygene < COUT_OX_CUIRASSE) {
                printf("Niveau d'oxygene insuffisant (besoin: %d)\n", COUT_OX_CUIRASSE);
                info = "Pas assez d'O2 !";
            }
            
            else if (cuirasse == 1) {
                printf("Cuirasse est deja active, il reste %d tours\n", duree_cuirasse);
                info = "Cuirasse deja active !";
            }

            else {
                cuirasse = 1;
                p->niveau_oxygene -= COUT_OX_CUIRASSE;
                duree_cuirasse = 3;
                printf("Cuirasse aquatique activee : -30%% degats pendant 3 tours !\n");
                info = "Cuirasse activee !";
            }
            break;

        case 3: // Souffle maîtrisé
            
            if (duree_souffle == 0 && souffle == 1) {
                souffle = 0;  // Réinitialiser l'état
            }
            
            
            if (p->niveau_oxygene < COUT_OX_SOUFLE) {
                printf("Niveau d'oxygene insuffisant (besoin: %d)\n", COUT_OX_SOUFLE);
                info = "Pas assez d'O2 !";
            }
            else if (souffle == 1) {
                printf("Souffle est deja actif, il reste %d tours\n", duree_souffle);
                info = "Souffle deja actif !";
            }
            else {
                souffle = 1;
                p->niveau_oxygene -= COUT_OX_SOUFLE;
                duree_souffle = 3;
                printf("Souffle maitrise active : fatigue /2 pendant 3 tours !\n");
                info = "Souffle active !";
            }
            break;

        case 4: // Vague régénérante
            {
                if (p->niveau_oxygene - COUT_OX_VAGUE > 0)
                {
                    int soin = (int)(p->points_de_vie_max * 0.25);
                    p->points_de_vie += soin;
                    if (p->points_de_vie > p->points_de_vie_max)
                    p->points_de_vie = p->points_de_vie_max;
                    info = "Vague régénérante : +25%% PV restaurés instantanément !";

                    p->niveau_oxygene -= COUT_OX_VAGUE;
                    printf("competance consomme %d oxygen\n", COUT_OX_VAGUE);
                }else{
                    printf("Niveau d'oxygen insuffisant");
                }
                
                
            }
            break;

        default:
            info = "Compétence invalide.";
            break;
    }
}

// BOSS
// Appliquer effet boss
void appliquer_effet_boss(CreatureMarine *boss, Plongeur *p){
    if(!boss->est_vivant){
        return;
    }

    // 50% d'activer l'effet du Boss
    if(rand() % 100 >= 50){
        printf("Effet special Boss desactive pour ce tour..\n");
        return;
    }

    printf("BOSS ACTIVE %s !\n", boss->effet_special);

    if (strcmp(boss->effet_special, "RAGE") == 0) {
        boss->attaque_minimale += 8;
        boss->attaque_maximale += 8;
        printf("RAGE : ATK +8 ! (total: %d-%d)\n", boss->attaque_minimale, boss->attaque_maximale);
    }
    else if (strcmp(boss->effet_special, "MULTI") == 0) {
        printf("MULTI : 2e attaque !\n");
        attaquer_plongeur(boss, p);
    }
    else if (strcmp(boss->effet_special, "VENOM") == 0) {

        p->niveau_fatigue += 35;
        if (p->niveau_fatigue > FATIGUE_MAX) p->niveau_fatigue = FATIGUE_MAX;
        p->niveau_oxygene -= 15;
        if (p->niveau_oxygene < 0) p->niveau_oxygene = 0;
        
        printf("ABYSSAL GRIP : +35%% fatigue + -15 O2 !\n");
    }
    else if (strcmp(boss->effet_special, "VORTEX") == 0) {
        p->defense = p->defense / 2;
        if (p->defense < 1) p->defense = 1;
        printf("VORTEX : Defense divisee par 2 !\n");
    }
    else if (strcmp(boss->effet_special, "CHAOS") == 0) {

        int heal = 100;
        boss->points_de_vie_actuels += heal;
        if (boss->points_de_vie_actuels > boss->points_de_vie_max)
            boss->points_de_vie_actuels = boss->points_de_vie_max;
        printf("CHAOS : +100 PV !\n");
    }
}


// LOGIQUE COMBAT GLOBAL
void gerer_tour_combat(Plongeur *p, char cmd, char **screen) {
    extern CreatureMarine *g_creatures_en_combat;
    extern int g_nbr_creatures_en_combat;
    extern int g_creature_tour_index;
    extern char* info;
    char *competence[4] = {"Elan marin", "Cuirasse aquatique", "Souffle maitrise", "Vague regerante"};
    
    int choix_action = 0;  // 1=légère, 2=lourde, 3=économiser, 4=compétence
    int tour_joueur_effectue = 0;
    int profondeur = p->map_pos_y;

    verifier_oxygene_critique(p);

    
    if (cmd == 'A' || cmd == 'a') choix_action = 1;
    else if (cmd == 'B' || cmd == 'b') choix_action = 2;
    else if (cmd == 'C' || cmd == 'c') choix_action = 3;
    else if (cmd == 'D' || cmd == 'd'){
        choix_action = 4; // Competence
        screen_status = 11;
    } 
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
    // if (choix_action == 4){
    //     int comp = prompt_for_competence("Entrer une competence");
    //     // int choix_comp = choisir_competence();
    //     appliquer_competence(p, comp);
    // }
    if (choix_action == 1 || choix_action == 2) {
        // Demander la cible
        int target_index = prompt_for_target(g_nbr_creatures_en_combat, g_creatures_en_combat);
        CreatureMarine* cible = &g_creatures_en_combat[target_index];

        attaquer_creature(p, cible, choix_action);
        consommation_o2(p, choix_action, profondeur);
        
        if (cible->points_de_vie_actuels <= 0) {
            cible->est_vivant = 0;
            info = "Vous avez vaincu une creature !";
            
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