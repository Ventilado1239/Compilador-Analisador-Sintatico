#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sintatico.h"

static Token current_token;

static void casaToken(int expected) {
    if (current_token.tipo == expected) {
        current_token = proximoToken();
    } else {
        if (current_token.tipo == TOKEN_EOF) {
            printf("%d:fim de arquivo não esperado.\n", current_token.linha);
        } else {
            printf("%d:token nao esperado [%s].\n", current_token.linha, current_token.lexema);
        }
        exit(1);
    }
}

static int is_relacao(int type) {
    return (type == '=' || type == TOKEN_NEQ || type == '<' || type == TOKEN_LE || type == TOKEN_GE || type == '>');
}

/* Declarações das funções de descida recursiva */
static void analisarPrograma(void);
static void analisarBloco(void);
static void analisarParteDeclaracoesVariaveis(void);
static void analisarDeclaracaoVariaveis(void);
static void analisarListaIdentificadores(void);
static void analisarTipo(void);
static void analisarComandoComposto(void);
static void analisarComando(void);
static void analisarAtribuicao(void);
static void analisarComandoCondicional(void);
static void analisarComandoRepetitivo(void);
static void analisarExpressao(void);
static void analisarRelacao(void);
static void analisarExpressaoSimples(void);
static void analisarTermo(void);
static void analisarFator(void);
static void analisarVariavel(void);

/* <programa>::= program <identificador> ; <bloco> . */
static void analisarPrograma(void) {
    printf("<programa> ::= program <identificador> ; <bloco> .\n");
    casaToken(TOKEN_PROGRAM);
    casaToken(TOKEN_IDENT);
    casaToken(';');
    analisarBloco();
    casaToken('.');
}

/* <bloco> ::= <parte de declarações de variáveis> <comando composto> */
static void analisarBloco(void) {
    printf("<bloco> ::= <parte de declarações de variáveis> <comando composto>\n");
    analisarParteDeclaracoesVariaveis();
    analisarComandoComposto();
}

/* <parte de declarações de variáveis> ::= { var <declaração de variáveis> {; <declaração de variáveis>} ; } */
static void analisarParteDeclaracoesVariaveis(void) {
    if (current_token.tipo != TOKEN_VAR) {
        printf("<parte de declarações de variáveis> ::= vazio\n");
        return;
    }
    printf("<parte de declarações de variáveis> ::= var <declaração de variáveis> {; <declaração de variáveis>} ;\n");
    for (;;) {
        if (current_token.tipo != TOKEN_VAR) break;
        casaToken(TOKEN_VAR);
        analisarDeclaracaoVariaveis();
        casaToken(';');
        for (;;) {
            if (current_token.tipo != TOKEN_IDENT) break;
            analisarDeclaracaoVariaveis();
            casaToken(';');
        }
    }
}

/* <declaração de variáveis> ::= <lista de identificadores> : <tipo> */
static void analisarDeclaracaoVariaveis(void) {
    printf("<declaração de variáveis> ::= <lista de identificadores> : <tipo>\n");
    analisarListaIdentificadores();
    casaToken(':');
    analisarTipo();
}

/* <lista de identificadores> ::= <identificador> { , <identificador> } */
static void analisarListaIdentificadores(void) {
    printf("<lista de identificadores> ::= <identificador> { , <identificador> }\n");
    casaToken(TOKEN_IDENT);
    for (;;) {
        if (current_token.tipo != ',') break;
        casaToken(',');
        casaToken(TOKEN_IDENT);
    }
}

/* <tipo> ::= integer | real */
static void analisarTipo(void) {
    switch (current_token.tipo) {
        case TOKEN_INTEGER:
            printf("<tipo> ::= integer\n");
            casaToken(TOKEN_INTEGER);
            break;
        case TOKEN_REAL:
            printf("<tipo> ::= real\n");
            casaToken(TOKEN_REAL);
            break;
        default:
            casaToken(TOKEN_INTEGER); // Força erro
            break;
    }
}

/* <comando composto> ::= begin <comando> ; { <comando> ; } end */
static void analisarComandoComposto(void) {
    printf("<comando composto> ::= begin <comando> ; { <comando> ; } end\n");
    casaToken(TOKEN_BEGIN);
    analisarComando();
    casaToken(';');
    for (;;) {
        if (current_token.tipo == TOKEN_END || current_token.tipo == TOKEN_EOF) {
            break;
        }
        analisarComando();
        casaToken(';');
    }
    casaToken(TOKEN_END);
}

