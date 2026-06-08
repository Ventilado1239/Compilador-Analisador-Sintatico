#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "sintatico.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_fonte>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo '%s'\n", argv[1]);
        return 1;
    }

    // Lê o conteúdo do arquivo
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *src = malloc(size + 1);
    if (!src) {
        perror("malloc");
        fclose(f);
        return 1;
    }

    size_t read_bytes = fread(src, 1, size, f);
    src[read_bytes] = '\0';
    fclose(f);

    // 1) Inicializa a análise léxica
    initLexico(src);

    // 2) Inicia a análise sintática
    Token first_token = proximoToken();
    iniciarSintatico(first_token);

    // 3) Limpa a memória e reporta sucesso
    free(src);
    printf("Compilacao concluida com sucesso!\n");
    return 0;
}
