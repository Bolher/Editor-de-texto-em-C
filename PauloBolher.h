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



