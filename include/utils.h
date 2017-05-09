#ifndef _UTILS_H_
#define _UTILS_H_

#define bool int

enum{
  INVALID = -1,
  SAIR,
  INDICADOR_DE_TAMANHO,
  DELIMITADOR_ENTRE_REG,
  NUMERO_DE_CAMPS_FIXO,
  VISUALIZAR_TODOS,
  VISUALIZAR_DOMINIO,
  VISUALIZAR_REG_RRN,
  VISUALIZAR_CAMPO_RRN,
  MENU = 9
};

enum{
  FALSE,
  TRUE
};

/*
*    char *lerLinha(FILE *arquivo, int *tamanho, char delimitador) 
* Funcao responsavel por ler uma linha de um arquivo de entrada ate que se en_
* contre um delimitador valido. Ao final o tamanho e retornado por referencia.
* @return a linha lida
* @args arquivo de entrada, tamanho que sera retornado e um delimitador.
*/
char *lerLinha(FILE *fp, int *tamanho, char delimiter);

/*
*    char *lerNChar(FILE *arquivo, int n)
* Funcao responsavel por ler uma linha de tamanho n de um arquivo de entrada.
* Caso encontre-se o final do arquivo, retorna-se uma linha de tamanho menor.
* @return a linha lida
* @args arquivo de entrada e o tamanho da linha desejado.
*/
char *lerNChar(FILE *, int );

/*
*    long int lerInt(FILE *arquivo)
* Funcao responsavel por ler um numero do tipo int de um arquivo de entrada.
* @return o valor do numero do tipo int lido.
* @args arquivo de entrada
*/
int lerInt(FILE *);

/*
*    long int lerLong(FILE *arquivo)
* Funcao responsavel por ler um numero do tipo long de um arquivo de entrada.
* @return o valor do numero do tipo int lido.
* @args arquivo de entrada
*/
long int lerLong(FILE *);

/*
*    int lerCharToInt(FILE *arquivo)
* Funcao responsavel por ler um numero do tipo int de um arquivo de entrada
* a partir da conversao de caracteres.
* @return o valor do numero do tipo int lido. Caso nenhum numero seja lido,
* retorna-se -1
* @args arquivo de entrada
*/
int lerCharToInt(FILE *);

/*
*    long int lerCharToLong(FILE *arquivo)
* Funcao responsavel por ler um numero do tipo long de um arquivo de entrada
* a partir da conversao de caracteres.
* @return o valor do numero do tipo long lido. Caso nenhum numero seja lido,
* retorna-se -1
* @args arquivo de entrada
*/
long int lerCharToLong(FILE *);

/*
*    void lerCampo(char **string, int *tamCampo, FILE *arquivoDeEntrada)
* Funcao responsavel por ler um campo do arquivo de entrada.
* @return vazio
* @args string que retornara com o campo lido, um inteiro que retornara com o
* tamanho do campo lido e o arquivo de entrada o qual se deseja ler
*/
void lerCampo(char **, int *, FILE *);

/*
*    int salvaString(char *string, int tamCampo, FILE *arquivoDeSaida)
* Funcao responsavel por salvar o conteudo de uma string em um arquivo de
* saida.
* @return inteiro que diz se a funcao foi bem sucedida (1) ou nao (0)
* @args numero do tipo cahr *, o tamanho da string e o arquivo no qual se deseja
* escrever
*/
int salvaLinha(char *, int , FILE *);

/*
*    int salvaInt(long int *n, FILE *arquivoDeSaida)
* Funcao responsavel por salvar o conteudo de um numero int em um arquivo de
* saida.
* @return inteiro que diz se a funcao foi bem sucedida (1) ou nao (0)
* @args numero do tipo int e o arquivo que se deseja escrever
*/
int salvaInt(int *, FILE *);

/*
*    int salvaLong(long int *n, FILE *arquivoDeSaida)
* Funcao responsavel por salvar o conteudo de um numero long em um arquivo de
* saida.
* @return inteiro que diz se a funcao foi bem sucedida (1) ou nao (0)
* @args numero do tipo long e o arquivo que se deseja escrever
*/
int salvaLong(long int *, FILE *);

/*
*    int salvaCampoVariavel(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, char *nomeCampo)
* Funcao responsavel por salvar o conteudo de um campo de tamanho variavel de um
* arquivo de entrada em um arquivo de saida.
* @return inteiro que diz se a funcao foi bem sucedida (qtdd bytes escritos)
* ou nao (-1)
* @args arquivo de entrada, arquivo de saida e nome do campo
*/
int salvaCampoVariavel(FILE *, FILE *, char *);

/*
*    int salvaCampoFixo(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, 
* int limite, char *nomeCampo)
* Funcao responsavel por salvar o conteudo de um campo de tamanho fixo de um
* arquivo de entrada para um arquivo de saida.
* @return inteiro que diz se a funcao foi bem sucedida (1) ou nao (0)
* @args arquivo de entrada, arquivo de saida, tamanho limite do campo de tamanho
* fixo e o nome do campo
*/
int salvaCampoFixo(FILE *, FILE *, int , char *);

