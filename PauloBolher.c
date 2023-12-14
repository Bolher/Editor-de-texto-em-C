#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h> /// para mapear teclas como f1 e f10, e tambem armazenar carcteres na estrutura de dados sem apartar enter
#include <unistd.h> /// para saber se um arquivo já existe
#include <windows.h>
#include "PauloBolher.h"

///foi utilizado a biblioteca PDcurses sendo necessario a sua instalação e compilação para o funcioanmento do codigo fonte
///foi perguntado para o professor se poderia utilizar essa biblioteca e o mesmo autorizou o uso da biblioteca


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

        fclose(arquico);
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

void seta_para_direita(lista **l, int *linha_cursor, int *coluna_cursor){
    if(verificar_andar_cursor_direita(l, coluna_cursor, linha_cursor)){
        (*coluna_cursor)++;
    }
    move(*linha_cursor, *coluna_cursor);
}

void seta_para_esquerda(lista **l, int *linha_cursor, int *coluna_cursor){
    if(verificar_andar_cursor_esquerda(l, coluna_cursor, linha_cursor)){
        (*coluna_cursor)--;
    }
    move(*linha_cursor, *coluna_cursor);
}

void seta_para_cima(lista **l, int *linha_cursor, int *coluna_cursor){
    int retorno;

    retorno = verificar_caractere_acima(l, linha_cursor, coluna_cursor);

    if(retorno > 0){
        *coluna_cursor = retorno - 1;
        (*linha_cursor)--;
        move(*linha_cursor, *coluna_cursor);

    }else if(retorno == 0){
        (*linha_cursor)--;
        move(*linha_cursor, 0);
        (*coluna_cursor) = 0;
    }

}

void seta_para_baixo(lista **l, int *linha_cursor, int *coluna_cursor){
    int retorno;

    retorno = verificar_caractere_baixo(l, linha_cursor, coluna_cursor);

    if(retorno > 0){
        *coluna_cursor = retorno - 1;
        (*linha_cursor)++;
        move(*linha_cursor, *coluna_cursor);

    }else if(retorno == 0){
        (*linha_cursor)++;
        move(*linha_cursor, 0);
        (*coluna_cursor) = 0;
    }
}


void page_down(lista **l, int *linha_cursor, int *coluna_cursor, int total_linha){
    int verficar_pageDOWN = 0;

    verficar_pageDOWN = total_linha - (*linha_cursor);

    if(verficar_pageDOWN >= 25){
        *linha_cursor = (*linha_cursor) + 25;
        move(*linha_cursor, *coluna_cursor);

    }else{
        *linha_cursor = total_linha;
        move(total_linha, *coluna_cursor);
    }
}

void page_up(lista **l, int *linha_cursor, int *coluna_cursor, int total_linha){
    int verificar_pageUP = 0;

    verificar_pageUP = (*linha_cursor) - total_linha + 1 ;

    if(verificar_pageUP >= 0){
        *linha_cursor = (*linha_cursor) - 25;
        move(*linha_cursor, *coluna_cursor);

    }else{
        *linha_cursor = 0;
        move(*linha_cursor, *coluna_cursor);
    }
}

void enter(lista **l, int *linha_cursor, int *coluna_cursor, int *posicao_inserir, int *total_linha){
    int retorno;
    retorno = saber_posicao_cursor(l, linha_cursor, coluna_cursor);
    inserir_posicao(l, &retorno, '\n');



    if(validacao_enter(l, coluna_cursor, linha_cursor)){

        (*linha_cursor)++;
        (*coluna_cursor) = contador_coluna(l, *linha_cursor, *coluna_cursor);
        (*posicao_inserir) ++;
        (*total_linha) ++;


        seta_para_cima(l, linha_cursor, coluna_cursor);
        seta_para_baixo(l, linha_cursor,coluna_cursor);

        move(*linha_cursor, *coluna_cursor);

    }else{

        (*coluna_cursor) = 0;
        (*linha_cursor) = (*linha_cursor) + 2;
        (*posicao_inserir) ++;
        (*total_linha) ++;

        seta_para_cima(l, linha_cursor, coluna_cursor);
        seta_para_baixo(l, linha_cursor, coluna_cursor);

        move(*linha_cursor, *coluna_cursor);

    }



    clear();
    exibir_lista(*l);
    refresh();

    move(*linha_cursor, *coluna_cursor);

    //printw("linha: %d, coluna: %d", *linha_cursor, *coluna_cursor);

}

void insert(lista **l, int *linha_cursor, int *coluna_cursor, char ch){
    lista *aux = *l;
    int j, contador = 0;
    if(*linha_cursor == 0){

        for(j =0; j< *coluna_cursor; j++){
            aux = aux->next;
        }
        if(aux != NULL){
            aux->caracter = ch;
        }

    }else{

        ///caso não esteja na primeira linha percorre até a linha do cursosr contando a quantidade de "\n"
        while(contador != *linha_cursor){

            if(aux->caracter == '\n'){
                contador++;
            }
            aux = aux->next;

        }

        /// realiza a mesma coisa que o for de quando o cursor está na primeira linha
        for(j = 0; j< *coluna_cursor; j++){
            aux = aux->next;
        }

        aux->caracter = ch;
    }

    clear();
    exibir_lista(*l);

    move(*linha_cursor, *coluna_cursor);
}

