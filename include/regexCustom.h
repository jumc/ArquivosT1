#ifndef _REGEX_CUSTOM_H_
#define _REGEX_CUSTOM_H_

/*
*    int match(char *linha, char *regex)
* Funcao responsavel por verificar se uma linha combina com uma expressao regular
* passada para a funcao. Caso a linha combine com a expressao, retorna-se um
* inteiro que representa sucesso. Caso contrario, retorna-se o inteiro oposto.
* @return inteiro que representa sucesso (1) ou falha (0)
* @args linha a ser verificada e a expressao regular
*/
int match(char *, char *);

/*
*    char *tokenize (char *linha, int n, char delimitador)
* Funcao responsavel por 'tokenizar', ie, colher de uma linha de caracteres uma
* palavra circundada por delimitadores comuns e que ocupa a posicao n. Exemplo:
* tome "isso,eh,um,exemplo" como a linha, n = 2 e o delimitador = ','. O retorno
* da funcao neste caso seria "um".
* @return token escolhido
* @args linha a ser percorrida, a posicao do token desejado e o delimitador
* existente entre os tokens
*/
char *tokenize (char *string, int n, char delim);

#endif