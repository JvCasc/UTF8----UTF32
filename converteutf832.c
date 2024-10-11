/// João Vitor Castro Cascão 2310375 3WB

#include "converteutf832.h"
#include <stdio.h>
#include <stdlib.h>

// Essa função converte caracteres UTF-8 para código Unicode

unsigned int utf8_to_unicode(unsigned char *utf8, int *num_bytes) {
    unsigned int unicode = 0;
    *num_bytes = 0;

    if ((utf8[0] & 0x80) == 0) { 
        unicode = utf8[0];
        *num_bytes = 1;

    } else if ((utf8[0] & 0xE0) == 0xC0) { 
        unicode = (utf8[0] & 0x1F) << 6 | 
        (utf8[1] & 0x3F);
        *num_bytes = 2;

    } else if ((utf8[0] & 0xF0) == 0xE0) { 
        unicode = (utf8[0] & 0x0F) << 12 | 
        (utf8[1] & 0x3F) << 6 | 
        (utf8[2] & 0x3F);
        *num_bytes = 3;

    } else if ((utf8[0] & 0xF8) == 0xF0) {
        unicode = (utf8[0] & 0x07) << 18 |
        (utf8[1] & 0x3F) << 12 |
        (utf8[2] & 0x3F) << 6 |
        (utf8[3] & 0x3F);
        *num_bytes = 4;
    }

    return unicode;
}

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida) {
    unsigned char utf8[4];
    unsigned int unicode;
    int num_bytes;

     if (!arquivo_entrada || !arquivo_saida) {
        fprintf(stderr, "Falha ao abrir os arquivos para UTF-8 para UTF-32");
        return -1;
    }

    // Escrevendo o BOM para UTF-32 Little-Endian e verificando erros 
    unsigned int bom = 0x0000FEFF;
    if (fwrite(&bom, sizeof(unsigned int), 1, arquivo_saida) != 1) {
        fprintf(stderr, "Erro ao escrever BOM no arquivo de saída.\n");
        return -1;
    }

    while (fread(utf8, 1, 1, arquivo_entrada) == 1) {
        unicode = utf8_to_unicode(utf8, &num_bytes);

        // Escrever unicode como UTF-32 Little-Endian
        if (fwrite(&unicode, 4, 1, arquivo_saida) != 1) {
            fprintf(stderr, "Erro ao escrever UTF-32 no arquivo de saída.\n");
            return -1;
        }
    }

    return 0;
}

