/*
 * Pilha para contador de objetos
 * 
 * Autores: Daniel Stuart e Leonardo Deldotto.
 */

#include <stdlib.h>
#include "pilha.h"

/*
 * Cria uma pilha com os valores iniciais X e Y
 */
struct tPilha *criarPilha(int x, int y) {
    struct tPilha *ret = (struct tPilha *) malloc(sizeof(struct tPilha)); //aloca a pilha
    ret->tamanho = 1;
    ret->pixels = (struct tPos *) malloc(sizeof(struct tPos)); //aloca o vetor de posições
    if (ret->pixels == NULL)
        return NULL; //erro
    
    ret->pixels->x = x;
    ret->pixels->y = y;
    return ret;
}

/*
 * Obtém o ultimo dado da pilha
 */
struct tPos *get(struct tPilha *pilha) {
    return pilha->pixels + (pilha->tamanho - 1);
}


/*
 * Remove o último dado da pilha
 */
int pop(struct tPilha *pilha) {
    if (!pilha->tamanho) //tamanho é zero
        return -1; //erro

    pilha->tamanho--; //diminui o tamanho da pilha
    
    //Não deleta a pilha caso não haja mais dados, apenas zera a mesma.
    if (!pilha->tamanho) {
        pilha->pixels->x = 0;
        pilha->pixels->y = 0;
        
        return 0; //retorna sem erros
    }
    
    pilha->pixels = (struct tPos *) realloc(pilha->pixels, sizeof(struct tPos)*pilha->tamanho); //diminui o tamanho do vetor de posições
    if (pilha->pixels == NULL)
        return -1; //erro

    return 0;
}

/*
 * Adiciona um dado na pilha
 */
int push(struct tPilha *pilha, int x, int y) {
    pilha->tamanho++; //aumenta tamanho da pilha
    if (pilha->tamanho != 1) { //caso já haja dados na pilha
        pilha->pixels = (struct tPos *) realloc(pilha->pixels, sizeof(struct tPos)*pilha->tamanho); //aumenta tamanho do vetor de posições
        
        if (pilha->pixels == NULL)
            return -1; //erro
    }
    
    (pilha->pixels + (pilha->tamanho - 1))->x = x;
    (pilha->pixels + (pilha->tamanho - 1))->y = y;
    
    return 0;
}

/*
 * Deleta a pilha, livrando a memória
 */
void deletarPilha(struct tPilha *pilha) {
    free(pilha->pixels);
    free(pilha);
}