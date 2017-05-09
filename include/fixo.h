#ifndef _FIXO_H_
#define _FIXO_H_

/*
*   void escreverFixo();
* Funcao responsavel por transcrever o arquivo de entrada padrao inteiro forma_
* tado com campos de tamanho fixo
* @return vazio
* @args vazio
*/
void escreverFixo();

/*
*   bool buscaDominioFixo(FILE *arquivo, char *dominioBuscado)
* Funcao responsavel por imprimir todos os registros de um arquivo formatado
* por campos de tamanho fixo que possuem o dominio buscado passado.
* @return VERDADEIRO caso encontre-se algum elemento que contenha o dominio 
* buscado ou FALSO caso contrario.
* @args arquivo que se le e o dominio buscado.
*/
void buscaDominioFixo(FILE *, char *);

/*
*   void visualizarFixo(FILE *arquivo)
* Funcao responsavel por imprimir todos os registros de um arquivo formatado
* por campos de tamanho fixo
* @return vazio
* @args arquivo que se le.
*/
void visualizarFixo(FILE *);

/*
*   void registroRRNFixo(FILE *arquivo, int rrn)
* Funcao responsavel por imprimir um registro existente em um arquivo passado
* para a funcao a partir de um rrn escolhido.
* @return vazio
* @args arquivo que se le e o rrn escolhido.
*/
void registroRRNFixo(FILE *, int);

/*
*    void campoRegistroFixo(FILE *arquivoDeSaida, int rrn, int campo)
* Funcao responsavel por imprimir um campo de um registro com campos de tamanho
* fixo a partir do rrn passado para a funcao.
* @return vazio
* @args arquivo que se le, rrn escolhido e o numro do campo.
*/
void campoRegistroFixo(FILE *, int , int );

#endif