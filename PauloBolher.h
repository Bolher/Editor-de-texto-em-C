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

int validacao_enter(lista **l, int *posicao_cursor, int *coluna_cursor){

    int j, i, contador = 0;
    lista *aux = *l;

    if(*coluna_cursor == 0){

        for(j =0; j< *posicao_cursor; j++){
            aux = aux->next;
        }

    }else{

        while(contador != *coluna_cursor){

            if(aux->caracter == '\n' || aux->caracter == 10){
                contador++;
            }

            aux = aux->next;

        }

        for(i = 0; i< *posicao_cursor; i++){
            aux = aux->next;
        }
    }

    if(aux->next == NULL){
        return 0;
    }
    return 1;

}

///verifica se tem um caracter qualquer a frente do cursor para pdoer movimentar ele
int verificar_andar_cursor_direita(lista **l, int *posicao_cursor, int *coluna_cursor){

    int j, i, contador = 0;
    lista *aux = *l;

    ///verifica se está na primeira linha ai percorre até a coluna do cursor ve se o ponteiro "next" do elemento onde está o cursor for != de NULL se for pode andar
    ///pq tem caracter a direita
    if(*coluna_cursor == 0){

        for(j =0; j< *posicao_cursor; j++){
            aux = aux->next;
        }

    }else{

        ///se não estiver na primeira linha percorre até a linha do cursor contando os "\n" e depois faz a mesma coisa de ver o ponteiro "next"
        while(contador != *coluna_cursor){

            if(aux->caracter == '\n' || aux->caracter == 10){
                contador++;
            }

            aux = aux->next;

        }

        for(i = 0; i< *posicao_cursor; i++){
            aux = aux->next;
        }
    }

    if(aux == NULL || aux->caracter == '\n'){
        return 0;
    }
    return 1;
}

///verifica se possui algum caracter a esquerda do cursor para poder movelo
int verificar_andar_cursor_esquerda(lista **l, int *posicao_cursor, int *coluna_cursor){
    int j, i, contador = 0;
    lista *aux = *l;
    lista *anterior = NULL;

    ///se o cursor estiver na primeira linha ele percorre até percorre até a coluna onde o cursor está se a variavel "anterior" que a posição anteriro a o cursor for nulo quer
    ///dizer que não pode andar apra esqueda posi não há caracter lá
    if(*coluna_cursor == 0){
        for(j =0; j<*posicao_cursor; j++){
            anterior = aux;
            aux = aux->next;
        }

        if(anterior == NULL){
            return 0;
        }
        return 1;


    }else{

        ///caso não esteja na primeira linha percorre até a linha do cursosr contando a quantidade de "\n"
        while(contador != *coluna_cursor){

            if(aux->caracter == '\n' || aux->caracter == 10){
                contador++;
            }
            aux = aux->next;

        }

        /// realiza a mesma coisa que o for de quando o cursor está na primeira linha
        for(i = 0; i< *posicao_cursor; i++){
            anterior = aux;
            aux = aux->next;
        }
    }

    if(anterior == NULL){
        return 0;
    }

    return 1;

}

///verifica se tem um caracter a baixo de onde o cursor está para poder andar
int verificar_caractere_baixo(lista **l, int *linha_cursor, int *coluna_cursor){

    int contador = 0, coluna_linha_inferior = 0;
    lista *linha_inferior = *l;

    ///percorre até a linha de baixo do cursor faz isso contando a contidade de '\n'
    while (contador <= *linha_cursor && linha_inferior != NULL) {
        if (linha_inferior->caracter == '\n') {
            contador++;
        }
        linha_inferior = linha_inferior->next;
    }

    ///verifica se na linah de baixo possui alguma coisa
    if (linha_inferior == NULL) {
        return -1;
    }

    ///percorre até a coluna correta da linha de baixo ou para antes se não tiver cacarcer na coluna da linah de baixo
    while (linha_inferior != NULL &&  linha_inferior->caracter != '\n' && coluna_linha_inferior <= *coluna_cursor) {
        if (linha_inferior->caracter != '\n') {
            coluna_linha_inferior++;
        }
        linha_inferior = linha_inferior->next;
    }
    //coluna_linha_inferior--;
    //move((*linha_cursor) + 1, coluna_linha_inferior);
    //coluna_linha_inferior++;
    return coluna_linha_inferior;
}

int contador_coluna(lista **l, int linha_cursor, int coluna_cursor){
    lista *aux = *l;
    int i, contador = 0, posicao = 0;

    ///se tiver apenas uma linha ele percorre até a coluna onde está o cursor
    if(linha_cursor == 0){
        for(i = 0; i<coluna_cursor; i++){
            posicao ++;
        }

        return posicao;
    }

    ///caso tenha mais de uma linha ele percorre até a linah onde está o cursor faz isso contando a quantidade de '\n'
    while(contador <= (linha_cursor) - 1 && aux != NULL){
        if(aux->caracter == '\n'){
            contador ++;
        }
        aux = aux->next;
    }

    ///percorre até a coluna onde o cursor está
    for(i = 0; i< coluna_cursor && aux != NULL; i++){
        contador++;
        posicao++;
    }

    return posicao;
}

