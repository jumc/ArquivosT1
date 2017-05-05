#ifndef _UTILS_H_
#define _UTILS_H_

enum{
  INVALID = -1,
  SAIR,
  INDICADOR_DE_TAMANHO,
  DELIMITADOR_ENTRE_REG,
  NUMERO_DE_CAMPS_FIXO,
  VIZUALIZAR_TODOS,
  VIZUALIZAR_DOMINIO,
  VIZUALIZAR_REG_RRN,
  VIZUALIZAR_CAMPO_RRN,
  MENU = 9
};

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

char *campoVariavel(FILE *fp);

void vizualizaCampoVariavel(FILE *fp, char *nomeCampo);

void vizualizaCampoFixo(FILE *fp, char *nomeCampo, int tam);

void vizualizaCampoLong(FILE *fp, char *nomeCampo);

void visualizar(FILE *fSaida);

int buscaDominio(FILE *fSaida);

void registroRRN(FILE *fSaida);

FILE *fopen_(char *filename, char *openmode);

int lerOperacao();

void imprimirSaudacoes();

#endif