/* <comando> ::= <atribuição> | <comando composto> | <comando condicional> | <comando repetitivo> */
static void analisarComando(void) {
    switch (current_token.tipo) {
        case TOKEN_IDENT:
            printf("<comando> ::= <atribuição>\n");
            analisarAtribuicao();
            break;
        case TOKEN_BEGIN:
            printf("<comando> ::= <comando composto>\n");
            analisarComandoComposto();
            break;
        case TOKEN_IF:
            printf("<comando> ::= <comando condicional>\n");
            analisarComandoCondicional();
            break;
        case TOKEN_WHILE:
            printf("<comando> ::= <comando repetitivo>\n");
            analisarComandoRepetitivo();
            break;
        default:
            casaToken(TOKEN_IDENT); // Força erro
            break;
    }
}

/* <atribuição> ::= <variável> := <expressão> */
static void analisarAtribuicao(void) {
    printf("<atribuição> ::= <variável> := <expressão>\n");
    analisarVariavel();
    casaToken(TOKEN_ASSIGN);
    analisarExpressao();
}

/* <comando condicional> ::= if <expressão> then <comando> [ else <comando> ] */
static void analisarComandoCondicional(void) {
    printf("<comando condicional> ::= if <expressão> then <comando> [ else <comando> ]\n");
    casaToken(TOKEN_IF);
    analisarExpressao();
    casaToken(TOKEN_THEN);
    analisarComando();
    if (current_token.tipo == TOKEN_ELSE) {
        casaToken(TOKEN_ELSE);
        analisarComando();
    }
}

/* <comando repetitivo> ::= while <expressão> do <comando> */
static void analisarComandoRepetitivo(void) {
    printf("<comando repetitivo> ::= while <expressão> do <comando>\n");
    casaToken(TOKEN_WHILE);
    analisarExpressao();
    casaToken(TOKEN_DO);
    analisarComando();
}

/* <expressão> ::= <expressão simples> [ <relação> <expressão simples> ] */
static void analisarExpressao(void) {
    printf("<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]\n");
    analisarExpressaoSimples();
    if (is_relacao(current_token.tipo)) {
        analisarRelacao();
        analisarExpressaoSimples();
    }
}

/* <relação> ::= = | <> | < | <= | >= | > */
static void analisarRelacao(void) {
    int type = current_token.tipo;
    if (is_relacao(type)) {
        printf("<relação> ::= %s\n", current_token.lexema);
        casaToken(type);
    } else {
        casaToken('='); // Força erro
    }
}

/* <expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> } */
static void analisarExpressaoSimples(void) {
    printf("<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }\n");
    if (current_token.tipo == '+' || current_token.tipo == '-') {
        casaToken(current_token.tipo);
    }
    analisarTermo();
    for (;;) {
        if (current_token.tipo != '+' && current_token.tipo != '-') break;
        casaToken(current_token.tipo);
        analisarTermo();
    }
}

/* <termo> ::= <fator> { ( * | / ) <fator> } */
static void analisarTermo(void) {
    printf("<termo> ::= <fator> { ( * | / ) <fator> }\n");
    analisarFator();
    for (;;) {
        if (current_token.tipo != '*' && current_token.tipo != '/') break;
        casaToken(current_token.tipo);
        analisarFator();
    }
}

/* <fator> ::= <variável> | <número> | ( <expressão> ) */
static void analisarFator(void) {
    switch (current_token.tipo) {
        case TOKEN_IDENT:
            printf("<fator> ::= <variável>\n");
            analisarVariavel();
            break;
        case TOKEN_NUM:
            printf("<fator> ::= <número>\n");
            casaToken(TOKEN_NUM);
            break;
        case '(':
            printf("<fator> ::= ( <expressão> )\n");
            casaToken('(');
            analisarExpressao();
            casaToken(')');
            break;
        default:
            casaToken(TOKEN_IDENT); // Força erro
            break;
    }
}

/* <variável> ::= <identificador> */
static void analisarVariavel(void) {
    printf("<variável> ::= <identificador>\n");
    casaToken(TOKEN_IDENT);
}

/* Ponto de entrada principal para o analisador sintático */
void iniciarSintatico(Token first_token) {
    current_token = first_token;
    analisarPrograma();
    
    // Verifica se processamos todos os tokens. Esperamos que o último seja TOKEN_EOF.
    // Se não for, significa que há lixo após o ponto final do programa.
    if (current_token.tipo != TOKEN_EOF) {
        printf("%d:token nao esperado [%s].\n", current_token.linha, current_token.lexema);
        exit(1);
    }
}