#ifndef _DELIMITADOR_H_
#define _DELIMITADOR_H_

/* 
* -- int escreverDelimitador()
* Escreve um arquivo de saida com os dados do arquivo de entrada organizados em
* registros separados por delimitador (#) com campos de tamanho fixo e variavel
*/
int escreverDelimitador();

void imprimirRegistroDelimitadores(FILE *);

int buscaDominioDelimitadores(char *);

void visualizarTodosDelimitadores();

void buscaRRNDelimitadores(int );

#endif