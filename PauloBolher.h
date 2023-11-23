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

///função(F2) que verifica se nome do arquivo para salvar as informações da estrutura já existe por exemplo: ele verifica se o arquivo PauloBolher1.txt já existe
///se existe ele verifica o PauloBolher2.txt até encontra o que não existe e esse nome volta para a função F2 como referencia por um vetor
///onde cria um arquivo como esse nome para salvar as informações
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
int verificar_andar_cursor_direita(lista **l, int posicao_cursor){

    int j;
    lista *aux = *l;

    for(j =0; j<posicao_cursor; j++){
        aux = aux->next;
    }

    if(aux == NULL){
        return 0;
    }
    return 1;
}

///verifica se possui algum caracter a esquerda do cursor para poder movelo
int verificar_andar_cursor_esquerda(lista **l, int posicao_cursor){
    int j;
    lista *aux = *l;

    for(j =0; j<posicao_cursor--; j++){
        aux = aux->next;
    }

    if(aux == NULL){
        return 0;
    }
    return 1;
}

void inserir_posicao(lista **l, int posicao, char caracter) {
    int contador = 0;
    lista *novo = getnode();
    lista *aux = *l;

    if (novo == NULL) {
        printf("Erro: falha na alocação de memória.\n");
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

///verifica se tem um caracter acima de onde o cursor está se tiver ele move para a posição caso não tenah ele coloca o cursor no final da linha de cima
int verificar_caractere_acima(lista **l, int linha_cursor, int coluna_cursor) {

    int contador = 0;
    lista *aux = *l;
    int posicao = 0;

    // Encontrar a posição na lista correspondente à linha atual e coluna desejada
    while (contador < linha_cursor && aux != NULL) {
        if (aux->caracter == '\n') {
            contador++;
        }
        aux = aux->next;
        posicao++;
    }

    // Retroceder para a linha superior
    contador = 0;
    aux = *l;
    int posicao_linha_superior = posicao - coluna_cursor;

    // Procurar a posição na linha superior
    while (contador < linha_cursor - 1 && aux != NULL) {
        if (aux->caracter == '\n') {
            contador++;
        }
        aux = aux->next;
    }

    // Encontrar o caractere na linha superior
    while (posicao < posicao_linha_superior && aux != NULL) {
        if (aux->caracter == '\n') {
            // Caso a linha acabe, a posição não existe
            return 0;
        }
        posicao++;
        aux = aux->next;
    }

    // Retornar 1 se não houver caractere na linha de cima, caso contrário, retornar 0
    if (aux == NULL || aux->caracter == '\n') {
        return 1;
    } else {
        return 0;
    }
}



