#include <stdio.h>
#include <stdlib.h>
//Affichage correcte en console windows
#include <windows.h>

//░▒▓█
//█▓▒░
//▮▯

int largeur = 20;
int hauteur = 20;

char fill = '~';

char **init_screen();
void print_screen(char **screen);

void screen_header();
void screen_main();
void screen_footer();


int main(){
    //Affichage correcte en console windows
    SetConsoleOutputCP(CP_UTF8);

    //char **screen = init_screen();
    //print_screen(screen);
    //free(screen);

    screen_header();
    screen_main();
    screen_footer();

    return 0;
}

void screen_header(){
    printf("╭─────────────────────────────── Ocean  Depth ────────────────────────────────╮\n"); //Static
    printf("│  Vie: ███████▒░░ 72%  |  O₂: ██████▒░░░ 64%  |  Fatigue: █▒░░░░░░░░ 15%     │\n"); //
    printf("│  Profondeur: 500M     |  Arme équipé : Harpon Rouillé                       │\n");
    printf("├─────────────────────────────────────────────────────────────────────────────┤\n"); //Static


}
void screen_main(){
    printf("│   ╭─────────────────────────────────────────────────────────────────────╮   │\n");
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   │                                                                     │   │\n"); //screen dyna
    printf("│   ╰─────────────────────────────────────────────────────────────────────╯   │\n"); //Static
    printf("├─────────────────────────────────────────────────────────────────────────────┤\n"); //Static
}
void screen_footer(){
    printf("│  [M] Carte  [I] Inventaire  [R] Remonter  [D] Descendre  [A] Attaquer       │\n");
    printf("╰─────────────────────────────────────────────────────────────────────────────╯\n"); //Static
}

char** init_screen(){

    // Allocation dynamique du tab d'ecran
    char **screen = malloc(hauteur * sizeof(char*));
    for (int i = 0; i < hauteur; i++){
        screen[i] = malloc(largeur * sizeof(char));
        for (int j = 0; j < largeur; j++) {
            screen[i][j] = fill;
        }
    }

    return screen;
}

// Affichage de truand ep2
void print_screen(char **screen) {

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            printf("%c  ", screen[i][j]);
        }
        printf("\n");
    }
}