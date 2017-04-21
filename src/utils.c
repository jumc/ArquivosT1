#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regexCustom.h>
#include <utils.h>

char *readLine(FILE *fp){
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

void escrever(FILE *output_file){
    //Esta funcao escreve em um arquivo ate o momento em que o usuario nao digitar nada ou
    //digitar o comando PARAR ou SAIR...

    // Variaveis da funcao
    int flag = 1;
    char *string = NULL;
    
    //1. Imprimindo na tela o funcionamento da funcao...
    printf("\n--------------------\n| *ESCRITA*\n");
    printf("| A insercao de registros ira acabar se voce der entrada em um registro vazio ou digitar o comando PARAR, SAIR ou 0.\n");
    printf("| Digite os campos no formato: \"nome sobrenome email idade\"\n");
    printf("| Para correta escrita do arquivo, respeite a ordem de entrada\n");
    
    //2. Iniciando a escrita...
    while(flag){
        //a. Lendo a string...
        printf("> ");
        string = readLine(stdin);
        if(match(string,"^\\s*[0]\\s*$")) flag = 0;
        if(match(string,"^\\s*[Ss][Aa][Ii][Rr]\\s*$")) flag = 0;
        if(match(string,"^\\s*[Pp][Aa][Rr][Aa][Rr]\\s*$")) flag = 0;
        if(match(string,"^\\s*$")) flag = 0;
        
        //b. Analisando qual é o caso tratado...
        if(flag){

            //a. todos os atributos
            if(match(string,"^\\s*[a-zA-Z]+\\s+[a-zA-Z]+\\s+[a-zA-Z\\.\\@\\_\\-]+\\s+[0-99999]+\\s*$")){
                printf("::Todos atributos digitados::\n");
                int f[4] = {1, 1, 1, 1};
                escreverRegistro(f, string, output_file);
            }
            //b. nome sobrenome e idade
            else if(match(string,"^\\s*[a-zA-Z]+\\s+[a-zA-Z]+\\s+[0-99999]+\\s*$")){
                printf("::nome,sobrenome,idade::\n");
                int f[4] = {1, 1, 0, 1};
                escreverRegistro(f, string, output_file);
            }
            //c.nome email e idade
            else if(match(string,"^\\s*[a-zA-Z]+\\s+[a-zA-Z\\.\\@\\_\\-]+\\s+[0-99999]+\\s*$")){
                printf("::nome,email,idade::\n");
                int f[4] = {1, 0, 1, 1};
                escreverRegistro(f, string, output_file);
            }
            //d.nome sobrenome e email
            else if(match(string,"^\\s*[a-zA-Z]+\\s+[a-zA-Z]+\\s+[a-zA-Z\\.\\@\\_\\-]+\\s*$")){
                printf("::nome,sobrenome,email::\n");
                int f[4] = {1, 1, 1, 0};
                escreverRegistro(f, string, output_file);
            }
            //e. nome e idade
            else if(match(string,"^\\s*[a-zA-Z]+\\s+[0-99999]+\\s*$")){
                printf("::nome,idade::\n");
                int f[4] = {1, 0, 0, 1};
                escreverRegistro(f, string, output_file);
            }
            //f. email e idade
            else if(match(string,"^\\s*[a-zA-Z\\.\\@\\_\\-]+\\s+[0-99999]+\\s*$")){
                printf("::email,idade::\n");
                int f[4] = {0, 0, 1, 1};
                escreverRegistro(f, string, output_file);
            }
            //g. nome e sobrenome
            else if(match(string,"^\\s*[a-zA-Z]+\\s+[a-zA-Z]+\\s*$")){
                printf("::nome,sobrenome::\n");
                int f[4] = {1, 1, 0, 0};
                escreverRegistro(f, string, output_file);
            }
            //h. nome e email
            else if(match(string,"^\\s*[a-zA-Z]+\\s+[a-zA-Z\\.\\@\\_\\-]+\\s*$")){
                printf("::nome,email::\n");
                int f[4] = {1, 0, 1, 0};
                escreverRegistro(f, string, output_file);
            }
            //nome
            else if(match(string,"^\\s*[a-zA-Z]+\\s*$")){
                printf("::nome::\n");
                int f[4] = {1, 0, 0, 0};
                escreverRegistro(f, string, output_file);
            }
            //email
            else if(match(string,"^\\s*[a-zA-Z\\.\\@\\_\\-]+\\s*$")){
                printf("::email::\n");
                int f[4] = {0, 0, 1, 0};
                escreverRegistro(f, string, output_file);
            }
            //idade
            else if(match(string,"^\\s*[0-99999]+\\s*$")){
                printf("::idade::\n");
                int f[4] = {0, 0, 0, 1};
                escreverRegistro(f, string, output_file);
            }
        }
        //c. Liberando a string lida...
        free(string);
    }
    //3. Encerrando a funcao...
    return;
}

int operate_select(){
    int op = INVALID;
    char *string = readLine(stdin);
    if(string){
        if(match(string,"^\\s*[Ee][Ss][Cc][Rr][Ii][Tt][Aa]\\s*$"))
            op = ESCRITA;
        if(match(string,"^\\s*[Rr][Ee][Cc][Uu][Pp][Ee][Rr][Aa][Rr]\\s*$"))
            op = REC;
        if(match(string,"^\\s*[Ss][Aa][Ii][Rr]\\s*$"))
            op = atoi(string);
        if(match(string,"^\\s*[0-99999]\\s*$"))
            op = atoi(string);
        if(op > 3 || op < 0) op = -1;
        free(string);
    }
    return op;
}