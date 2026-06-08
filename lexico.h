#ifndef LEXICO_H
#define LEXICO_H

/* -------------------- Definições de tokens -------------------- */

// Palavras-chave (Keywords)
#define TOKEN_PROGRAM     256
#define TOKEN_VAR         257
#define TOKEN_INTEGER     258
#define TOKEN_REAL        259
#define TOKEN_BEGIN       260
#define TOKEN_END         261
#define TOKEN_IF          262
#define TOKEN_THEN        263
#define TOKEN_ELSE        264
#define TOKEN_WHILE       265
#define TOKEN_DO          266

// Identificadores e Números
#define TOKEN_IDENT       267
#define TOKEN_NUM         268

// Operadores compostos / delimitadores
#define TOKEN_ASSIGN      269  // :=
#define TOKEN_NEQ         270  // <>
#define TOKEN_LE          271  // <=
#define TOKEN_GE          272  // >=

// Token inválido (caso de erro)
#define TOKEN_INVALID     273

// Fim de arquivo (EOF)
#define TOKEN_EOF         0

typedef struct {
    int tipo;
    char lexema[128]; // String original do lexema
    double valor;     // Valor numérico (se aplicável, para TOKEN_NUM)
    int linha;        // Número da linha onde o token foi encontrado
} Token;

/* -------------------- Assinaturas -------------------- */
void initLexico(const char *src);
Token proximoToken(void);
const char *nomeToken(int t);

#endif

