#include <curses.h>

typedef struct lista{
    char caracter;
    struct lista *next;
}lista;

void inicializar(lista **l){
    *l = NULL;
}

lista* getnode(){
    return(lista *)malloc (sizeof(lista));
}

void freenode(lista *l){
    free(l);
}

int vazia(lista *l){
    if(l == NULL){
        return 1;
    }
    return 0;
}

void remover_inicio(lista **l){
    lista *remover;

    remover = *l;
    if(!vazia(*l)){
        *l = remover->next;
        freenode(remover);
    }else{
        printw("erro ao remover da lista");
        exit(1);
    }
}

void exibir_lista(lista *l) {
    lista *aux;

    if(vazia(l)) {
        printf("\nErro! - Lista vazia");
        return;
    }
    aux = l;
    while(!vazia(aux)) {
        printw("%c", aux->caracter);
        aux = aux->next;
    }
}

///fun��o(F2) que verifica se nome do arquivo para salvar as informa��es da estrutura j� existe por exemplo: ele verifica se o arquivo PauloBolher1.txt j� existe
///se existe ele verifica o PauloBolher2.txt at� encontra o que n�o existe e esse nome volta para a fun��o F2 como referencia por um vetor
///onde cria um arquivo como esse nome para salvar as informa��es
void verificar_existencia_arquivo(char novo_nome[]) {
    char nome_base[] = "PauloBolher";
    char extensao[] = ".txt";
    char texto[] = "_texto";
    char numero_arquivo_como_string[20];
    int numero_arquivo = 1;

    do {
        sprintf(numero_arquivo_como_string, "%d", numero_arquivo);
        strcpy(novo_nome, nome_base);
        strcat(novo_nome, numero_arquivo_como_string);
        strcat(novo_nome, texto);
        strcat(novo_nome, extensao);
        numero_arquivo++;
    } while (access(novo_nome, F_OK) == 0);

}
///verifica se tem um caracter qualquer a frente do cursor para pdoer movimentar ele
int verificar_andar_cursor_direita(lista **l, int posicao_cursor, int coluna_cursor){

    int j, i, contador = 0;
    lista *aux = *l;


    if(coluna_cursor == 0){

        for(j =0; j<posicao_cursor; j++){
            aux = aux->next;
        }

    }else{

        while(contador != coluna_cursor){

            if(aux->caracter == '\n' || aux->caracter == 10){
                contador++;
            }

            aux = aux->next;

        }

        for(i = 0; i< posicao_cursor; i++){
            aux = aux->next;
        }
    }

    if(aux == NULL || aux->caracter == '\n'){
        return 0;
    }
    return 1;
}

///verifica se possui algum caracter a esquerda do cursor para poder movelo
int verificar_andar_cursor_esquerda(lista **l, int posicao_cursor, int coluna_cursor){
    int j, i, contador = 0;
    lista *aux = *l;
    lista *anterior = NULL;

    if(coluna_cursor == 0){

        for(j =0; j<posicao_cursor; j++){
            anterior = aux;
            aux = aux->next;
        }

        if(anterior == NULL){
            return 0;
        }
        return 1;

    }else{

        while(contador != coluna_cursor){

            if(aux->caracter == '\n' || aux->caracter == 10){
                contador++;
            }
            aux = aux->next;

        }

        for(i = 0; i< posicao_cursor; i++){
            anterior = aux;
            aux = aux->next;
        }
    }

    if(anterior == NULL){
        return 0;
    }

    return 1;

}

int verificar_caractere_baixo(lista **l, int linha_cursor, int coluna_cursor){
    int contador = 0, coluna_linha_inferior = 0;
    lista *linha_inferior = *l;

    while (contador <= linha_cursor && linha_inferior != NULL) {
        if (linha_inferior->caracter == '\n') {
            contador++;
        }
        linha_inferior = linha_inferior->next;
    }

    if (linha_inferior == NULL || linha_inferior->next == NULL || linha_inferior->caracter == '\n') {
        return 0;
    }

    while (linha_inferior != NULL && linha_inferior->caracter != '\n' && coluna_linha_inferior < coluna_cursor) {
        if (linha_inferior->caracter != '\n') {
            coluna_linha_inferior++;
        }
        linha_inferior = linha_inferior->next;
    }

    move(linha_cursor + 1, coluna_linha_inferior);
    return 1;
}


void inserir_posicao(lista **l, int posicao, char caracter) {
    int contador = 0;
    lista *novo = getnode();
    lista *aux = *l;

    if (novo == NULL) {
        printf("Erro: falha na aloca��o de mem�ria.\n");
        exit(1);
    }
    novo->caracter = caracter;

    if (posicao == 0 || *l == NULL) {
        novo->next = *l;
        *l = novo;
        return;
    }

    while (contador < posicao - 1 && aux->next != NULL) {
        aux = aux->next;
        contador++;
    }

    novo->next = aux->next;
    aux->next = novo;

}

void inserir_fim(lista **l, char caracter){
    lista *novo_espaco_lista;
    lista *troca;

    novo_espaco_lista = getnode();

    if(novo_espaco_lista != NULL){
        novo_espaco_lista->caracter = caracter;
        novo_espaco_lista->next = NULL;

        if(vazia(*l)){
            *l = novo_espaco_lista;
        }else{
            troca = *l;

            while(troca->next != NULL){
                troca = troca->next;
            }
            troca->next = novo_espaco_lista;
        }
    }else{
        exit(1);
    }
}

///verifica se tem um caracter acima de onde o cursor est� se tiver ele move para a posi��o caso n�o tenah ele coloca o cursor no final da linha de cima
int verificar_caractere_acima(lista **l, int linha_cursor, int coluna_cursor) {

    int contador = 0, coluna_linha_superior = 0;
    lista *linha_atual = *l;
    lista *linha_superior = *l;

    while (contador < linha_cursor - 1 && linha_superior != NULL) {
        if (linha_superior->caracter == '\n') {
            contador++;
        }
        linha_superior = linha_superior->next;
    }

    if (linha_superior == NULL ) {
        return 0;
    }

    while (linha_superior != NULL && linha_superior->caracter != '\n' && coluna_linha_superior < coluna_cursor) {
        if (linha_superior->caracter != '\n') {
            coluna_linha_superior++;
        }
        linha_superior = linha_superior->next;
    }

    move(linha_cursor - 1, coluna_linha_superior);
    return 1;
}
