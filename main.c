#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct PixelInfo {
    unsigned int r, g, b;
    int lido, fundo;
};

struct ImageInfo {
    unsigned int largura, altura, intensidade;
    struct PixelInfo *inicio;
};

FILE *readFile();
void image2Struct (FILE *, struct PixelInfo *);
struct PixelInfo *getPixel(struct ImageInfo *, int, int);
void printImage(struct ImageInfo *);
int getNumber(FILE *, unsigned int *);
int power(int, int);

int main() {
    int ret = 0;
    FILE *ppm;
    struct ImageInfo imagem;
    printf("Contador de Objetos PPM\n");
    printf("  Por: Daniel Stuart\n\n");

    ppm = readFile();
    if (ppm == NULL)
        return -1;

    ret += getNumber(ppm, &imagem.largura);
    ret += getNumber(ppm, &imagem.altura);
    ret += getNumber(ppm, &imagem.intensidade);
    if (ret) {
        printf("\nErro no formato do arquivo!\n");
        return -1;
    }

    printf("%d x %d - %d\n", imagem.largura, imagem.altura, imagem.intensidade);
    imagem.inicio = calloc(imagem.altura * imagem.largura, sizeof(struct PixelInfo));
    if (imagem.inicio == NULL) {
        printf("\nErro no alocamento!\n");
        return -1;
    }

    image2Struct(ppm, imagem.inicio);
    printImage(&imagem);

    return 0;
}

FILE *readFile() {
    FILE *ret;
    char file[100];
    do {
        printf("Digite o nome do arquivo: ");
        fgets(file, 100, stdin);
        file[strlen(file) - 1] = '\0';

        ret = fopen(file, "r");
        if (ret == NULL)
            printf("Erro ao abrir arquivo! Tente novamente...\n\n");
        
    } while (ret == NULL);

    if (!(fgetc(ret) == 'P' && fgetc(ret) == '6')) {
        printf("\nErro no formato do arquivo!\n");
        return NULL;
    }

    return ret;
}

void image2Struct (FILE *ppm, struct PixelInfo *first) {
    struct PixelInfo *pixels = first;
    unsigned int c = fgetc(ppm);
    pixels->r = c;
    c = fgetc(ppm);
    pixels->g = c;
    c = fgetc(ppm);
    pixels->b = c;
    c = fgetc(ppm);
    pixels->fundo = 1;
    pixels++;

    while (c != EOF) {
        pixels->r = c;
        c = fgetc(ppm);
        pixels->g = c;
        c = fgetc(ppm);
        pixels->b = c;
        c = fgetc(ppm);
        if (pixels->r == first->r && pixels->g == first->g && pixels->b == first->b)
            pixels->fundo = 1;

        pixels++;
    }
}

struct PixelInfo *getPixel(struct ImageInfo *image, int x, int y) {
    unsigned long inc = sizeof(struct PixelInfo)*(x + image->largura*y);
    return ((unsigned long) image->inicio + inc);
};

void printImage(struct ImageInfo *image) {
    struct PixelInfo *pixels = image->inicio;
    int i;
    for (i = 1; i <= image->altura * image->largura; i++) {
#ifdef __unix__
        if (pixels->r > (pixels->g + pixels->b)/2) {
            if (pixels->g > pixels->r/2) {
                printf("\033[0;33m");
                printf("%c", '#');
                printf("\033[0m");
            } else if (pixels->b > pixels->r/2) {
                printf("\033[0;35m");
                printf("%c", '#');
                printf("\033[0m");
            } else {
                printf("\033[0;31m");
                printf("%c", '#');
                printf("\033[0m");
            }
        } else if (pixels->g > (pixels->r + pixels->b)/2) {
            if (pixels->b > pixels->g/2) {
                printf("\033[0;36m");
                printf("%c", '#');
                printf("\033[0m");
            } else {
                printf("\033[0;32m");
                printf("%c", '#');
                printf("\033[0m");
            }
        } else if (pixels->b > (pixels->g + pixels->r)/2) {
            printf("\033[0;34m");
            printf("%c", '#');
            printf("\033[0m");
        } else {
            if (pixels->r > image->intensidade/2) {
                printf("%c", '#');
            } else {
                printf("%c", ' ');
            }
        }
#else
        if ((pixels->r + pixels->g + pixels->b) > image->intensidade/2) {
            printf("%c", '#');
        } else {
            printf("%c", ' ');
        }
#endif
        if (!(i % image->largura))
            printf("\n");
        pixels++;
    }
}

int getNumber(FILE *file, unsigned int *val) {
    unsigned int i, ret=0, c = fgetc(file);
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
    return;
}

int power(int base, int exponent) {
    int i, ret = 1;
    for (i = 0; i < exponent; i++) {
        ret *= base;
    }

    return ret;
}