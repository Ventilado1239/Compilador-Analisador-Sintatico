# Compiladores — Analisador Sintático

Analisador léxico e sintático para a linguagem MicroPascal (μ-Pascal), implementado em C.

**Disciplina:** Linguagens Formais, Autômatos e Compiladores
**Universidade:** Universidade Católica de Brasília — 1º semestre de 2026
**Professor:** Marcelo Eustáquio

## Compilação

```bash
gcc main.c lexico.c sintatico.c -o compilador.exe
```

## Uso

```bash
./compilador.exe <arquivo_fonte.mp>

# Exemplo rodando um dos testes:
./compilador.exe testes/correto1.mp
```

## Estrutura

```
src/
  main.c         — Ponto de entrada
  lexico.h       — Definições de tokens e assinaturas do léxico
  lexico.c       — Implementação do analisador léxico
  sintatico.h    — Assinatura do analisador sintático
  sintatico.c    — Implementação do analisador sintático (descida recursiva)
testes/
  correto1.mp    — Programa correto (atribuições simples)
  correto2.mp    — Programa correto (while, if/else, múltiplas variáveis)
  correto3.mp    — Programa correto (if com begin/end aninhado)
  errado1.mp     — Erro: falta ponto e vírgula
  errado2.mp     — Erro: falta ponto final
  errado3.mp     — Erro: operador inválido =>
relatorio.md     — Relatório técnico
```
