#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <time.h>

void tabla(WINDOW *a[6][5]);
void joc(WINDOW *a[6][5], char cuv[30][6]);
void menu(WINDOW *a[6][5], char cuv[30][6]);

int cuvgen(char cuv[30][6]) {
    srand((unsigned)time(NULL));
    return rand() % 27;
}
/*functie care genereaza indicele unui cuvant aleator*/

void tabla(WINDOW *a[6][5]) {
    initscr();
    mvprintw(0, 64, "WORDLE");
    int i, j;
    for(i = 0; i < 6; i++) {
        for(j = 0; j < 5; j++) {
            a[i][j] = newwin(3, 6, 6 * (i + 1) / 2 ,10 * (j + 10) / 2 + j + 2);
            refresh();
            box(a[i][j], 0, 0);
            wrefresh(a[i][j]);
        } 
    }
}
/*se genereaza cele 30 de patrate in care se vor scrie incercarile de cuvinte*/

void joc(WINDOW *a[6][5], char cuv[30][6]) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_YELLOW);
/*se genereaza cele doua perechi de culori cu care se vor umple patratelele*/
    WINDOW *errwin = newwin(3, 40, 1 ,1);
    int i, j, cul[5], k, x, win = 0;
    int l = cuvgen(cuv);
/*se genereaza indicele cuvantului de ghicit*/
    for(i = 0; i < 6; i++) {
        char aux[6], cop[6];
        for(k = 0; k < 5; k++) {
            cul[k] = -1;
        }
        /*se initializeaza vectorul de culori cu -1 care semnifica negru*/
        for(k = 0; k < 5; k++) {
            aux[k] = cuv[l][k];
        }
        /*se creeaza o copie a cuvantului de ghicit*/
        k = -1;
        for(j = 0; j < 5; j++) {
            wmove(a[i][j], 1, 2);
            int c = wgetch(a[i][j]);
            if(c == 58) {
                wclear(a[i][j]);
                box(a[i][j], 0, 0);
                wrefresh(a[i][j]);
                j = j - 1;
                menu(a, cuv);
                /*daca se apasa tasta ":" se va afisa meniul*/
            }
            cop[++k] = (char)c;
            /*se retine intreaga solutie introdusa*/
            wclear(errwin);
            wrefresh(errwin);
            refresh();
            /*in cazul a care a fost afisata anterior o eroare se sterge*/
            if(c == 127) {
                wclear(a[i][j]);
                box(a[i][j], 0, 0);
                wrefresh(a[i][j]);
                if(j < 2) {
                    if(j == 0) {
                        j = -1;
                    }
                    if(j == 1) {
                        j = -1;
                    }
                } else {
                    j = j - 2;
                }
                /*se realizeaza implementarea tastei backspace*/
            } else {
                if(c == 10) {
                    j--;
                    mvwprintw(errwin, 1, 1,
                    "NU AI INTRODUS SUFICIENTE CARACTERE!");
                    wrefresh(errwin);
                    refresh();
                    /*in cazul in care se apasa enter si nu sunt 5
                    caractere introduse este afisata o eroare*/
                }
            }
            if(j == 4) {
                int c = getch();
                int s = 0;
                if(c == 10) {
                    for(k = 0 ; k < 5; k++) {
                        if(cop[k] == aux[k]) {
                        aux[k] = '*';
                        cul[k]  = 1;
                        cop[k] = '?';
                        s++;
                        }
                    }
                    /*se verifica cate caractere introduse de la tastatura
                    sunt pe pozitia corecta si se marcheaza*/
                    for(k = 0 ; k < 5; k++) {
                        for(x = 0; x < 5; x++) {
                            if(cop[x] == aux[k]) {
                                aux[k] = '*';
                                cop[x] = '?';
                                cul[x] = 0;
                                break;
                            }
                        }
                    }
                    /*se verifica daca celelalte litere exista in cuvantul
                    de ghicit, dar pe alte pozitii si se marcheaza*/
                    for(k = 0; k < 5; k++) {
                        if(cul[k] == 1) {
                            wbkgd(a[i][k], COLOR_PAIR(1));
                            wrefresh(a[i][k]);
                        } else {
                            if(cul[k] == 0) {
                                wbkgd(a[i][k], COLOR_PAIR(2));
                                wrefresh(a[i][k]);
                            }
                        }
                    }
                    /*se coloreaza casutele*/
                    if(s == 5) {
                        win = 1;
                        mvprintw(20, 20, "Felicitari, ai castigat!");
                        menu(a, cuv);
                    }
                    /*in cazul in care a fost ghicit cuvantul se afiseaza
                    un mesaj si se ofera posibilitatea unui nou joc*/
                } else {
                    if(c == 127) {
                        wclear(a[i][j]);
                        box(a[i][j], 0, 0);
                        wrefresh(a[i][j]);
                        j = j - 1;
                    } 
                }
            }
        }
    }
    if(!win) {
        mvprintw(20, 20, "Game over!Cuvantul era %s", cuv[l]);
        menu(a, cuv);
    }
    /*in cazul in care dupa cele 6 incercari nu a fost ghicit
    cuvatul, se afiseaza un mesaj si  se ofera posiblitatea 
    inceperii unui nou joc*/
}

void menu(WINDOW *a[6][5], char cuv[30][6]) {
    initscr();
    WINDOW *menuwin = newwin(6, 12, 8 ,5);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin, true);
    /*se apeleaza functia pentru a putea folosi sagetile*/
    char options[2][100] = {"Joc nou", "Iesi"};
    int alegere;
    int highlight = 0;
    while(1) {
        int i;
        for(i = 0; i < 2; i++) {
            if(i == highlight) {
                wattron(menuwin, A_REVERSE);
                /*se evidentiaza optiunea pe care o selectam*/
            }
            mvwprintw(menuwin, i + 1, 1, options[i]);
            wattroff(menuwin, A_REVERSE);
        }
        alegere = wgetch(menuwin);
        switch(alegere) {
            case KEY_UP:
                highlight--;
                if(highlight == -1) {
                    highlight = 1;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == 2) {
                    highlight = 0;
                }
                break;
            default:
                break;
        }
        if(alegere == 10) {
            break;
        }
    }
    if(strcmp(options[highlight], "Joc nou") == 0) {
        clear();
        tabla(a);
        joc(a, cuv);
    } else {
        endwin();
        exit(0);
    }
    /*se verifica alegerea facuta prin enter*/
}

int main() {
    char cuv[30][6] = {"arici", "atent", "baiat", "ceata", "debut", "peste",
    "fixat", "hamac", "harta", "jalon", "jucam", "lacat", "magie", "nufar",
    "oaste", "perus", "rigle", "roman", "sanie", "scris", "sonda", "texte",
    "tipar", "titan", "zebra", "vapor", "vatra"};
    WINDOW *a[6][5];
    cbreak();
    noecho();
    tabla(a);
    joc(a, cuv);
    getch();
    endwin();
}