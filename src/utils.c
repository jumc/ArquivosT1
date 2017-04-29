#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <regexCustom.h>
#include <utils.h>


char *lerLinha(FILE *fp, char delimiter){
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

int lerInt(FILE *fp){
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

long int lerLong(FILE *fp){
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
        // ------------------------------ PENSAR NO CASO NULO
        tamCampo = 0;
    } else {
        if(tamCampo != limite){
            printf("::%s de tamanho invalido::\n", nomeCampo);
            return 0;
        }

        if(!salvaString(string, tamCampo, fSaida)){
            free(string);
            printf("::Erro ao salvar %s::\n", nomeCampo);
            return 0;
        }
    }       

    free(string);
    return 1;
}

int salvaCampoLong(FILE *fEntrada, FILE *fSaida, char *nomeCampo){
// Salva um campo do tipo long em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    long int n;

    n = lerLong(fEntrada);
    // Se o valor do campo for string ou nulo salvara como LONG_MIN
    if(!salvaLong(&n, fSaida)){
        printf("::Erro ao salvar %s::\n", nomeCampo);
        return 0;
    }
    
    return 1;
}



// ----------------------- REFERENCIA
void recuperar(FILE *fp){
    int reg_len, field_len, idade, i, counter = 0;
    char *string = NULL;
    
    fseek(fp,0,SEEK_SET);
    printf("\n---------- DADOS DO SISTEMA -------------\n");
    while(!feof(fp)){
        if(counter == 10){
            counter = 0;
            printf(":: Aperte ENTER para continuar o browsing ::\n");
            getchar();
        }
        
        fread(&reg_len,sizeof(int),1,fp);
        
        if(!feof(fp)){
            printf("| ");
            for(i = 0; i < 3; i++){
                fread(&field_len,sizeof(int),1,fp);
                if(field_len > 0){
                    string = (char *)realloc(string,sizeof(char)*field_len+1);
                    fread(string,sizeof(char),field_len,fp);
                    string[field_len] = '\0';
                    printf("%s\t",string);
                    free(string);
                    string = NULL;
                }   
            }    
            fread(&field_len,sizeof(int),1,fp);
            if(field_len > 0){
                fread(&idade,sizeof(int),1,fp);
                printf("%d\t",idade);
            }
            printf("\n");
        }
        counter++;
    }
    fseek(fp,0,SEEK_END);
    printf("\n:: Fim do browsing ::\n");
    return;
}