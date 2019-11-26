/*
 * Contador de objetos em imagens PPM.
 * 
 * Autor: Daniel Stuart <daniel.stuart@pucpr.edu.br>
 */
#include "countppm.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    int ret = 0, objs;
    char input = 0;
    FILE *ppm;
    struct ImageInfo imagem;
    printf("Contador de Objetos PPM\n");
    printf("  Por: Daniel Stuart\n\n");
    
    //Pergunta o arquivo e o lê
    ppm = readFile();
    if (ppm == NULL)
        return -1; //erro

    //Extrai as informações da imagem através de caracteres ASCII
    ret += getNumber(ppm, &imagem.largura);
    ret += getNumber(ppm, &imagem.altura);
    ret += getNumber(ppm, &imagem.intensidade);
    if (ret) {
        printf("\nErro no formato do arquivo!\n");
        return -1; //erro
    }

    printf("\n%d x %d - %d\n", imagem.largura, imagem.altura, imagem.intensidade); //Mostra as informações na tela
    imagem.inicio = (struct PixelInfo *) calloc(imagem.altura * imagem.largura, sizeof(struct PixelInfo)); //aloca o espaço para o vetor
    if (imagem.inicio == NULL) {
        printf("\nErro no alocamento!\n");
        return -1; //erro
    }

    //Transforma a imagem em um vetor de struct de pixels
    image2Struct(ppm, imagem.inicio);
    
    //Pergunta se deseja mostrar a imagem caso ela caiba na tela
    if (imagem.largura <= 96 && imagem.altura <= 48) {
        printf("\nDeseja exibir a imagem [s/n]? ");
        scanf(" %c", &input);
    }
    
    //Conta os objetos e mostra a imagem sendo varrida dependendo da resposta do usuário
    objs = contarObjetos(&imagem, input == 's' || input == 'S');
    
    //Mostra a quantidade de objetos e encerra
    printf("\nObjetos: %d\n", objs);
    free(imagem.inicio); //Livra a memória
    fclose(ppm); //Fecha o arquivo
    
    return 0;
}