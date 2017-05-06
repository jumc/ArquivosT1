#ifndef _INDICADOR_H_
#define _INDICADOR_H_

int escreverIndicador(FILE *, FILE *);

void visualizarIndicadores(FILE *fSaida);

bool buscaDominioIndicadores(char *dominioBuscado, FILE *fp);

void registroRRNIndicadores(int rrn, FILE *fp);

#endif