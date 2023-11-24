#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h> // para mapear teclas como f1 e f10, e tambem armazenar carcteres na estrutura de dados sem apartar enter
#include <unistd.h> // para saber se um arquivo já existe
#include "PauloBolher.h"

void tecla_F1(lista **l){
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
        exibir_lista(*l);
    }
}

void tecla_F2(lista **l){
    int ch;
    char nome_arquivo[20];
    FILE *arquico;
    lista *aux = *l;

    verificar_existencia_arquivo(nome_arquivo);

    arquico = fopen(nome_arquivo, "w");

    if(arquico != NULL){
        while(aux != NULL){
            fputc(aux->caracter, arquico);
            aux = aux->next;
        }
    }else{
        clear();
        printw("erro ao abrir o arquivo precione enter para contiar");

        ch = getch();

        if(ch == 10){
            clear();
        }
    }
    clear();
    exibir_lista(*l);
    refresh();
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

void tecla_f12(lista **l){
    int ch;

    clear();
    printw("Prontuario do aluno..........................: PE301004X\n");
    printw("Nome do aluno................................: Paulo Eduardo Bolher\n");
    printw("curso do aluno...............................: bacharelado em ciencia da computacao(BCC)\n");
    printw("Ano de ingressao do aluno....................: 2021\n");

    printw("\nprecione qualquer tecla esq para sair");
    ch = getch();

    if(ch == 27){
        clear();
        exibir_lista(*l);
    }
}


void main(){
    int ch, retorno;
    int linha_cursor = 0, coluna_cursor = 0, posicao_inserir = 0;

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
            tecla_F1(&l);

        }else if(ch == KEY_F(2)){
            tecla_F2(&l);

        }else if(ch == KEY_F(10)){
            tecla_F10(&l);

        }else if(ch == KEY_F(12)){
            tecla_f12(&l);

        }else if(ch == KEY_RIGHT){
            if(verificar_andar_cursor_direita(&l, linha_cursor, coluna_cursor)){
                linha_cursor++;
            }
            move(coluna_cursor, linha_cursor);

        }else if(ch == KEY_UP){
            if(verificar_caractere_acima(&l, coluna_cursor, linha_cursor)){
                coluna_cursor --;
            }

        }else if(ch == KEY_DOWN){
            if(verificar_caractere_baixo(&l, coluna_cursor, linha_cursor)){
                coluna_cursor++;
            }

        }else if(ch == KEY_LEFT){
            if(verificar_andar_cursor_esquerda(&l, linha_cursor, coluna_cursor)){
                linha_cursor--;
            }
            move(coluna_cursor, linha_cursor);

        ///caso nenhuma tecla de ação tenha sido preciona ele interpreta como um carcter e armazena na lista
        }else if(ch == KEY_ENTER || ch == '\n'){

            inserir_posicao(&l, posicao_inserir, '\n');
            posicao_inserir++;
            coluna_cursor ++;
            linha_cursor = 0;
            move(coluna_cursor, linha_cursor);

        }else {
            linha_cursor++;

            if (linha_cursor % 119 == 0) {

                inserir_posicao(&l, posicao_inserir, '\n');
                posicao_inserir++;
                coluna_cursor ++;
                linha_cursor = 0;
            }

            inserir_posicao(&l, posicao_inserir, (char)ch);
            move(coluna_cursor, linha_cursor);
            posicao_inserir++;
        }

    /// encerra o progrma se o carcater lido for o 27 no caso é a tecla Esq
    }while(ch != 27);

    printw("\ndeseja salvar as informações em um arquivo: 1-sim, 2-nao");
    ch = getch();

    if(ch == '1'){
        tecla_F2(&l);
    }
}