void apagar(lista **l, int *linha_cursor, int *coluna_cursor) {
    int posicao_cursor;
    lista *aux, *anterior = NULL;
    int contador_caracteres = 0;
    int contador_linhas = -1;
    int vet[100000];

    if((*linha_cursor) == 0 && (*coluna_cursor) == 0){
        return;
    }

    posicao_cursor = saber_posicao_cursor(l, linha_cursor, coluna_cursor);

    if(*coluna_cursor <= 0 ){
        contar_caracter(l, vet);
        (*linha_cursor)--;
        (*coluna_cursor) = vet[(*linha_cursor)];
        move(*linha_cursor, *coluna_cursor);
    }

    if (posicao_cursor == 0) {
        return;
    }

    /// vai até o nó anterior ao nó que será removido
    aux = *l;
    for (int i = 0; i < posicao_cursor - 1; ++i) {
        anterior = aux;
        aux = aux->next;
    }

    /// se o nó atual não for o primeiro nó, remove o nó atual e conectar o nó anterior ao próximo nó
    if (anterior != NULL) {

        anterior->next = aux->next;

        freenode(aux);

        (*coluna_cursor)--;


        /// contar os caracteres na linha atual e as quebras de linha
        lista *contagem = *l;
        while (contagem != NULL && contagem != aux) {
            if (contagem->caracter == '\n') {
                contador_linhas++;
                if (contador_linhas == *linha_cursor) {
                    break; /// encontra o fim da linha atual
                }
                contador_caracteres = 0;
            } else {
                contador_caracteres++;
            }
            contagem = contagem->next;
        }

        /// se a linha atual estiver completa, mover o conteúdo da linha de baixo, deslocar todos os caracteres para a esquerda
        if (contador_caracteres >= 117) {

            lista *atual = anterior;
            while (atual->next != NULL && atual->next->caracter != '\n') {
                atual->caracter = atual->next->caracter;
                atual = atual->next;
            }
            /// se o próximo nó for uma quebra de linha, encontre o início da próxima linha e se a próxima linha não estiver vazia, mova o primeiro caractere
            if (atual->next != NULL && atual->next->caracter == '\n') {
                lista *proxima_linha = atual->next->next;
                if (proxima_linha != NULL && proxima_linha->caracter != '\n') {
                    atual->caracter = proxima_linha->caracter;
                    atual->next = proxima_linha->next;
                    freenode(proxima_linha);
                }
            }
        }
    } else {
        /// se o nó a ser removido for o primeiro da lista atualizar a posição do cursor
        *l = aux->next;
        freenode(aux);
        (*coluna_cursor)--;
    }

    /// reajustar a posição das quebras de linha
    aux = *l;
    int contador_coluna = 0;
    lista *ultimo_nao_nulo = NULL;
    while (aux != NULL) {
        contador_coluna++;
        if (aux->caracter != '\n') {
            ultimo_nao_nulo = aux;
        }
        if (contador_coluna == 120 || aux->next == NULL || aux->next->caracter == '\n') {
            if (ultimo_nao_nulo != NULL && (ultimo_nao_nulo->next == NULL || ultimo_nao_nulo->next->caracter != '\n')) {
                /// insere uma quebra de linha após o último caractere não nulo
                lista *nova_quebra_de_linha = (lista *)malloc(sizeof(lista));
                nova_quebra_de_linha->caracter = '\n';
                nova_quebra_de_linha->next = ultimo_nao_nulo->next;
                ultimo_nao_nulo->next = nova_quebra_de_linha;
            }
            contador_coluna = 0;
            ultimo_nao_nulo = NULL;
        }
        aux = aux->next;
    }
    clear();
    exibir_lista(*l);
    refresh();


    move(*linha_cursor, *coluna_cursor);

    //printw("linha: %d, coluna: %d", *linha_cursor, *coluna_cursor);
}

void tecla_delete(lista **l, int *linha_cursor, int *coluna_cursor){
    int posicao_cursor;

    if (*l != NULL) {
        posicao_cursor = saber_posicao_cursor(l, linha_cursor, coluna_cursor);

        remover_posicao(l, posicao_cursor);
        clear();
        exibir_lista(*l);
        refresh();

        (*coluna_cursor)--;

        if((*linha_cursor) != 0){
            seta_para_cima(l, linha_cursor, coluna_cursor);
            seta_para_baixo(l, linha_cursor, coluna_cursor);
        }else{
            seta_para_direita(l, linha_cursor, coluna_cursor);
            seta_para_esquerda(l, linha_cursor, coluna_cursor);
        }


        move(*linha_cursor, *coluna_cursor);
    }

}

