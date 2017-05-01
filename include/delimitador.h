#ifndef _DELIMITADOR_H_
#define _DELIMITADOR_H_

int escreverDelimitador(FILE *, FILE *);

void visualizarDelimitadores(FILE *fSaida);

int buscaDominioDelimitadores(char *dominioBuscado, FILE *fp);

void registroRRNDelimitadores(int rrn, FILE *fSaida);

#endif