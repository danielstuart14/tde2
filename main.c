#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FUNDO 1
#define LIDO 2
#define MOSTRAFUNDO 0

struct PixelInfo {
    unsigned int r, g, b;
    int tipo;
};

struct ImageInfo {
    unsigned int largura, altura, intensidade;
    struct PixelInfo *inicio;
};

struct tPos {
    int x, y;
};

struct tPilha {
    int tamanho;
    struct tPos *pixels;
};

struct tPilha *criarPilha(int, int);
int pop(struct tPilha *);
int push(struct tPilha *, int, int);
struct tPos *get(struct tPilha *);
void deletarPilha(struct tPilha *pilha);

int contarObjetos(struct ImageInfo *);
void abrir(struct ImageInfo *, int, int);

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
    struct PixelInfo *pixels[8];
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

    printf("\n%d x %d - %d\n", imagem.largura, imagem.altura, imagem.intensidade);
    imagem.inicio = (struct PixelInfo *) calloc(imagem.altura * imagem.largura, sizeof(struct PixelInfo));
    if (imagem.inicio == NULL) {
        printf("\nErro no alocamento!\n");
        return -1;
    }

    image2Struct(ppm, imagem.inicio);
    //printImage(&imagem);
    printf("Objetos: %d\n", contarObjetos(&imagem));

    free(imagem.inicio);
    return 0;
}

struct tPilha *criarPilha(int x, int y) {
    struct tPilha *ret = malloc(sizeof(struct tPilha));
    ret->tamanho = 1;
    ret->pixels = (struct tPos *) malloc(sizeof(struct tPos));
    if (ret->pixels == NULL)
        return NULL;
    
    ret->pixels->x = x;
    ret->pixels->y = y;
    return ret;
}

int pop(struct tPilha *pilha) {
    if (pilha->tamanho == 0)
        return -1;

    pilha->tamanho--;
    if (pilha->tamanho == 0) {
        pilha->pixels->x = 0;
        pilha->pixels->y = 0;
        return 0;
    }
    pilha->pixels = (struct tPos *) realloc(pilha->pixels, sizeof(struct tPos)*pilha->tamanho);
    if (pilha->pixels == NULL)
        return -1;

    return 0;
}

int push(struct tPilha *pilha, int x, int y) {
    pilha->tamanho++;
    if (pilha->tamanho != 1) {
        pilha->pixels = (struct tPos *) realloc(pilha->pixels, sizeof(struct tPos)*pilha->tamanho);
        if (pilha->pixels == NULL)
            return -1;
    }
    
    (pilha->pixels + (pilha->tamanho - 1))->x = x;
    (pilha->pixels + (pilha->tamanho - 1))->y = y;
    return 0;
}

struct tPos *get(struct tPilha *pilha) {
    return pilha->pixels + (pilha->tamanho - 1);
}

void deletarPilha(struct tPilha *pilha) {
    free(pilha->pixels);
    free(pilha);
}

int contarObjetos(struct ImageInfo *imagem) {
    struct PixelInfo *pixel = imagem->inicio;
    int i, j, obj=0;
    for (i = 0; i < imagem->altura; i++) {
        for (j = 0; j < imagem->largura; j++) {
            if (getPixel(imagem, j, i)->tipo == 0) {
                //printf("abrindo para %d %d\n", j, i);
                abrir(imagem, j, i);
                obj++;
            }
        }
    }
    
    return obj;
}

void abrir(struct ImageInfo *imagem, int x, int y) {
    int i, j;
    struct PixelInfo *pixel;
    struct tPilha *pilha;
    struct tPos *pos;
    pilha = criarPilha(x, y);
    
    while (pilha->tamanho > 0) {
        pos = get(pilha);
        x = pos->x;
        y = pos->y;
        pop(pilha);
        
        for (i = (y - 1); i <=  (y + 1); i++) {
            if (i >= 0 && i < imagem->altura) {
                pixel = getPixel(imagem, x, i);
                if (pixel->tipo == 0) {
                    pixel->tipo = 2;
                    push(pilha, x, i);
                }
            }
        }

        for (j = (x - 1); j <= (x + 1); j++) {
            if (j >= 0 && j < imagem->largura) {
                pixel = getPixel(imagem, j, y);
                if (pixel->tipo == 0) {
                    pixel->tipo = 2;
                    push(pilha, j, y);
                }
            }
        }

    }

    deletarPilha(pilha);
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
    struct PixelInfo *pixel = first;
    unsigned int c = fgetc(ppm);

    pixel->r = c;
    c = fgetc(ppm);
    pixel->g = c;
    c = fgetc(ppm);
    pixel->b = c;
    c = fgetc(ppm);
    pixel->tipo = FUNDO;
    pixel++;

    while (c != EOF) {
        pixel->r = c;
        c = fgetc(ppm);
        pixel->g = c;
        c = fgetc(ppm);
        pixel->b = c;
        c = fgetc(ppm);
        if (pixel->r == first->r && pixel->g == first->g && pixel->b == first->b)
            pixel->tipo = FUNDO;

        pixel++;
    }
}

struct PixelInfo *getPixel(struct ImageInfo *image, int x, int y) {
    long inc = x + (image->largura)*y;
    return image->inicio + inc;
};

void printImage(struct ImageInfo *image) {
    struct PixelInfo *pixel = image->inicio;
    int i;
    for (i = 1; i <= image->altura * image->largura; i++) {
        if (pixel->tipo != FUNDO || MOSTRAFUNDO) {
#ifdef __unix__
            if (pixel->r > (pixel->g + pixel->b)/2) {
                if (pixel->g > pixel->r/2) {
                    printf("\033[0;33m\u25A0\033[0m");
                } else if (pixel->b > pixel->r/2) {
                    printf("\033[0;35m\u25A0\033[0m");
                } else {
                    printf("\033[0;31m\u25A0\033[0m");
                }
            } else if (pixel->g > (pixel->r + pixel->b)/2) {
                if (pixel->b > pixel->g/2) {
                    printf("\033[0;36m\u25A0\033[0m");
                } else {
                    printf("\033[0;32m\u25A0\033[0m");
                }
            } else if (pixel->b > (pixel->g + pixel->r)/2) {
                printf("\033[0;34m\u25A0\033[0m");
            } else {
                if (pixel->r > image->intensidade/2) {
                    printf("\033[0;37m\u25A0\033[0m");
                } else {
                    printf("\033[0;30m\u25A0\033[0m");
                }
            }
#else
            if ((pixel->r + pixel->g + pixel->b) > image->intensidade/2) {
                printf("#");
            } else {
                printf(" ");
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
    return 0;
}

int power(int base, int exponent) {
    int i, ret = 1;
    for (i = 0; i < exponent; i++) {
        ret *= base;
    }

    return ret;
}