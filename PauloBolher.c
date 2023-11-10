#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h> // para mapear teclas como f1 e f10, e tambem armazenar carcteres na estrutura de dados sem apartar enter
#include <unistd.h> // para saber se um arquivo já existe
#include "PauloBolher.h"

void tecla_F1(){
    int ch;
    char caracter;

    FILE *arquivo;

    arquivo = fopen("PauloBolher_ajuda.txt", "r");

    if(arquivo == NULL){
        exit(1);
    }

    clear();

    while((caracter = fgetc(arquivo)) != EOF) {
        printw("%c", caracter);
    }

    fclose(arquivo);

    ch = getch();

    if(ch == 27){
        clear();
    }
}

void tecla_F2(lista **l){
    int ch;
    char nome_arquivo[20];
    FILE *arquico;

    verificar_existencia_arquivo(nome_arquivo);

    arquico = fopen(nome_arquivo, "w");

    if(arquico != NULL){
        while(*l != NULL){
            fputc((*l)->caracter, arquico);
            *l = (*l)->next;
        }
    }else{
        clear();
        printw("erro ao abrir o arquivo precione enter para contiar");

        ch = getch();

        if(ch == 10){
            clear();
            exibir_lista(*l);
        }
    }

}

void tecla_F10(lista **l) {
    int ch;
    char nome_arquivo[50];
    int caracter;
    FILE *arquivo;

    clear();
    printw("Informe o nome do arquivo que deseja abrir: ");

    getnstr(nome_arquivo, sizeof(nome_arquivo));


    arquivo = fopen(nome_arquivo, "r");

    if (arquivo != NULL) {

        while ((caracter = fgetc(arquivo)) != EOF) {
            inserir_fim(l, (char)caracter);
        }
        fclose(arquivo);
    } else {
        clear();
        printw("Erro ao abrir o arquivo. Pressione qualquer tecla para continuar.");
        ch = getch();
    }

    clear();
    exibir_lista(*l);
    refresh();
}


void main(){
    int ch;
    lista *l;

    inicializar(&l);

    /// Inicializa a biblioteca curses
    initscr();
    /// inicializa a função para armazenar carcteres na estrutura de dados sem apartar enter da biblioteca curses
    cbreak();
    /// inicialzia a mapeação de teclas como f1 e f10 da biblioteca curses
    keypad(stdscr, TRUE);


    do{
        ch = getch();

        ///verifica se a tecla precionada foi o f1
        if (ch == KEY_F(1)) {
            tecla_F1();

        }else if(ch == KEY_F(2)){
            tecla_F2(&l);

        }else if(ch == KEY_F(10)){
            tecla_F10(&l);

        ///caso nenhuma tecla de ação tenha sido preciona ele interpreta como um carcter e armazena na lista
        }else {
            inserir_fim(&l, (char)ch);
        }

    /// encerra o progrma se o carcater lido for o 27 no caso é a tecla Esq
    }while(ch != 27);

    exibir_lista(l);

}
