/*
 * Pilha para contador de objetos
 * 
 * Autores: Daniel Stuart e Leonardo Deldotto.
 */

#ifndef PILHA_H
#define PILHA_H
/*
 * Struct da posição de cada pixel, utilizado na pilha.
 */
struct tPos {
    int x, y;
};

/*
 * Struct da pilha, contendo seu tamanho e um ponteiro 
 * para a posição do primeiro pixel.
 */
struct tPilha {
    int tamanho;
    struct tPos *pixels;
};

struct tPilha *criarPilha(int, int);
struct tPos *get(struct tPilha *);

int pop(struct tPilha *);
int push(struct tPilha *, int, int);

void deletarPilha(struct tPilha *pilha);
#endif