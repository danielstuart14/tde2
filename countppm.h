#ifndef COUNTPPM_H
#define COUNTPPM_H

#include <stdio.h>

/*
 * Estado de cada pixel da imagem.
 * OBS: Devido o uso de calloc, o estado inicial é sempre zero.
 */
#define FUNDO 1
#define LENDO 2
#define LIDO 3

/*
 * Delay entre cada pixel (com tipo igual a zero) lido, em milisegundos.
 * OBS: Apenas quando o processo é mostrado na tela.
 */
#define DELAY 50

/*
 * Struct para cada pixel na imagem, contendo valores RGB
 * e o tipo de pixel (conforme macros acima).
 */
struct PixelInfo {
    int r, g, b;
    int tipo;
};

/*
 * Struct da imagem, contendo suas propriedades e um ponteiro 
 * para o primeiro pixel.
 */
struct ImageInfo {
    int largura, altura, intensidade;
    struct PixelInfo *inicio;
};

/*
 * Funções da biblioteca 
 */
FILE *readFile();

struct PixelInfo *getPixel(struct ImageInfo *, int, int);

int contarObjetos(struct ImageInfo *, int);
int getNumber(FILE *, int *);
int power(int, int);

void abrir(struct ImageInfo *, int, int, int);
void image2Struct (FILE *, struct PixelInfo *);
void cor(int, int, int, int);
void printImage(struct ImageInfo *);

#endif