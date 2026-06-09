**Universidade Católica de Brasília**
Linguagens Formais, Autômatos e Compiladores — 1º semestre de 2026
Professor: MARCELO EUSTÁQUIO

---

# RELATÓRIO TÉCNICO: ANALISADOR LÉXICO E SINTÁTICO PARA MICROPASCAL

Este relatório apresenta o desenvolvimento do nosso analisador léxico e sintático para a linguagem MicroPascal (μ-Pascal). O projeto foi feito em C, seguindo as diretrizes da disciplina de Linguagens Formais, Autômatos e Compiladores.

---

## 1. Descrição Geral da Arquitetura

Decidimos dividir o projeto em três partes principais para facilitar a organização:
1. **Analisador Léxico (`lexico.h` / `lexico.c`)**: Responsável por varrer o código-fonte caractere por caractere, ignorando espaços e comentários, e agrupando o texto em tokens.
2. **Analisador Sintático (`sintatico.h` / `sintatico.c`)**: Pede os tokens um por um ao léxico (sob demanda) e faz a análise sintática descendente recursiva (LL(1)). Ele vai testando as regras da gramática e imprimindo as derivações no terminal.
3. **Ponto de Entrada (`main.c`)**: É o arquivo principal que abre o arquivo fonte, inicializa o léxico e dá o "start" no sintático.

---

## 2. Analisador Léxico

### 2.1. Estruturas de Dados

- **`Token`**:
  ```c
  typedef struct {
      int tipo;          // Tipo do token (definido pelas constantes de token)
      char lexema[128];  // Texto bruto do lexema correspondente
      double valor;      // Valor numérico (relevante apenas para TOKEN_NUM)
      int linha;         // Número da linha do programa onde o token foi encontrado
  } Token;
  ```


### 2.2. Funções Implementadas no Léxico

- **`initLexico`**: Prepara o analisador léxico, recebendo a string do código-fonte e setando a linha inicial para 1.
- **`proximoToken`**: Essa é a função principal do léxico. Ela é chamada toda vez que o sintático precisa de um novo token. Ela pula os espaços e comentários, lê os caracteres e monta o token certinho (seja palavra reservada, número, identificador ou operador).
- **`pular_espacos_e_comentarios`**: Função auxiliar que ignora espaços em branco, quebras de linha (atualizando a contagem) e os comentários em bloco (tanto no estilo `{ ... }` quanto `(* ... *)`).
- **`nomeToken`**: Uma função utilitária para imprimir o nome do token como texto (ajuda bastante na hora de debugar).

---

## 3. Analisador Sintático

### 3.1. Estruturas de Dados

- **Variável global `current_token`**: O estado da análise é mantido apenas pelo token corrente.
  ```c
  static Token current_token;
  ```

### 3.2. Funções Implementadas

- **`casaToken`**: Compara o tipo do token corrente com o esperado pela gramática. Se coincidirem, consome o token chamando `proximoToken()` e avança a leitura. Caso contrário, reporta erro e finaliza a execução com `exit(1)`.
- **`iniciarSintatico`**: Função principal que inicia o processo de análise a partir do símbolo não-terminal `<programa>` recebendo o primeiro token lido pelo programa principal, e valida se o arquivo foi completamente analisado ao fim.
- **Funções dos Não-Terminais**: Cada não-terminal possui uma função dedicada que representa suas regras de produção:
  - `analisarPrograma`
  - `analisarBloco`
  - `analisarParteDeclaracoesVariaveis`
  - `analisarDeclaracaoVariaveis`
  - `analisarListaIdentificadores`
  - `analisarTipo`
  - `analisarComandoComposto`
  - `analisarComando`
  - `analisarAtribuicao`
  - `analisarComandoCondicional`
  - `analisarComandoRepetitivo`
  - `analisarExpressao`
  - `analisarRelacao`
  - `analisarExpressaoSimples`
  - `analisarTermo`
  - `analisarFator`
  - `analisarVariavel`

Cada função imprime a sua regra de produção correspondente no console à medida que é selecionada pelo analisador sintático (derivação à esquerda).

---

## 4. Testes e Resultados de Execução

Conforme solicitado, criamos 3 programas de teste que estão corretos segundo a gramática, e 3 programas com erros sintáticos propositais para ver se o compilador pega o erro e para na linha certa.

### 4.1. Programas Corretos

#### Caso 1: `correto1.mp`
**Código-fonte:**
```pascal
program teste1;
var x, y : integer;
begin
  x := 10;
  y := 20;
end.
```
**Resultado da Execução:**
```text
<programa> ::= program <identificador> ; <bloco> .
<bloco> ::= <parte de declarações de variáveis> <comando composto>
<parte de declarações de variáveis> ::= var <declaração de variáveis> {; <declaração de variáveis>} ;
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= integer
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
Compilacao concluida com sucesso!
```

---