///saber qual a posição que o cursor da lista o cursor está
int saber_posicao_cursor(lista **l, int *linha_cursor, int *coluna_cursor){
    lista *aux = *l;
    int i, contador = 0, posicao = 0;

    ///se tiver apenas uma linha ele percorre até a coluna onde está o cursor
    if(*linha_cursor == 0){
        for(i = 0; i<*coluna_cursor; i++){
            posicao ++;
        }

        return posicao;
    }

    ///caso tenha mais de uma linha ele percorre até a linah onde está o cursor faz isso contando a quantidade de '\n'
    while(contador <= (*linha_cursor) - 1 && aux != NULL){
        if(aux->caracter == '\n'){
            contador ++;
        }
        posicao++;
        aux = aux->next;
    }

    ///percorre até a coluna onde o cursor está
    for(i = 0; i< *coluna_cursor && aux != NULL; i++){
        contador++;
        posicao++;
    }

    return posicao;
}

void remover_posicao(lista **l, int posicao) {
    lista *aux = *l;
    lista *proximo;
    int contador = 0;

    if (*l == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    if (posicao == 0) {
        *l = aux->next;
        free(aux);
        return;
    }

    while (aux != NULL && contador < posicao - 1) {
        aux = aux->next;
        contador++;
    }

    if (aux == NULL || aux->next == NULL) {
        printf("Posição inválida.\n");
        return;
    }

    proximo = aux->next->next;
    free(aux->next);
    aux->next = proximo;
}


void inserir_posicao(lista **l, int *posicao, char caracter) {
    int contador = 0;
    lista *novo = getnode();
    lista *aux = *l;

    if (novo == NULL) {
        printf("Erro: falha na alocação de memória.\n");
        exit(1);
    }
    novo->caracter = caracter;

    if (*posicao == 0 || *l == NULL) {
        novo->next = *l;
        *l = novo;
        return;
    }

    while (contador < *posicao - 1 && aux->next != NULL) {
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
int verificar_caractere_acima(lista **l, int *linha_cursor, int *coluna_cursor) {
    int coluna_linha_superior = 0;
    int contador = 0;
    lista *linha_atual = *l;
    lista *linha_superior = *l;

    if (*linha_cursor != 0) {

        // Procura a linha acima do cursor
        while (contador < (*linha_cursor) - 1 && linha_superior != NULL) {
            if (linha_superior->caracter == '\n') {
                contador++;
            }
            linha_superior = linha_superior->next;
        }

        if (linha_superior == NULL || linha_superior->caracter == '\n') {

            return coluna_linha_superior;
        }

        // Movimenta para a posição onde o cursor deve estar na linha acima
        while (linha_superior != NULL && linha_superior->caracter != '\n' && coluna_linha_superior <= *coluna_cursor) {
            if (linha_superior->caracter != '\n') {
                coluna_linha_superior++;
            }
            linha_superior = linha_superior->next;
        }

        return coluna_linha_superior;
    }


    return -1;
}



///descobre onde é o final da linha e retorna a posção para poder mover o cursor na função da tecla "End"
int verificar_final_linha(lista **l, int linha_cursor, int coluna_cursor){
    lista *aux = *l;

    int i, contador = 0, posicao = 0;


    ///se for a priemira linha ele apenas conta a quantidade de caracters e depois move o cursor para essa contidade levando assi ma o fianl da linha
    if(linha_cursor == 0){

        while(aux != NULL){
            posicao++;
            aux = aux->next;
        }

        return posicao;
    }
    ///se não for a primeira linha ele percorre até a linha do cursor contado os "\n"
    while(contador <= linha_cursor - 1 && aux != NULL){
        if(aux->caracter == '\n'){
            contador ++;
        }
        aux = aux->next;
    }

    ///ai vai contando os caracter da linha até o final para mover o cursor para essa posição
    while(aux->caracter != '\n' && aux->next != NULL){
        posicao++;
        aux = aux->next;
    }

    return posicao;
}

int saber_caraceter_posicao(lista **l, int linha, int coluna){
    int i, contador = 0;
    lista *aux = *l;

    if(aux == NULL){
        return 0;

    }


        while(contador <= linha - 1 && aux != NULL){
            if(aux->caracter == '\n'){
                contador ++;
            }
            aux = aux->next;
        }

        for(i = 0; i< coluna && aux != NULL; i++){
            contador++;
            aux = aux->next;
        }

        if(aux != NULL && aux->caracter == '\n'){
            //printw("retornei 1");
            return 1;
        }

           // printw("retornei 0");
            return 0;




}

int conta_linha_branco(lista **l, int linha_cursor, int coluna_cursor){
    int contador = 0;
    int total_linha_branco = 0;
    lista *aux = *l;
    char anterior = 'a';

    while(contador < linha_cursor - 1){
        if(aux->caracter == '\n'){
            contador++;
            if(anterior == '\n'){
                total_linha_branco++;
            }
        }
        anterior = aux->caracter;
        aux = aux->next;
    }

    return total_linha_branco;
}
