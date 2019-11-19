#include "countppm.h"
#include "pilha.h"

#include <string.h>
#include <stdlib.h>

#ifdef __unix__
#include <unistd.h>
#else
#include <windows.h>
#endif

/*
 * Conta a quantidade de objetos na imagem
 */
int contarObjetos(struct ImageInfo *imagem, int mostrar) {
    int i, j, obj=0;
    
    //Varre todas as posições da imagem
    for (i = 0; i < imagem->altura; i++) {
        for (j = 0; j < imagem->largura; j++) {
            if (!getPixel(imagem, j, i)->tipo) { //objeto é do tipo 0
                abrir(imagem, j, i, mostrar); //abre as casas ao redor
                obj++; //aumenta a quantidade de objetos
            }
        }
    }
    
    return obj; //retorna quantidade de objetos
}

/*
 * Abre as casas com pixel do tipo "0" ao redor de 
 * um determinado pixel
 */
void abrir(struct ImageInfo *imagem, int x, int y, int mostrar) {
    int i;
    struct PixelInfo *pixel;
    struct tPilha *pilha;
    struct tPos *pos;
    pilha = criarPilha(x, y); //cria a pilha com X e Y iniciais
    
    while (pilha->tamanho > 0) { //enquantou houver algo na pilha
        pos = get(pilha); //obtém o ponteiro para a struct de posição do pixel
        x = pos->x;
        y = pos->y;
        pop(pilha); //remove o pixel da pilha
        for (i = -1; i <=  1; i++) {

            if ((y + i) >= 0 && (y + i) < imagem->altura) { //caso pixel adjacente esteja dentro da imagem
                pixel = getPixel(imagem, x, (y + i)); //obtém dados do pixel
                if (!pixel->tipo) { //caso tipo do pixel seja diferente de 0
                    pixel->tipo = LENDO; //utilizado apenas para exibição da imagem
                    push(pilha, x, (y + i)); //adiciona pixel na pilha
                    if (mostrar) {
                        printImage(imagem); //mostra a imagem na tela com o pixel atualmente sendo verificado
#ifdef __unix__
                        usleep(DELAY * power(10, 3));
#else
                        Sleep(DELAY);
#endif
                    }
                    pixel->tipo = LIDO; //define o pixel como tendo sido lido
                }
            }

            if ((x + i) >= 0 && (x + i) < imagem->largura) { //igual o código acima, mas para os pixels na horizontal
                pixel = getPixel(imagem, (x + i), y);
                if (!pixel->tipo) {
                    pixel->tipo = LENDO;
                    push(pilha, (x + i), y);
                    if (mostrar) {
                        printImage(imagem);
#ifdef __unix__
                        usleep(DELAY * power(10, 3));
#else
                        Sleep(DELAY);
#endif
                    }
                    pixel->tipo = LIDO;
                }
            }

        }

    }

    deletarPilha(pilha); //livra a memória após o uso
}

/*
 * Pergunta o arquivo e lê o mesmo
 */
FILE *readFile() {
    FILE *ret;
    char file[100];
    do {
        printf("Digite o nome do arquivo: ");
        fgets(file, 100, stdin); //obtém o nome do arquivo
        file[strlen(file) - 1] = '\0'; //remove o enter do final da string

        ret = fopen(file, "r"); //abre o arquivo no modo leitura
        if (ret == NULL) //arquivo não encontrado
            printf("Erro ao abrir arquivo! Tente novamente...\n\n");
        
    } while (ret == NULL); //pergunta o nome do arquivo até encontrar um válido

    if (!(fgetc(ret) == 'P' && fgetc(ret) == '6')) { //programa suporta apenas ppm do tipo RGB binário
        printf("\nErro no formato do arquivo!\n");
        return NULL;
    }

    return ret; //retorna arquivo aberto
}

/*
 * Transforma os valores RGB binários dos pixels em
 * um vetor de structs
 */
