#ifndef _DELIMITADOR_H_
#define _DELIMITADOR_H_

/*
*   void escreverDelimitador();
* Funcao responsavel por transcrever o arquivo de entrada padrao inteiro forma_
* tado com Delimitadores entre registros
* @return vazio
* @args vazio
*/
void escreverDelimitador();

/*
*   bool buscaDominioDelimitadores(FILE *arquivo, unsigned char *dominioBuscado)
* Funcao responsavel por imprimir todos os registros de um arquivo formatado
* por delimitadores entre registros que possuem o dominio buscado passado.
* @return VERDADEIRO caso encontre-se algum elemento que contenha o dominio 
* buscado ou FALSO caso contrario.
* @args arquivo que se le e o dominio buscado.
*/
void buscaDominioDelimitadores(FILE *, char *);

/*
*   void visualizarDelimitadores(FILE *arquivo)
* Funcao responsavel por imprimir todos os registros de um arquivo formatado
* por delimitadores entre registros
* @return vazio
* @args arquivo que se le.
*/
void visualizarDelimitadores(FILE *);

/*
*   void registroRRNDelimitadores(FILE *arquivo, int rrn)
* Funcao responsavel por imprimir um registro existente em um arquivo passado
* para a funcao a partir de um rrn escolhido.
* @return vazio
* @args arquivo que se le e o rrn escolhido.
*/
void registroRRNDelimitadores(FILE *, int);

/*
*    void campoRegistroDelimitadores(FILE *arquivoDeSaida, int rrn, int campo)
* Funcao responsavel por imprimir um campo de um registro com delimitadores entre
* registros a partir do rrn passado para a funcao.
* @return vazio
* @args arquivo que se le, rrn escolhido e o numro do campo.
*/
void campoRegistroDelimitadores(FILE *, int, int);

#endif