#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include "função-lista.h"

void exibir_lista(lista *l) {
    lista *aux;

    if(vazia(l)) {
        printf("\nErro! - Lista cheia");
        return;
    }
    aux = l;
    while(!vazia(aux)) {
        printf("%c", aux->caracter);
        aux = aux->next;
    }
    printf("\n");
}


void main(){
    lista *l;
    inicializar(&l);

    initscr();  // Inicializa a biblioteca ncurses
    cbreak();   // Habilita a entrada de caracteres sem necessidade de Enter
    keypad(stdscr, TRUE);  // Habilita teclas de função (F1, F2, etc.)

    int ch;
    while (1) {
        ch = getch();
        if (ch == 27) {
            break; // Saia do loop se a tecla Esc for pressionada
        } else if (ch == KEY_F(1)) {
            printw("A tecla F1 foi pressionada\n");
        } else {
            inserir_fim(&l, (char)ch);
        }
    }

    exibir_lista(l);

    endwin();
}
