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