#### Caso 2: `correto2.mp`
**Código-fonte:**
```pascal
program teste2;
var a : real;
    b : integer;
begin
  a := 5.5;
  while a > 0.0 do
    begin
      if b = 1 then
        b := 0
      else
        b := 1;
      a := a - 1.0;
    end;
end.
```
**Resultado da Execução:**
```text
<programa> ::= program <identificador> ; <bloco> .
<bloco> ::= <parte de declarações de variáveis> <comando composto>
<parte de declarações de variáveis> ::= var <declaração de variáveis> {; <declaração de variáveis>} ;
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= real
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= integer
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <comando repetitivo>
<comando repetitivo> ::= while <expressão> do <comando>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <variável>
<variável> ::= <identificador>
<relação> ::= >
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <comando composto>
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <comando condicional>
<comando condicional> ::= if <expressão> then <comando> [ else <comando> ]
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <variável>
<variável> ::= <identificador>
<relação> ::= =
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <variável>
<variável> ::= <identificador>
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
Compilacao concluida com sucesso!
```

---

#### Caso 3: `correto3.mp`
**Código-fonte:**
```pascal
program teste3;
var x : integer;
var y : real;
begin
  x := 10;
  if x < 20 then
    begin
      y := x * 2.5;
    end;
end.
```
**Resultado da Execução:**
```text
<programa> ::= program <identificador> ; <bloco> .
<bloco> ::= <parte de declarações de variáveis> <comando composto>
<parte de declarações de variáveis> ::= var <declaração de variáveis> {; <declaração de variáveis>} ;
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= integer
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= real
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <comando condicional>
<comando condicional> ::= if <expressão> then <comando> [ else <comando> ]
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <variável>
<variável> ::= <identificador>
<relação> ::= <
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
<comando> ::= <comando composto>
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <variável>
<variável> ::= <identificador>
<fator> ::= <número>
Compilacao concluida com sucesso!
```

---

### 4.2. Programas com Erros Sintáticos

#### Caso 1: `errado1.mp`
**Código-fonte:**
```pascal
program erro1;
var x : integer;
begin
  x := 10
end.
```
**Resultado da Execução:**
```text
<programa> ::= program <identificador> ; <bloco> .
<bloco> ::= <parte de declarações de variáveis> <comando composto>
<parte de declarações de variáveis> ::= var <declaração de variáveis> {; <declaração de variáveis>} ;
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= integer
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
5:token nao esperado [end].
```
*Explicação:* Colocamos um erro na linha 5. Faltou o ponto e vírgula `;` depois do `x := 10`. O compilador acusa erro no momento em que lê a palavra `end` porque estava esperando um `;`.

---

#### Caso 2: `errado2.mp`
**Código-fonte:**
```pascal
program erro2;
var x : integer;
begin
  x := 10;
end
```
**Resultado da Execução:**
```text
<programa> ::= program <identificador> ; <bloco> .
<bloco> ::= <parte de declarações de variáveis> <comando composto>
<parte de declarações de variáveis> ::= var <declaração de variáveis> {; <declaração de variáveis>} ;
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= integer
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <atribuição>
<atribuição> ::= <variável> := <expressão>
<variável> ::= <identificador>
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <número>
6:fim de arquivo não esperado.
```
*Explicação:* Esquecemos de colocar o ponto final (`.`) na última linha do programa. O analisador sintático chega no final do arquivo esperando o `.` e reporta o erro "fim de arquivo não esperado".

---

#### Caso 3: `errado3.mp`
**Código-fonte:**
```pascal
program erro3;
var x : integer;
begin
  if x => 10 then
    x := 5;
end.
```
**Resultado da Execução:**
```text
<programa> ::= program <identificador> ; <bloco> .
<bloco> ::= <parte de declarações de variáveis> <comando composto>
<parte de declarações de variáveis> ::= var <declaração de variáveis> {; <declaração de variáveis>} ;
<declaração de variáveis> ::= <lista de identificadores> : <tipo>
<lista de identificadores> ::= <identificador> { , <identificador> }
<tipo> ::= integer
<comando composto> ::= begin <comando> ; { <comando> ; } end
<comando> ::= <comando condicional>
<comando condicional> ::= if <expressão> then <comando> [ else <comando> ]
<expressão> ::= <expressão simples> [ <relação> <expressão simples> ]
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
<fator> ::= <variável>
<variável> ::= <identificador>
<relação> ::= =
<expressão simples> ::= [ + | - ] <termo> { ( + | - ) <termo> }
<termo> ::= <fator> { ( * | / ) <fator> }
4:token nao esperado [>].
```
*Explicação:* Aqui usamos o operador `=>` (maior ou igual ao contrário). No MicroPascal, o correto seria `>=`. O analisador léxico tenta ser esperto e quebra `=>` em `=` e `>`. O sintático acaba aceitando o `=` como relação, mas logo depois dá de cara com o `>` e não entende nada, acusando o erro corretamente.
