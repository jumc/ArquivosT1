#ifndef _UTILS_H_
#define _UTILS_H_

char *lerLinha(FILE *fp, char delimiter);

int lerInt(FILE *fp);

long int lerLong(FILE *fp);

void lerCampo(char **string, int *tamCampo, FILE *fEntrada);

int salvaString(char *string, int tamCampo, FILE *fSaida);

int salvaInt(int *n, FILE *fSaida);

int salvaLong(long int *n, FILE *fSaida);

int salvaCampoVariavel(FILE *fEntrada, FILE *fSaida, char *nomeCampo);

int salvaCampoFixo(FILE *fEntrada, FILE *fSaida, int limite, char *nomeCampo);

int salvaCampoLong(FILE *fEntrada, FILE *fSaida, char *nomeCampo);



#endif