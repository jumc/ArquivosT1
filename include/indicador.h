#ifndef _INDICADOR_H_
#define _INDICADOR_H_

/*
*   void escreverIndicador();
* Funcao responsavel por transcrever o arquivo de entrada padrao inteiro forma_
* tado com indicadores de tamanho
* @return vazio
* @args vazio
*/
void escreverIndicador();

/*
*   bool buscaDominioIndicador(FILE *arquivo, char *dominioBuscado)
* Funcao responsavel por imprimir todos os registros de um arquivo formatado
* por indicadores de tamanho que possuem o dominio buscado passado.
* @return VERDADEIRO caso encontre-se algum elemento que contenha o dominio 
* buscado ou FALSO caso contrario.
* @args arquivo que se le e o dominio buscado.
*/
bool buscaDominioIndicador(FILE *, char *);

/*
*   void visualizarIndicador(FILE *arquivo)
* Funcao responsavel por imprimir todos os registros de um arquivo formatado
* por indicadores de tamanho.
* @return vazio
* @args arquivo que se le.
*/
void visualizarIndicador(FILE *);

/*
*   void registroRRNIndicador(FILE *arquivo, int rrn)
* Funcao responsavel por imprimir um registro existente em um arquivo passado
* para a funcao a partir de um rrn escolhido.
* @return vazio
* @args arquivo que se le e o rrn escolhido.
*/
void registroRRNIndicador(FILE *, int );

/*
*    void campoRegistroIndicador(FILE *arquivoDeSaida, int rrn, int campo)
* Funcao responsavel por imprimir um campo de um registro com indicadores de tamanho
* a partir do rrn passado para a funcao.
* @return vazio
* @args arquivo que se le, rrn escolhido e o numro do campo.
*/
void campoRegistroIndicador(FILE *, int , int );

#endif