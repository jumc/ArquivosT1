#ifndef _UTILS_H_
#define _UTILS_H_

char *lerLinha(FILE *fp, char delimiter);

char *lerNChar(FILE *fp, int n);

int lerCharToInt(FILE *fp);

long int lerCharToLong(FILE *fp);

void lerCampo(char **string, int *tamCampo, FILE *fEntrada);

int salvaString(char *string, int tamCampo, FILE *fSaida);

int salvaInt(int *n, FILE *fSaida);

int salvaLong(long int *n, FILE *fSaida);

int salvaCampoVariavel(FILE *fEntrada, FILE *fSaida, char *nomeCampo);

int salvaCampoFixo(FILE *fEntrada, FILE *fSaida, int limite, char *nomeCampo);

int salvaCampoLong(FILE *fEntrada, FILE *fSaida, char *nomeCampo);

void vizualizaCampoVariavel(FILE *fp, char *nomeCampo);

void vizualizaCampoFixo(FILE *fp, char *nomeCampo, int tam);

void vizualizaCampoLong(FILE *fp, char *nomeCampo);

void visualizar(FILE *fSaida);

#endif