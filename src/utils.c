#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <regexCustom.h>
#include <utils.h>
#include <delimitador.h>

/* A FAZER

--> Funcao para descobrir com que tipo de registros foi construido o arquivo
--> Mascara para documento
--> Mascara para timestamp
--> Visualizacao de nulo
--> Truncar campos fixos
--> Visualizar acentos e caracteres especiais
--> Tirar espacos e colocar em upper na comparacao da busca
*/

char *lerLinha(FILE *fp, char delimiter){
// Retorna uma string lida de um arquivo do seu ponto atual ate um delimitador, fim de arquivo, enter ou outros indicadores de fim de linha
    char c = 0;
    int counter = 0;
    char *string = NULL;
    
    do{
        c = fgetc(fp);
        string = (char *)realloc(string,sizeof(char)*(counter+1));
        string[counter++] = c;
    }while(c != 10 && c != 11 && c != 12 && c != 13 && c != EOF && c != delimiter);
    string[counter-1] = '\0';
    
    return string;
}

char *lerNChar(FILE *fp, int n){
// Retorna uma string de n chars lida a partir de um arquivo a nao ser que encontre fim de arquivo
    char c = 0;
    int counter = 0;
    char *string = NULL;
    
    do{
        c = fgetc(fp);
        string = (char *)realloc(string,sizeof(char)*(counter+1));
        string[counter++] = c;
    }while(counter < n && c != EOF);
    string = (char *)realloc(string,sizeof(char)*(counter));
    string[counter] = '\0';
    
    return string;
}

int lerCharToInt(FILE *fp){
// Le digitos de um arquivo ate que o caracter lido nao seja um algarismo. Retorna convertido para int. Caso nao tenha lido ao menos 1 numero, retorna INT_MIN
    char c = '0';
    int counter = 0;
    char *string = NULL;
    int i;

    while(isdigit(c)){
        c = fgetc(fp);
        string = (char *)realloc(string,sizeof(char)*(counter+1));
        string[counter++] = c;
    };

    // Se leu ao menos digito valido
    if(counter > 1){
        string[counter-1] = '\0';
        i = atoi(string);
        free(string);
        return i;
    } else{ // Se nao, retorna INT_MIN
        free(string);
        return INT_MIN;
    }
}

long int lerCharToLong(FILE *fp){
// Le digitos de um arquivo ate que o caracter lido nao seja um algarismo. Retorna convertido para long int. Caso nao tenha lido ao menos 1 numero, retorna LONG_MIN
    char c = '0';
    int counter = 0;
    char *string = NULL;
    long int i;

    while(isdigit(c)){
        c = fgetc(fp);
        string = (char *)realloc(string,sizeof(char)*(counter+1));
        string[counter++] = c;
    };

    // Se leu ao menos digito valido
    if(counter > 1){
        string[counter-1] = '\0';
        i = atol(string);
        free(string);
        return i;
    } else{ // Se nao, retorna INT_MIN
        free(string);
        return LONG_MIN;
    }
}

void lerCampo(char **string, int *tamCampo, FILE *fEntrada){
// Le um campo do arquivo de entrada, isto e, da posicao atual do ponteiro de arquivo ate o proximo ';' fornecendo o conteudo do campo e seu tamanho 
    *string = lerLinha(fEntrada, ';');
    *tamCampo = strlen(*string);
}

int salvaString(char *string, int tamCampo, FILE *fSaida){
// Salva uma string em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    int escrito;

    escrito = fwrite(string, 1, tamCampo, fSaida);
    if(escrito != tamCampo) // Caso nao conseguiu escrever todos os bytes do texto...
        return 0;

    return 1;
}

int salvaInt(int *n, FILE *fSaida){
// Salva um int em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    int escrito;

    escrito = fwrite(n, 1, sizeof(int), fSaida);
    if(escrito != sizeof(int)) // Caso nao conseguiu escrever todos os bytes do int...
        return 0;

    return 1;
}

int salvaLong(long int *n, FILE *fSaida){
// Salva um long int em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    int escrito;

    escrito = fwrite(n, 1, sizeof(long int), fSaida);
    if(escrito != sizeof(long int)) // Caso nao conseguiu escrever todos os bytes do long int...
        return 0;

    return 1;
}

