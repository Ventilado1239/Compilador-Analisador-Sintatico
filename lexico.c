#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexico.h"

static const char *fonte;
static int pos_fonte = 0;
static int linha_atual = 1;

/*
 * initLexico
 * Inicializa o analisador lexico com o codigo-fonte.
 */
void initLexico(const char *src) {
    fonte = src;
    pos_fonte = 0;
    linha_atual = 1;
}

/* Pula espaços em branco e comentários */
static void pular_espacos_e_comentarios(void) {
    while (1) {
        // Ignora espaços, tabulações e quebras de linha
        while (fonte[pos_fonte] == ' ' || fonte[pos_fonte] == '\t' || fonte[pos_fonte] == '\r' || fonte[pos_fonte] == '\n') {
            if (fonte[pos_fonte] == '\n') {
                linha_atual++;
            }
            pos_fonte++;
        }

        // Verifica comentário no estilo { ... }
        if (fonte[pos_fonte] == '{') {
            pos_fonte++; // consome '{'
            while (fonte[pos_fonte] != '\0' && fonte[pos_fonte] != '}') {
                if (fonte[pos_fonte] == '\n') {
                    linha_atual++;
                }
                pos_fonte++;
            }
            if (fonte[pos_fonte] == '}') {
                pos_fonte++; // consome '}'
                continue; // volta a checar se há mais espaços ou comentários
            } else {
                // Comentário não fechado
                break;
            }
        }

        // Verifica comentário no estilo (* ... *)
        if (fonte[pos_fonte] == '(' && fonte[pos_fonte + 1] == '*') {
            pos_fonte += 2; // consome '(*'
            while (fonte[pos_fonte] != '\0' && !(fonte[pos_fonte] == '*' && fonte[pos_fonte + 1] == ')')) {
                if (fonte[pos_fonte] == '\n') {
                    linha_atual++;
                }
                pos_fonte++;
            }
            if (fonte[pos_fonte] == '*' && fonte[pos_fonte + 1] == ')') {
                pos_fonte += 2; // consome '*)'
                continue; // volta a checar se há mais espaços ou comentários
            } else {
                // Comentário não fechado
                break;
            }
        }

        // Se chegou aqui, não há mais comentários ou espaços para pular
        break;
    }
}

/*
 * proximoToken
 * Retorna o proximo token lido do arquivo-fonte.
 */