/*
*    int salvaCampoLong(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, char *nomeCampo)
* Funcao responsavel por salvar o conteudo de um campo do tipo long de um
* arquivo de entrada para um arquivo de saida.
* @return inteiro que diz se a funcao foi bem sucedida (1) ou nao (0)
* @args arquivo de entrada, arquivo de saida e nome do campo
*/
int salvaCampoLong(FILE *, FILE *, char *);

/*
*    char *campoVariavel(FILE *arquivo)
* Funcao responsavel por retornar o conteudo de um campo de tamanho variavel
* @return string que representa o campo de tamanho variavel
* @args arquivo o qual se le o arquivo de tamanho variavel
*/
char *campoVariavel(FILE *);

/*
*    int salvaCampoVariavelAlt(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, 
* char *string, int *tamCampo, char *nomeCampo)
* Funcao responsavel por salvar de um arquivo de entrada em um arquivo de saida
* uma string que representa um campo de tamanho variavel.
* @return inteiro que diz se a funcao foi bem (1) sucedida ou nao (0)
* @args arquivo de entrada, arquivo de saida, string a ser salva, tamanho do
* campo e nome do campo.
*/
int salvaCampoVariavelAlt(FILE *fEntrada, FILE *fSaida, char *string, int *tamCampo, char *nomeCampo);

/*
*    int salvaCampoFixoAlt(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, 
* char **string, int *tamCampo, int limite, char *nomeCampo)
* Funcao responsavel por salvar um campo de tamanho fixo de um arquivo de entrada
* em um arquivo de saida.
* @return inteiro que diz se a funcao foi bem sucedida (1) ou nao (0)
* @args arquivo de entrada, arquivo de saida, string a ser salva, tamanho do
* campo, limite do tamanho e nome do campo.
*/
int salvaCampoFixoAlt(FILE *, FILE *, char **, int *, int , char *);

/*
*    void visualizaCampoVariavel(FILE *arquivo, char *nomeCampo)
* Funcao responsavel por imprimir um campo de tamanho variavel existente em um
* arquivo
* @return vazio
* @args arquivo o qual se le e o nome do campo de tamanho variavel
*/
void visualizaCampoVariavel(FILE *, char *);

/*
*    void visualizaCampoFixo(FILE *arquivo, char *nomeCampo, int tamanho);
* Funcao responsavel por imprimir um campo fixo existente em um arquivo,
* @return vazio
* @args arquivo o qual se le, nome do campo do tipo long e tamanho do campo
*/
void visualizaCampoFixo(FILE *, char *, int );

/*
*    void visualizaCampoLong(FILE *arquivo, char *nomeCampo);
* Funcao responsavel por imprimir um campo do tipo long existente em um
* arquivo passado como argumento.
* @return vazio
* @args arquivo o qual se le e o nome do campo do tipo long
*/
void visualizaCampoLong(FILE *, char *);

/*
*    void visualizar();
* Funcao responsavel por imprimir todos os registro existentes no arquivo de
* saida gerado pelo programa.
* @return vazio
* @args vazio
*/
void visualizar();

/*
*    void buscaDominio();
* Funcao responsavel por buscar todos os registros que possuem o dominio buscado.
* @return vazio
* @args vazio
*/
void buscaDominio();

/*
*    void registroRRN();
* Funcao responsavel por imprimir na tela um registro inteiro do RRN escolhido.
* @return vazio
* @args vazio
*/
void registroRRN();

/*
*    void campoRegistro();
* Funcao responsavel por imprimir na tela um campo pertencente a um registro.
* O campo e o registro sao escolhidos pelo usuario.
* @return vazio
* @args vazio
*/
void campoRegistro();

/*
*    void ignorarCampoVariavel(FILE *arquivo);
* Funcao responsavel por ignonorar um campo variavel, ie, pular um campo de
* tamanho variavel na leitura de um arquivo.
* @return vazio
* @args arquivo que se le
*/
void ignorarCampoVariavel(FILE *fSaida);

/*
*    void ignorarRegistro(FILE *arquivo);
* Funcao responsavel por ignonorar um registro, ie, pular um registro de um
* arquivo.
* @return vazio
* @args arquivo que se le
*/
void ignorarRegistro(FILE *);
  
/*
*    FILE *fopen_(char *nome_do_arquivo, char *modo_de_abertura);
* Funcao responsavel por ler da entrada padrao de texto a operacao que o usua_
* rio deseja realizar.
* @return caso a abertura seja bem sucessida, retorna-se o arquivo aberto, caso
* a acao e abortada e o programa encerrado (exit(1)).
* @args vazio
*/
FILE *fopen_(char *, char *);

/*
*    int lerOperacao();
* Funcao responsavel por ler da entrada padrao de texto a operacao que o usua_
* rio deseja realizar.
* @return inteiro que representa a operacao
* @args vazio
*/
int lerOperacao();

/*
*    void imprimirSaudacoes();
* Funcao responsavel por imprimir na tela as saudacoes do sistema criado.
* @return vazio
* @args vazio
*/
void imprimirSaudacoes();

#endif