void main(){
    int ch;
    int linha_cursor = 0, coluna_cursor = 0, posicao_inserir = 0;
    int total_linha = 0;
    int retorno, inserir = 0;
    int total_branco;
    int buffer_height = 100;
    int validar = 0;

    lista *l;

    ///travando o terminal


    inicializar(&l);

    /// Inicializa a biblioteca curses
    initscr();
    /// inicializa a função para armazenar carcteres na estrutura de dados sem apartar enter da biblioteca curses
    cbreak();
    /// inicialzia a mapeação de teclas como f1 e f10 da biblioteca curses
    keypad(stdscr, TRUE);

    scrollok(stdscr, TRUE);
    //setscrreg(0, buffer_height - 1);

    do{
        ch = getch();

        ///verifica se a tecla precionada foi o f1
        if (ch == KEY_F(1)) {
            tecla_F1(&l);

        ///f2
        }else if(ch == KEY_F(2)){
            tecla_F2(&l);

        ///f10
        }else if(ch == KEY_F(10)){
            tecla_F10(&l);

        ///f12
        }else if(ch == KEY_F(12)){
            tecla_f12(&l);

        ///seta para direita
        }else if(ch == KEY_RIGHT){
            seta_para_direita(&l, &coluna_cursor, &linha_cursor);

        ///seta para cima
        }else if(ch == KEY_UP){
            seta_para_cima(&l, &coluna_cursor, &linha_cursor);

        ///seta apra baixo
        }else if(ch == KEY_DOWN){
            seta_para_baixo(&l, &coluna_cursor, &linha_cursor);

        ///seta para esquerda
        }else if(ch == KEY_LEFT){
            seta_para_esquerda(&l, &coluna_cursor, &linha_cursor);

        ///tecla enter
        }else if(ch == KEY_ENTER || ch == '\n'){
            enter(&l, &coluna_cursor, &linha_cursor, &posicao_inserir, &total_linha);


        ///tecla insert
        }else if(ch == KEY_IC){

            ///dentro do inserte foi implementado novamente a navegação com setas para ele poder navegar quando está no modo insert

            do{
                ch = getch();

                if(ch == KEY_LEFT){
                    seta_para_esquerda(&l, &coluna_cursor, &linha_cursor);

                }else if(ch == KEY_RIGHT){
                    seta_para_direita(&l, &coluna_cursor, &linha_cursor);

                }else if(ch == KEY_UP){
                    seta_para_cima(&l, &coluna_cursor, &linha_cursor);

                }else if(ch == KEY_DOWN){
                    seta_para_baixo(&l, &coluna_cursor, &linha_cursor);

                }else{
                    if(ch != KEY_IC){
                        insert(&l, &coluna_cursor, &linha_cursor, (char)ch);
                    }
                }

            }while(ch != KEY_IC);

        ///tecla page down
        }else if(ch == KEY_NPAGE){
            page_down(&l, &coluna_cursor, &linha_cursor, total_linha);

        ///tecla page up
        }else if(ch == KEY_PPAGE){
            page_up(&l, &coluna_cursor, &linha_cursor, total_linha);

        ///tecla home
        }else if(ch == KEY_HOME){
            move(coluna_cursor, 0);
            linha_cursor = 0;

        ///tecla end
        }else if(ch == KEY_END){
            linha_cursor = verificar_final_linha(&l, coluna_cursor, linha_cursor);
            move(coluna_cursor, linha_cursor);

        /// tecla backspace
        }else if(ch == 8){
            apagar(&l, &coluna_cursor, &linha_cursor);
            validar = 1;

        ///tecla delete
        }else if(ch == KEY_DC){
            tecla_delete(&l, &coluna_cursor, &linha_cursor);

        ///caso nenhuma tecla de ação tenha sido preciona ele interpreta como um carcter e armazena na lista
        }else{
            linha_cursor++;

            if (linha_cursor % 119 == 0) {

                inserir_posicao(&l, &posicao_inserir, '\n');
                posicao_inserir++;
                coluna_cursor++;
                total_linha ++;
                linha_cursor = 1;
            }

            //if(saber_caraceter_posicao(&l, coluna_cursor, linha_cursor)){

            //}

            inserir = saber_posicao_cursor(&l, &coluna_cursor, &linha_cursor);

            lista *aux;

            aux = l;

            int contador = 1;

            while(contador < inserir){
                aux = aux->next;
                contador++;
            }

            if(validar == 1){
                inserir --;
            }

            if((aux != NULL && aux->caracter == '\n' && aux->next != NULL) && validar == 0){
                inserir--;
            }

            if(ch == '\t'){
                ch == ' ';
            }



            inserir_posicao(&l, &inserir, (char)ch);

            posicao_inserir++;

            clear();
            exibir_lista(l);
            refresh();

            linha_cursor = contador_coluna(&l, coluna_cursor, linha_cursor);

            move(coluna_cursor, linha_cursor);

            //validar = 0;
        }

        //printw("linha: %d, coluna: %d", coluna_cursor, linha_cursor);

    /// encerra o progrma se o carcater lido for o 27 no caso é a tecla Esq
    }while(ch != 27);

    printw("\ndeseja salvar as informações em um arquivo: 1-sim, 2-nao");
    ch = getch();

    if(ch == '1'){
        tecla_F2(&l);
    }

    endwin();

}
