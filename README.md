# Blink com interrupções ![Language grade: C](https://img.shields.io/badge/language-C-blue)

Autora: Lesly Montúfar

Código blink com botão parar mudar frequência em que o led pisca, para STM32 utilizando interrupções.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Usando a STM32CubeIDE:
1. Configure a placa para ter dois botões e um led piscando, que serão tratados como interrupções.
2. O led piscará usando como tempo base o SysTick.
3. Pode-se fazer chamadas das funções do HAL e CSIS, para isso a suas funções podem ser abstraídas para a camada hw (de hardware, em `hw.c` e `hw.h`) para facilitar a leitura do código.
4. O desenvolvimento da aplicação deve estar numa pasta da aplicação `app`.
5. Os requisitos da aplicação serão cumpridos em `app.c` e `app.h`. 
6. As duas entradas, `PA0` e `PA1` obterão as seguintes possíveis saídas `S`, que são os delays ON/OFF do led:

| PA0 | PA1 | LED ON/OFF |
|:----------:|:----------:|:-------------:|
| 0 | 0 | 100 / 100 |
| 0 | 1 | 300 / 150 |
| 1 | 0 | 600 / 300 |
| 1 | 1 | 900 / 450 |


## Desenvolvimento

