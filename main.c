#include <stdio.h>
#include <string.h>

int main() {
    int largura, altura;
    char c, file[100];
    FILE *ppm;
    printf("Contador de Objetos PPM\n");
    printf("  Por: Daniel Stuart\n\n");

    do {
        printf("Digite o nome do arquivo: ");
        fgets(file, 100, stdin);
        file[strlen(file) - 1] = '\0';

        ppm = fopen(file, "r");
        if (ppm == NULL)
            printf("Erro ao abrir arquivo! Tente novamente...\n\n");
        
    } while (ppm == NULL);

    c = fgetc(ppm);
    while (c == ' ')
        c = fgetc(ppm);

    if (!(c == 'P' && fgetc(ppm) == '6')) {
        printf("\nErro no formato do arquivo!\n");
        return -1;
    }

    c = fgetc(ppm);
    while (c == ' ' || c == '\n')
        c = fgetc(ppm);

    if (c < 49 || c > 57) {
        printf("\nErro no formato do arquivo!\n");
        return -1;
    }
    largura = c - 48;

    c = fgetc(ppm);
    while (c == ' ' || c == '\n')
        c = fgetc(ppm);
    
    if (c < 49 || c > 57) {
        printf("\nErro no formato do arquivo!\n");
        return -1;
    }
    altura = c - 48;
}