void image2Struct (FILE *ppm, struct PixelInfo *first) {
    struct PixelInfo *pixel = first;
    int c = fgetc(ppm); //obtém próximo caracter

    pixel->r = c; //intensidade do subpixel vermelho
    c = fgetc(ppm);
    pixel->g = c; //intensidade do subpixel verde
    c = fgetc(ppm);
    pixel->b = c; //intensidade do subpixel azul
    c = fgetc(ppm);
    pixel->tipo = FUNDO; //define tipo como fundo
    pixel++; //próximo ponteiro na lista

    while (c != EOF) { //enquanto o caractér não for do tipo End Of File.
        pixel->r = c;
        c = fgetc(ppm);
        pixel->g = c;
        c = fgetc(ppm);
        pixel->b = c;
        c = fgetc(ppm);
        if (pixel->r == first->r && pixel->g == first->g && pixel->b == first->b)
            pixel->tipo = FUNDO; //caso seja igual o primeiro pixel, então é fundo.

        pixel++;
    }
}

/*
 * Retorna o pixel na posição X e Y
 */
struct PixelInfo *getPixel(struct ImageInfo *image, int x, int y) {
    long inc = x + (image->largura)*y; //incremento é igual a posição em X + a largura da imagem vezes y.
    return image->inicio + inc; //retorna um ponteiro para o pixel em X e Y, através da adição do incremento ao ponteiro inicial da imagem
};

/*
 * Define uma cor conforme valores RGB do pixel.
 * OBS: Suporta apenas 8 cores.
 */
void cor(int r, int g, int b, int intensidade) {
    if (r > (g + b)/2) {
        if (g > r/2) {
            printf("\033[0;33m");
        } else if (b > r/2) {
            printf("\033[0;35m");
        } else {
            printf("\033[0;31m");
        }
    } else if (g > (r + b)/2) {
        if (b > g/2) {
            printf("\033[0;36m");
        } else {
            printf("\033[0;32m");
        }
    } else if (b > (g + r)/2) {
        printf("\033[0;34m");
    } else {
        if (r > intensidade/2) {
            printf("\033[0;37m");
        } else {
            printf("\033[0;30m");
        }
    }
}

/*
 * Mostra a imagem na tela do usuário
 */
void printImage(struct ImageInfo *image) {
    struct PixelInfo *pixel = image->inicio;
    int i;

#ifdef __unix__
    system("clear");
#else
    system("cls");
#endif

    printf("\n%d x %d - %d\n\n", image->largura, image->altura, image->intensidade);
    for (i = 1; i <= image->altura * image->largura; i++) {
        if (pixel->tipo != FUNDO) {
#if __unix__
            if (pixel->tipo != LENDO) {
                cor(pixel->r, pixel->g, pixel->b, image->intensidade);
            } else {
                cor(image->intensidade - pixel->r, image->intensidade - pixel->g, image->intensidade - pixel->b, image->intensidade);
            }
            printf("\u25A0\033[0m");
#else
            if (pixel->tipo != LENDO) {
                printf("#");
            } else {
                printf("X");
            }
#endif
        } else {
            printf(" ");
        }

        if (!(i % image->largura))
            printf("\n");

        pixel++;
    }
}

/*
 * Obtém um número de um arquivo texto
 */
int getNumber(FILE *file, int *val) {
    int i, ret=0, c = fgetc(file);
    while (c == ' ' || c == '\n') {
        c = fgetc(file);
        if (c == EOF)
            return -2;
    }

    for (i=0; i < 4; i++) { 
        if (c == ' ' || c == '\n' || c == EOF) {
            *val = ret / power(10, (4 - i));
            if (c == EOF)
                return -1;
            return 0;
        }
        c -= 48;
        ret += c * power(10, 3) / power(10, i);
        c = fgetc(file);
    }

    *val = ret;
    return 0;
}

/*
 * Retorna o resultado da exponencial entre os parâmetros
 * OBS: Apenas expoentes positivos
 */
int power(int base, int exponent) {
    int i, ret = 1;
    for (i = 0; i < exponent; i++) {
        ret *= base;
    }

    return ret;
}