Token proximoToken(void) {
    Token tok;
    memset(&tok, 0, sizeof(tok));

    pular_espacos_e_comentarios();

    tok.linha = linha_atual;

    if (fonte[pos_fonte] == '\0') {
        tok.tipo = TOKEN_EOF;
        strcpy(tok.lexema, "fim de arquivo");
        return tok;
    }
    
    char c = fonte[pos_fonte];

    // Identificadores e Palavras Reservadas
    if (isalpha((unsigned char)c) || c == '_') {
        int len = 0;
        while (isalnum((unsigned char)fonte[pos_fonte]) || fonte[pos_fonte] == '_') {
            if (len < 127) {
                tok.lexema[len++] = fonte[pos_fonte];
            }
            pos_fonte++;
        }
        tok.lexema[len] = '\0';

        // Checa palavras-chave
        if (strcmp(tok.lexema, "program") == 0) tok.tipo = TOKEN_PROGRAM;
        else if (strcmp(tok.lexema, "var") == 0) tok.tipo = TOKEN_VAR;
        else if (strcmp(tok.lexema, "integer") == 0) tok.tipo = TOKEN_INTEGER;
        else if (strcmp(tok.lexema, "real") == 0) tok.tipo = TOKEN_REAL;
        else if (strcmp(tok.lexema, "begin") == 0) tok.tipo = TOKEN_BEGIN;
        else if (strcmp(tok.lexema, "end") == 0) tok.tipo = TOKEN_END;
        else if (strcmp(tok.lexema, "if") == 0) tok.tipo = TOKEN_IF;
        else if (strcmp(tok.lexema, "then") == 0) tok.tipo = TOKEN_THEN;
        else if (strcmp(tok.lexema, "else") == 0) tok.tipo = TOKEN_ELSE;
        else if (strcmp(tok.lexema, "while") == 0) tok.tipo = TOKEN_WHILE;
        else if (strcmp(tok.lexema, "do") == 0) tok.tipo = TOKEN_DO;
        else tok.tipo = TOKEN_IDENT;
        
        return tok;
    }

    // Números (inteiros ou reais)
    if (isdigit((unsigned char)c)) {
        int len = 0;
        while (isdigit((unsigned char)fonte[pos_fonte])) {
            if (len < 127) {
                tok.lexema[len++] = fonte[pos_fonte];
            }
            pos_fonte++;
        }
        // Verifica se possui parte decimal
        if (fonte[pos_fonte] == '.' && isdigit((unsigned char)fonte[pos_fonte + 1])) {
            if (len < 127) {
                tok.lexema[len++] = fonte[pos_fonte];
            }
            pos_fonte++; // consome '.'
            while (isdigit((unsigned char)fonte[pos_fonte])) {
                if (len < 127) {
                    tok.lexema[len++] = fonte[pos_fonte];
                }
                pos_fonte++;
            }
        }
        tok.lexema[len] = '\0';
        tok.tipo = TOKEN_NUM;
        tok.valor = strtod(tok.lexema, NULL);
        return tok;
    }

    // Atribuição ou Dois-pontos
    if (c == ':') {
        if (fonte[pos_fonte + 1] == '=') {
            tok.tipo = TOKEN_ASSIGN;
            strcpy(tok.lexema, ":=");
            pos_fonte += 2;
        } else {
            tok.tipo = ':';
            strcpy(tok.lexema, ":");
            pos_fonte++;
        }
        return tok;
    }

    // Operadores relacionais ou Menor-que
    if (c == '<') {
        if (fonte[pos_fonte + 1] == '>') {
            tok.tipo = TOKEN_NEQ;
            strcpy(tok.lexema, "<>");
            pos_fonte += 2;
        } else if (fonte[pos_fonte + 1] == '=') {
            tok.tipo = TOKEN_LE;
            strcpy(tok.lexema, "<=");
            pos_fonte += 2;
        } else {
            tok.tipo = '<';
            strcpy(tok.lexema, "<");
            pos_fonte++;
        }
        return tok;
    }

    if (c == '>') {
        if (fonte[pos_fonte + 1] == '=') {
            tok.tipo = TOKEN_GE;
            strcpy(tok.lexema, ">=");
            pos_fonte += 2;
        } else {
            tok.tipo = '>';
            strcpy(tok.lexema, ">");
            pos_fonte++;
        }
        return tok;
    }

    // Símbolos e operadores de um único caractere
    if (c == ';' || c == '.' || c == ',' || c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
        tok.tipo = c;
        tok.lexema[0] = c;
        tok.lexema[1] = '\0';
        pos_fonte++;
        return tok;
    }

    // Caractere inválido
    tok.tipo = TOKEN_INVALID;
    tok.lexema[0] = c;
    tok.lexema[1] = '\0';
    pos_fonte++;
    return tok;
}

/*
 * nomeToken
 * Retorna a representacao textual de um token.
 */
const char *nomeToken(int t) {
    switch (t) {
        case TOKEN_PROGRAM: return "program";
        case TOKEN_VAR: return "var";
        case TOKEN_INTEGER: return "integer";
        case TOKEN_REAL: return "real";
        case TOKEN_BEGIN: return "begin";
        case TOKEN_END: return "end";
        case TOKEN_IF: return "if";
        case TOKEN_THEN: return "then";
        case TOKEN_ELSE: return "else";
        case TOKEN_WHILE: return "while";
        case TOKEN_DO: return "do";
        case TOKEN_IDENT: return "IDENT";
        case TOKEN_NUM: return "NUM";
        case TOKEN_ASSIGN: return ":=";
        case TOKEN_NEQ: return "<>";
        case TOKEN_LE: return "<=";
        case TOKEN_GE: return ">=";
        case TOKEN_INVALID: return "INVALID";
        case TOKEN_EOF: return "EOF";
        default: {
            static char buf[2] = {0, 0};
            buf[0] = (char)t;
            return buf;
        }
    }
}