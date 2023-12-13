#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h> /// para mapear teclas como f1 e f10, e tambem armazenar carcteres na estrutura de dados sem apartar enter
#include <unistd.h> /// para saber se um arquivo j� existe
#include <windows.h>
#include "PauloBolher.h"

///foi utilizado a biblioteca PDcurses sendo necessario a sua instala��o e compila��o para o funcioanmento do codigo fonte
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

        ///caso n�o esteja na primeira linha percorre at� a linha do cursosr contando a quantidade de "\n"
        while(contador != *linha_cursor){

            if(aux->caracter == '\n'){
                contador++;
            }
            aux = aux->next;

        }

        /// realiza a mesma coisa que o for de quando o cursor est� na primeira linha
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

    // Encontrar a posi��o do cursor na lista
    posicao_cursor = saber_posicao_cursor(l, linha_cursor, coluna_cursor);

    // Se a posi��o do cursor for 0, n�o h� nada para apagar
    if (posicao_cursor == 0) {
        return;
    }

    // Navegar at� o n� anterior ao n� que ser� removido
    aux = *l;
    for (int i = 0; i < posicao_cursor - 1; ++i) {
        anterior = aux;
        aux = aux->next;
    }

    // Se o n� atual n�o for o primeiro n�
    if (anterior != NULL) {
        // Remover o n� atual e conectar o n� anterior ao pr�ximo n�
        anterior->next = aux->next;
        freenode(aux);

        // Atualizar a posi��o do cursor
        (*coluna_cursor)--;

        // Deslocar todos os caracteres subsequentes para a esquerda
        lista *atual = anterior;
        while (atual->next != NULL && atual->next->caracter != '\n') {
            atual->caracter = atual->next->caracter;
            atual = atual->next;
        }

        // Se o pr�ximo n� for uma quebra de linha, encontre o in�cio da pr�xima linha
        if (atual->next != NULL && atual->next->caracter == '\n') {
            lista *proxima_linha = atual->next->next;
            // Se a pr�xima linha n�o estiver vazia, mova o primeiro caractere
            if (proxima_linha != NULL && proxima_linha->caracter != '\n') {
                atual->caracter = proxima_linha->caracter;
                atual->next = proxima_linha->next;
                freenode(proxima_linha);
            }
        }
    } else {
        // Se o n� a ser removido for o primeiro da lista
        *l = aux->next;
        freenode(aux);
        // Atualizar a posi��o do cursor
        (*coluna_cursor)--;
    }

    // Reajustar a posi��o das quebras de linha
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
                // Insere uma quebra de linha ap�s o �ltimo caractere n�o nulo
                lista *nova_quebra_de_linha = (lista *)malloc(sizeof(lista));
                nova_quebra_de_linha->caracter = '\n';
                nova_quebra_de_linha->next = ultimo_nao_nulo->next;
                ultimo_nao_nulo->next = nova_quebra_de_linha;
            }
            contador_coluna = 0;
        }
        aux = aux->next;
    }

    // Atualizar a exibi��o do texto
    clear();
    exibir_lista(*l);
    refresh();

    // Mover o cursor para a posi��o correta
    move(*linha_cursor, *coluna_cursor);
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

    lista *l;

    ///travando o terminal
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, 119, 39};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    SetConsoleScreenBufferSize(hConsole, (COORD){120, 40});


    inicializar(&l);

    /// Inicializa a biblioteca curses
    initscr();
    /// inicializa a fun��o para armazenar carcteres na estrutura de dados sem apartar enter da biblioteca curses
    cbreak();
    /// inicialzia a mapea��o de teclas como f1 e f10 da biblioteca curses
    keypad(stdscr, TRUE);

    noecho();
    ///pagina��o no terminal



//    scrollok(stdscr, TRUE);
//    setscrreg(0, buffer_height - 1);

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

            ///dentro do inserte foi implementado novamente a navega��o com setas para ele poder navegar quando est� no modo insert

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

        ///tecla delete
        }else if(ch == KEY_DC){
            tecla_delete(&l, &coluna_cursor, &linha_cursor);

        ///caso nenhuma tecla de a��o tenha sido preciona ele interpreta como um carcter e armazena na lista
        }else {
            linha_cursor++;

            if (linha_cursor % 119 == 0) {

                inserir_posicao(&l, &posicao_inserir, '\n');
                posicao_inserir++;
                coluna_cursor++;
                total_linha ++;
                linha_cursor = 1;
            }

            if(saber_caraceter_posicao(&l, coluna_cursor, linha_cursor)){

            }

            inserir = saber_posicao_cursor(&l, &coluna_cursor, &linha_cursor);

            lista *aux;

            aux = l;

            int contador = 1;

            while(contador < inserir){
                aux = aux->next;
                contador++;
            }

            if(aux != NULL && aux->caracter == '\n' && aux->next != NULL){
                inserir--;
            }

            inserir_posicao(&l, &inserir, (char)ch);

            posicao_inserir++;

            clear();
            exibir_lista(l);
            refresh();

            retorno = contador_coluna(&l, coluna_cursor, linha_cursor);

            move(coluna_cursor, retorno);


        }

    /// encerra o progrma se o carcater lido for o 27 no caso � a tecla Esq
    }while(ch != 27);

    printw("\ndeseja salvar as informa��es em um arquivo: 1-sim, 2-nao");
    ch = getch();

    if(ch == '1'){
        tecla_F2(&l);
    }

    endwin();

}
