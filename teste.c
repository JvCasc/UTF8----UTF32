#include "converteutf832.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *entradaUtf8, *saidaUtf32;
    FILE *entradaUtf32, *saidaUtf8;

    // Abrindo arquivos para teste da função convUtf8p32
    entradaUtf8 = fopen("utf8_peq.txt", "rb");  // Substitua pelo caminho correto do arquivo de entrada UTF-8
    saidaUtf32 = fopen("utf32_saida.txt", "wb");  // Substitua pelo caminho correto do arquivo de saída UTF-32

    // Chamando a função convUtf8p32
    if (convUtf8p32(entradaUtf8, saidaUtf32) != 0) {
        fprintf(stderr, "Falha na conversão de UTF-8 para UTF-32\n");
    }

    // Fechando os arquivos
    fclose(entradaUtf8);
    fclose(saidaUtf32);

    /* 
    entradaUtf32 = fopen("utf32_peq.txt", "rb");  // Reutilizando o arquivo de saída como entrada
    saidaUtf8 = fopen("utf8_saida.txt", "wb");  // Substitua pelo caminho correto do arquivo de saída UTF-8
    if (!entradaUtf32 || !saidaUtf8) {
        fprintf(stderr, "Falha ao abrir os arquivos para UTF-32 para UTF-8");
        return -1;
    }

    // Chamando a função convUtf32p8
    if (convUtf32p8(entradaUtf32, saidaUtf8) != 0) {
        fprintf(stderr, "Falha na conversão de UTF-32 para UTF-8\n");
    }

    // Fechando os arquivos
    fclose(entradaUtf32);
    fclose(saidaUtf8);

    */

    return 0;
}