int salvaCampoVariavel(FILE *fEntrada, FILE *fSaida, char *nomeCampo){
// Salva um campo de tamanho variavel em um arquivo de saida utilizando o metodo de indicador de tamanho.  Retorna a quantidade de bytes escritos caso obtenha sucesso, -1 caso contrario
    char *string;
    int tamCampo;

    lerCampo(&string, &tamCampo, fEntrada);
    if(match(string, "^\\s*[Nn][Uu][Ll][Ll]\\s*$")){ // Se o valor do campo for nulo...
        tamCampo = 0;
    }
    if(!salvaInt(&tamCampo, fSaida)){
        free(string);
        printf("::Erro ao salvar tamanho do %s::\n", nomeCampo);
        return -1;
    }
    printf("%s\n", string);
    if(!salvaString(string, tamCampo, fSaida)){
        free(string);
        printf("::Erro ao salvar %s::\n", nomeCampo);
        return -1;
    }       

    free(string);
    return 1;
}

int salvaCampoFixo(FILE *fEntrada, FILE *fSaida, int limite, char *nomeCampo){
// Salva um campo de tamanho fixo em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    char *string ;
    int tamCampo;

    lerCampo(&string, &tamCampo, fEntrada);
    if(match(string, "^\\s*[Nn][Uu][Ll][Ll]\\s*$")){ // Se o valor do campo for nulo...
        free(string);
        string = (char *) calloc(sizeof(char), limite);
        tamCampo = limite;
    } else {
        if(tamCampo != limite){
            printf("::%s de tamanho invalido::\n", nomeCampo);
            free(string);
            return 0;
        }
    }
    if(!salvaString(string, tamCampo, fSaida)){
        free(string);
        printf("::Erro ao salvar %s::\n", nomeCampo);
        return 0;
    } 

    free(string);
    return 1;
}

int salvaCampoLong(FILE *fEntrada, FILE *fSaida, char *nomeCampo){
// Salva um campo do tipo long em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    long int n;

    n = lerCharToLong(fEntrada);
    // Se o valor do campo for string ou nulo salvara como LONG_MIN
    if(!salvaLong(&n, fSaida)){
        printf("::Erro ao salvar %s::\n", nomeCampo);
        return 0;
    }
    
    return 1;
}

char *campoVariavel(FILE *fp){
// Retorna o conteudo de um campo de tamanho variavel
    int tam;

    fread(&tam, sizeof(int), 1, fp);
    char *campo = lerNChar(fp, tam);

    return campo;
}

void vizualizaCampoVariavel(FILE *fp, char *nomeCampo){
// Mostra na tela um campo de tamanho variavel
    char *campo = campoVariavel(fp);
    if(campo){ // Se o valor do campo nao for nulo...
        printf("%s: %s\n", nomeCampo, campo);
        free(campo);
    } else {
        printf("%s:\n", nomeCampo);
    }
}

void vizualizaCampoFixo(FILE *fp, char *nomeCampo, int tam){
// Mostra na tela um campo de tamanho fixo
    //VER SE E NULL
    char *campo = lerNChar(fp, tam);
    printf("%s: %s\n", nomeCampo, campo);
    free(campo);
}

void vizualizaCampoLong(FILE *fp, char *nomeCampo){
// Mostra na tela um campo long
    long int n;
    fread(&n, sizeof(long int), 1, fp);
    if(n == LONG_MIN){
        printf("%s:\n", nomeCampo);
    } else {
        printf("%s: %ld\n", nomeCampo, n);
    }
}

void visualizar(FILE *fSaida){
// Mostra todos os registros na tela, de acordo com o tipo de organizacao do arquivo de saida
    /*int tipo = tipoRegistro(fSaida);  

    switch(tipo):
        case 0:
            visualizarIndicador(fSaida);
            break;
        case 1: 
            visualizarDelimitadores(fSaida);
            break;
        case 2:
            visualizarFixo(fSaida);
            break;
        case 3:
            printf("::Arquivo vazio::\n\n");
        case 4:
            printf("::Dados gravados de maneira incorreta::\n\n");*/
    visualizarDelimitadores(fSaida);
}

int buscaDominio(FILE *fSaida){
// Mostra todos os registros na que contenham o dominio especificado pelo usuario, de acordo com o tipo de organizacao do arquivo de saida
    printf("Buscar pelo dominio: ");
    char *dominioBuscado = lerLinha(stdin, ' ');
    printf("\n");
    /*int tipo = tipoRegistro(fSaida);  

    switch(tipo):
        case 0:
             return visualizarIndicador(fSaida);
            break;
        case 1: 
            return buscaDominioDelimitadores(dominioBuscado, fSaida);
            break;
        case 2:
            return visualizarFixo(fSaida);
            break;
        case 3:
            printf("::Arquivo vazio::\n\n");
            return 0;
            break;
        case 4:
            printf("::Dados gravados de maneira incorreta::\n\n");
            return 0;
            break;*/
    return buscaDominioDelimitadores(dominioBuscado, fSaida);
}