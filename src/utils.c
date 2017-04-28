#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <regexCustom.h>
#include <utils.h>


char *leLinha(FILE *fp){
    char c = 0;
    int counter = 0;
    char *string = NULL;
    
    do{
        c = fgetc(fp);
        string = (char *)realloc(string,sizeof(char)*(counter+1));
        string[counter++] = c;
    }while(c != 10 && c != 11 && c != 12 && c != 13 && c != EOF);
    string[counter-1] = '\0';
    
    return string;
}

int leInt(FILE *fp){
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

void escreverNulo(FILE *output_file){
    int aux_len = 0;
    fwrite(&aux_len,sizeof(int),1,output_file);
}

void escreverString(char *str, FILE *output_file){
    int aux_len = strlen(str);
    fwrite(&aux_len, sizeof(int), 1, output_file);
    fwrite(str, sizeof(char), strlen(str), output_file);
}

void escreverRegistro(int *flags, char *string, FILE *output_file){
    char **campos = (char **) malloc(sizeof(char *)*4); // [0] Nome [1] Sobrenome [2] Email [3] Idade
    char *aux;
    int len = 0, i, aux_len, idade;

    //Determinando tamanho do registro e obtendo campos
    for(i = 0; i < 3; i++){
        if(flags[i]){
            campos[i] = tokenize(string, i, ' ');
            len += 4 + strlen(campos[i]);
        }
    }
    if(flags[3]){
        aux = tokenize(string,3,' ');
        idade = atoi(aux);
        len += 8;
    }
    fwrite(&len,sizeof(int),1,output_file);

    //Escrevendo dados
    for(int i = 0; i < 3; i++){
        if(flags[i])
            escreverString(campos[i], output_file);
        else
            escreverNulo(output_file);
    }    
    if(flags[3]){
        aux_len = 4;
        fwrite(&aux_len,sizeof(int),1,output_file);
        fwrite(&idade,sizeof(int),1,output_file);
    } else
        escreverNulo(output_file);

    for(i = 0; i < 4; i++){
        if(flags[i])
            free(campos[i]);
    }
    free(campos);
}