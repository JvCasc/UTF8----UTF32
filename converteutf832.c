/// João Vitor Castro Cascão 2310375 3WB

#include "converteutf832.h"
#include <stdio.h>
#include <stdlib.h>

// Essa função converte caracteres UTF-8 para código Unicode e as operacoes de deslocamento de bits 
// e juncoes permite que a gente forme o codigo unicode  

unsigned int utf8_to_unicode(unsigned char *utf8, int *num_bytes) {
    unsigned int unicode = 0;
    *num_bytes = 0;

    if ((utf8[0] & 0x80) == 0) {   /// Verifica se o bit mais significativo do primeiro byte é 0
        unicode = utf8[0];
        *num_bytes = 1;  

    } else if ((utf8[0] & 0xE0) == 0xC0) {  /// Verifica se os três bits mais significativos do primeiro byte é 110
        unicode = (utf8[0] & 0x1F) << 6 | 
        (utf8[1] & 0x3F);
        *num_bytes = 2;

    } else if ((utf8[0] & 0xF0) == 0xE0) {  /// Verifica se os quatro bits mais significativos é 1110
        unicode = (utf8[0] & 0x0F) << 12 | 
        (utf8[1] & 0x3F) << 6 | 
        (utf8[2] & 0x3F);
        *num_bytes = 3;

    } else if ((utf8[0] & 0xF8) == 0xF0) { /// Verifica se os cinco bits mais significativos é 11110
        unicode = (utf8[0] & 0x07) << 18 |
        (utf8[1] & 0x3F) << 12 |
        (utf8[2] & 0x3F) << 6 |
        (utf8[3] & 0x3F);
        *num_bytes = 4;
    }

    return unicode;
}


// Converte UTF8 para UTF32 
int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida) {
    unsigned char utf8[4];
    unsigned int unicode;
    int num_bytes;

    // Testa se tem algum problema na abertura dos arquivos
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

        // Escrever unicode como UTF-32 Little-Endian e verificando erros
        if (fwrite(&unicode, 4, 1, arquivo_saida) != 1) {
            fprintf(stderr, "Erro ao escrever UTF-32 no arquivo de saída.\n");
            return -1;
        }
    }

    return 0;
}




// Função auxiliar para converter código Unicode para caracteres UTF-8
int unicode_to_utf8(unsigned int unicode, unsigned char *utf8) {
    if (unicode <= 0x7F) {
        utf8[0] = (unsigned char)unicode;
        return 1;
    } else if (unicode <= 0x7FF) {
        utf8[0] = 0xC0 | (unicode >> 6);
        utf8[1] = 0x80 | (unicode & 0x3F);
        return 2;
    } else if (unicode <= 0xFFFF) {
        utf8[0] = 0xE0 | (unicode >> 12);
        utf8[1] = 0x80 | ((unicode >> 6) & 0x3F);
        utf8[2] = 0x80 | (unicode & 0x3F);
        return 3;
    } else if (unicode <= 0x10FFFF) {
        utf8[0] = 0xF0 | (unicode >> 18);
        utf8[1] = 0x80 | ((unicode >> 12) & 0x3F);
        utf8[2] = 0x80 | ((unicode >> 6) & 0x3F);
        utf8[3] = 0x80 | (unicode & 0x3F);
        return 4;
    }
    return 0;
}

// Função que converte UTF32 para UTF8
int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida) {
    unsigned int unicode;
    unsigned char utf8[4];
    size_t num_bytes;

    // Ler o BOM para determinar o tipo de endianness e verifica erro
    if (fread(&unicode, 4, 1, arquivo_entrada) != 1) {
        fprintf(stderr, "Falha ao ler BOM do arquivo de entrada.\n");
        return -1;
    }

    // Determinar endianness
    int isLittleEndian = 0;
    if (unicode == 0xFFFE0000) {  // Verifica se é Big endian
        isLittleEndian = 0;
    } else if (unicode == 0x0000FEFF) {  // Verifica se é Little endian
        isLittleEndian = 1;
    } 
    
    else {
        fprintf(stderr, "BOM inválido ou arquivo UTF-32 não é endian-conhecido.\n");
        return -1;
    }

    while (fread(&unicode, 4, 1, arquivo_entrada) == 1) {
        if (!isLittleEndian) {
            // Converte de Big Endian para Little Endian
            unicode = (unicode >> 24) | 
                      ((unicode >> 8) & 0x0000FF00) | 
                      ((unicode << 8) & 0x00FF0000) | 
                      (unicode << 24);
        }

        num_bytes = unicode_to_utf8(unicode, utf8);

        if (fwrite(utf8, 1, num_bytes, arquivo_saida) != num_bytes) {
            fprintf(stderr, "Erro ao escrever UTF-8 no arquivo de saída.\n");
            return -1;
        }
    }

    return 0;
}