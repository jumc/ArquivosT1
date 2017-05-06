#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <regexCustom.h>
#include <utils.h>
#include <delimitador.h>

/* A FAZER
--> Funcao para descobrir com que tipo de registros foi construido o arquivo
--> Mascara para documento
--> Mascara para timestamp
--> Visualizacao de nulo
--> Truncar campo fixos
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

    escrito = fwrite(string, sizeof(char), tamCampo, fSaida);
    if(escrito != tamCampo) // Caso nao conseguiu escrever todos os bytes do texto...
        return 0;

    return 1;
}

int salvaInt(int *n, FILE *fSaida){
// Salva um int em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    int escrito;

    escrito = fwrite(n, sizeof(int), 1, fSaida);
    if(escrito != 1) // Caso nao conseguiu escrever todos os bytes do int...
        return 0;

    return 1;
}

int salvaLong(long int *n, FILE *fSaida){
// Salva um long int em um arquivo de saida. Retorna 1 caso obtenha sucesso, 0 caso contrario
    int escrito;

    escrito = fwrite(n, sizeof(long int), 1, fSaida);
    if(escrito != 1) // Caso nao conseguiu escrever todos os bytes do long int...
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

int salvaCampoVariavelAlt(FILE *fEntrada, FILE *fSaida, char *string, int *tamCampo, char *nomeCampo){
// Salva um campo de tamanho variavel em um arquivo de saida utilizando
// o metodo de indicador de tamanho. Retorna a quantidade de bytes escritos
// caso obtenha sucesso, -1 caso contrario   

    if(match(string, "^\\s*[Nn][Uu][Ll][Ll]\\s*$")){ // Se o valor do campo for nulo...
        *tamCampo = 0;
    }

    if(!salvaInt(tamCampo, fSaida)){
        printf("::Erro ao salvar tamanho do %s::\n", nomeCampo);
        return -1;
    }

    if(!salvaString(string, *tamCampo, fSaida)){
        printf("::Erro ao salvar %s::\n", nomeCampo);
        return -1;
    }

    return 1;
}


int salvaCampoFixoAlt(FILE *fEntrada, FILE *fSaida, char *string, int *tamCampo, int limite, char *nomeCampo){
    
    if(match(string, "^\\s*[Nn][Uu][Ll][Ll]\\s*$")){ 
        // Se o valor do campo for nulo...
        // ------------------------------ PENSAR NO CASO NULO
        *tamCampo = 0;

    } else {
        if(*tamCampo != limite) {
            printf("::%s de tamanho invalido::\n", nomeCampo);
            return 0;
        }

        if(!salvaString(string, *tamCampo, fSaida)) {
            printf("::Erro ao salvar %s::\n", nomeCampo);
            return 0;
        }
    }

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

int tipoRegistro(fSaida) {
    int n;
    fread(&n, sizeof(int), 1, fSaida);

    return n;
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

bool buscaTotalDominioIndicador(char *dominioProcurado,FILE *arquivo){
    //0. Variaveis da funcao
    bool flag_encontrou = FALSE;
    int tamanhoDoRegistro = 0, tamanhoDoCampo = 0, RRN = 0;  
    long ticket = 0, bytesLidos = 0;
    char *dominio = NULL, *campo = NULL;

    //1. Iniciando a busca em um arquivo com indicadores de tamanho...
    while(!feof(arquivo)){
        //a. Lendo o tamanho do registro...
        bytesLidos += fread(&tamanhoDoRegistro,sizeof(int),1,arquivo);
        //b. Lendo o tamanho do dominio...
        bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
        //c. Lendo o dominio...
        dominio = (char *)malloc(sizeof(char)*tamanhoDoCampo);
        bytesLidos += fread(&dominio,sizeof(char),tamanhoDoCampo,arquivo);
        
        //d. Atualizando o tamanho do registro...
        tamanhoDoRegistro -= (tamanhoDoCampo + sizeof(int));
        
        //e. Caso o dominio do registro seja o dominio procurado imprimisse na
        //tela o registro completo...
        //- Caso contrario, pula-se para o proximo dominio...
        if(strcmp(dominio,dominioProcurado)==0){
            //atualizando a flag de retorno
            flag_encontrou = TRUE;
            
            //i. imprimindo o dominio
            printf("RRN = %d\nDominio: %s\n",RRN,dominio);
            
            //ii. imprimindo documento
                tamanhoDoCampo = 19;
                campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
                    bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
                    printf("Documento: %s\n",campo);
                free(campo);
            //iii. imprimindo nome
                bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
                campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
                    bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
                    printf("Nome: %s\n",campo);
                free(campo);
            //iv. imprimindo cidade
                bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
                campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
                    bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
                    printf("Cidade: %s\n",campo);
                free(campo);            
            //v. imprimindo uf
                bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
                campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
                    bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
                    printf("UF: %s\n",campo);
                free(campo);            
            //vi. imprimindo data hora
                tamanhoDoCampo = 19;
                campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
                    bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
                    printf("Data e Hora (Criacao): %s\n",campo);
                free(campo);           
            //vii. imprimindo data hora atualiza
                tamanhoDoCampo = 19;
                campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
                    bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
                    printf("Data e Hora (Atualizacao): %s\n",campo);
                free(campo);            
            //viii. imprimindo ticket
                bytesLidos += fread(&ticket,sizeof(long),1,arquivo);
                printf("Ticket: %ld\n",ticket);
        }
        else
            fseek(arquivo,SEEK_CUR,tamanhoDoRegistro);
        
        //f. Incrementando o RRN...
        RRN++;
        
        //g. Liberando o dominio lido e continuando a buscar...
        free(dominio);
    }
    
    //2. Encerrando a busca...
    return flag_encontrou;
}

bool buscaTotalDominioDelimtitador(char *dominioProcurado, FILE *arquivo){
    //0. Variaveis da funcao
    bool flag_encontrou = FALSE, flag_imprime = FALSE;
    char delimitador = '#';
    int tamanhoDoRegistro = 0, tamanhoDoCampo = 0, RRN = 0;  
    long ticket = 0, bytesLidos = 0;
    char *dominio = NULL, *campo = NULL;

    //1. Iniciando a busca em um arquivo com indicadores delimitadores de
    //registros...
    while(!feof(arquivo)){
        //a. Lendo o tamanho do dominio...
        bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
        //b. Lendo o dominio...
        dominio = (char *)malloc(sizeof(char)*tamanhoDoCampo);
        bytesLidos += fread(&dominio,sizeof(char),tamanhoDoCampo,arquivo);
        
        //c. Caso o dominio do registro seja o dominio procurado imprimisse na
        //tela o registro completo...
        //- Caso contrario, pula-se para o proximo dominio...
        if(strcmp(dominio,dominioProcurado)==0){
            flag_encontrou = TRUE;
            flag_imprime = TRUE;
        }
            
            //i. imprimindo o dominio caso o dominio lido seja o dominio procurado
            if(flag_imprime) printf("RRN = %d\nDominio: %s\n",RRN,dominio);
            //ii. imprimindo documento caso o dominio lido seja o dominio procurado
            tamanhoDoCampo = 19;
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Documento: %s\n",campo);
            free(campo);
            //iii. imprimindo nome caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Nome: %s\n",campo);
            free(campo);
            //iv. imprimindo cidade caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Cidade: %s\n",campo);
            free(campo);            
            //v. imprimindo uf caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("UF: %s\n",campo);
            free(campo);            
            //vi. imprimindo data hora caso o dominio lido seja o dominio procurado
            tamanhoDoCampo = 19;
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Data e Hora (Criacao): %s\n",campo);
            free(campo);           
            //vii. imprimindo data hora atualiza caso o dominio lido seja o dominio procurado
            tamanhoDoCampo = 19;
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Data e Hora (Atualizacao): %s\n",campo);
            free(campo);            
            //viii. imprimindo ticket caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&ticket,sizeof(long),1,arquivo);
            if(flag_imprime)printf("Ticket: %ld\n",ticket);
        
        //d. Incrementando o RRN...
        delimitador = fgetc(arquivo);
        RRN++;
        
        //e. Liberando o dominio lido e continuando a buscar...
        free(dominio);
        flag_imprime = FALSE;
    }
    
    //2. Encerrando a busca...
    return flag_encontrou;
}

bool buscaTotalDominioCFixos(char *dominioProcurado, FILE *arquivo){
    //0. Variaveis da funcao
    bool flag_encontrou = FALSE, flag_imprime = FALSE;
    int tamanhoDoRegistro = 0, tamanhoDoCampo = 0, RRN = 0;  
    long ticket = 0, bytesLidos = 0;
    char *dominio = NULL, *campo = NULL;

    //1. Iniciando a busca em um arquivo com numero fixo de campo...
    while(!feof(arquivo)){
        //a. Lendo o tamanho do dominio...
        bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
        //b. Lendo o dominio...
        dominio = (char *)malloc(sizeof(char)*tamanhoDoCampo);
        bytesLidos += fread(&dominio,sizeof(char),tamanhoDoCampo,arquivo);
        
        //c. Caso o dominio do registro seja o dominio procurado imprimisse na
        //tela o registro completo...
        //- Caso contrario, pula-se para o proximo dominio...
        if(strcmp(dominio,dominioProcurado)==0){
            flag_encontrou = TRUE;
            flag_imprime = TRUE;
        }
            
            //i. imprimindo o dominio caso o dominio lido seja o dominio procurado
            if(flag_imprime) printf("RRN = %d\nDominio: %s\n",RRN,dominio);
            //ii. imprimindo documento caso o dominio lido seja o dominio procurado
            tamanhoDoCampo = 19;
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Documento: %s\n",campo);
            free(campo);
            //iii. imprimindo nome caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Nome: %s\n",campo);
            free(campo);
            //iv. imprimindo cidade caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Cidade: %s\n",campo);
            free(campo);            
            //v. imprimindo uf caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&tamanhoDoCampo,sizeof(int),1,arquivo);
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("UF: %s\n",campo);
            free(campo);            
            //vi. imprimindo data hora caso o dominio lido seja o dominio procurado
            tamanhoDoCampo = 19;
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Data e Hora (Criacao): %s\n",campo);
            free(campo);           
            //vii. imprimindo data hora atualiza caso o dominio lido seja o dominio procurado
            tamanhoDoCampo = 19;
            campo = (char *)malloc(sizeof(char)*tamanhoDoCampo);
            bytesLidos += fread(&campo,sizeof(char),tamanhoDoRegistro,arquivo);
            if(flag_imprime) printf("Data e Hora (Atualizacao): %s\n",campo);
            free(campo);            
            //viii. imprimindo ticket caso o dominio lido seja o dominio procurado
            bytesLidos += fread(&ticket,sizeof(long),1,arquivo);
            if(flag_imprime)printf("Ticket: %ld\n",ticket);
        
        //d. Incrementando o RRN...
        RRN++;
        
        //e. Liberando o dominio lido e continuando a buscar...
        free(dominio);
        flag_imprime = FALSE;
    }
    
    //2. Encerrando a busca...
    return flag_encontrou;
}
        

// --- void buscaDominio(char *dominio)
// Imprime na tela todos os registros que contenham o dominio especificado pelo 
//usuario realizando uma busca de acordo com o tipo de organizacao do arquivo 
//aberto para busca
void buscaDominio(char *dominio){
    //0. Variaveis da funcao
    int tipoDoArquivo = INVALID, bytesLidos = 0;
    FILE *arquivo = NULL;
    
    //1. Abrindo o arquivo que sera usado na busca...
    printf("Buscar pelo dominio: %s\n",dominio);
    arquivo = fopen_("output.txt","rb");
    
    //2. Verificando o tipo do arquivo e iniciando a busca correspondente ao
    //tipo lido...
    bytesLidos += fread(&tipoDoArquivo,sizeof(int),1,arquivo);
    switch(tipoDoArquivo){
        case INDICADOR_DE_TAMANHO:
            if(!buscaTotalDominioIndicador(dominio,arquivo))
                printf("::Dominio nao encontrado::\n");
        break;
        
        case DELIMITADOR_ENTRE_REG:
            if(!buscaTotalDominioDelimtitador(dominio,arquivo))
                printf("::Dominio nao encontrado::\n");
        break;
        
        case NUMERO_DE_CAMPS_FIXO:
            if(!buscaTotalDominioCFixos(dominio,arquivo))
                printf("::Dominio nao encontrado::\n");
        break;
        
        default:
            printf("::Erro ao ler o tipo do arquivo para busca::\n");
        break;
    }
    
    //3. Fechando o arquivo aberto e encerrando a funcao...
    fclose(arquivo);
    return;
}

void registroRRN(FILE *fSaida){
    // Mostra o registro com o RRN especificado pelo usuario, de acordo com o tipo de organizacao do arquivo de saida
    /*printf("Buscar pelo RRN: ");
    int rrn = lerCharToInt(stdin);
    printf("\n");
    int tipo = tipoRegistro(fSaida);  
    switch(tipo):
        case 0:
            registroRRNIndicador(fSaida);
            break;
        case 1: 
            registroRRNDelimitadores(fSaida);
            break;
        case 2:
            registroRRNFixo(fSaida);
            break;
        case 3:
            printf("::Arquivo vazio::\n\n");
            break;
        case 4:
            printf("::Dados gravados de maneira incorreta::\n\n");
            break;
    registroRRNDelimitadores(rrn, fSaida);
    */
}

FILE *fopen_(char *filename, char *openmode){
    FILE *fp = fopen(filename,openmode);
    if(fp == NULL){
        printf("Nao foi possivel abrir o arquivo de entrada\n");
        exit(1);
    }
    return fp;
}

int lerOperacao(){
    int operacao = INVALID;
    char *linha = lerLinha(stdin,10);
    if(linha){
        //-----------------COMANDOS PUROS COMO NO MENU (SEM NUMERO)
            //(1) gerar arquivo - indicador de tamanho
            if(match(linha,"^\\s*[a-zA-Z ]*\\s*[Ii]+[Nn]+[Dd]+[Ii]+[Cc]+[Aa]+[Dd]+[Oo]+[Rr]+\\s*[a-zA-Z ]*[Tt]+[Aa]+[Mm]+[Aa]+[Nn]+[Hh]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = INDICADOR_DE_TAMANHO;
            //(2) gerar arquivo - delimitador entre registros
            if(match(linha,"^\\s*[a-zA-Z ]*[Dd]+[Ee]+[Ll]+[Ii]+[Mm]+[Ii]+[Tt]+[Aa]+[Dd]+[Oo]+[Rr]+[Ee]*[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = DELIMITADOR_ENTRE_REG;
            //(3) gerar arquivo - campo fixos
            if(match(linha,"^\\s*[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s+[a-zA-Z ]*[Ff]+[Ii]+[Xx]+[Oo]+[Ss]+\\s*[a-zA-Z ]*$"))
                operacao = NUMERO_DE_CAMPS_FIXO;
            if(match(linha,"^\\s*[a-zA-Z ]*[Ff]+[Ii]+[Xx]+[Oo]+[Ss]*\\s+[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = NUMERO_DE_CAMPS_FIXO;
            
            if(match(linha,"^\\s*[a-zA-Z ]*\\s*[Rr]+[Rr]+[Nn]+\\s*[a-zA-Z ]*$")){
                //(6) buscar por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Rr]+[Ee]+[Gg]+[Ii]+[Ss]+[Tt]+[Rr]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VIZUALIZAR_REG_RRN;
                //(7) buscar campo por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VIZUALIZAR_CAMPO_RRN;
            }
                
        //-----------------CRIACAO
        if( match(linha,"^\\s*[a-zA-Z ]*\\s*[Gg]+[Ee]+[Rr]+[Aa]+[Rr]*\\s*[a-zA-Z ]*$")
          || match(linha,"^\\s*[a-zA-Z ]*\\s*[Cc]+[Rr]+[Ii]+[Aa]+[Rr]*\\s*[a-zA-Z ]*$")){
            //(1) gerar arquivo - indicador de tamanho
            if(match(linha,"^\\s*[a-zA-Z ]*\\s*[Tt]+[Aa]+[Mm]+[Aa]+[Nn]+[Hh]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = INDICADOR_DE_TAMANHO;
            //(2) gerar arquivo - delimitador entre registros
            if(match(linha,"^\\s*[a-zA-Z ]*[Dd]+[Ee]+[Ll]+[Ii]+[Mm]+[Ii]+[Tt]+[Aa]+[Dd]+[Oo]+[Rr]+\\s*[a-zA-Z ]*$"))
                operacao = DELIMITADOR_ENTRE_REG;
            //(3) gerar arquivo - campo fixos
            if(match(linha,"^\\s*[a-zA-Z ]*[Ff]+[Ii]+[Xx]+[Oo]+\\s*[a-zA-Z ]*$"))
                operacao = NUMERO_DE_CAMPS_FIXO;
        }
        //-----------------BUSCAS
        if( match(linha,"^\\s*[a-zA-Z ]*\\s*[Vv]+[Ii]+[Ss]+[Uu]+[Aa]+[Ll]+[Ii]+[Zz]+[Aa]+[Rr]*\\s*[a-zA-Z ]*$")
          || match(linha,"^\\s*[a-zA-Z ]*\\s*[Bb]+[Uu]+[Ss]+[Cc]+[Aa]+[Rr]*\\s*[a-zA-Z ]*$")){
            //(4) todos os registros  
            if(match(linha,"^\\s*[a-zA-Z ]*[Tt]+[Oo]+[Dd]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = VIZUALIZAR_TODOS;
            //(5) buscar por dominio
            if(match(linha,"^\\s*[a-zA-Z ]*[Dd]+[Oo]+[Mm]+[Ii]+[Nn]+[Ii]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = VIZUALIZAR_DOMINIO;
            if(match(linha,"^\\s*[a-zA-Z ]*\\s*[Rr]+[Rr]+[Nn]+\\s*[a-zA-Z ]*$")){
                //(6) buscar por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Rr]+[Ee]+[Gg]+[Ii]+[Ss]+[Tt]+[Rr]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VIZUALIZAR_REG_RRN;
                //(7) buscar campo por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VIZUALIZAR_CAMPO_RRN;
            }
        }
        //-------------SISTEMA
        //(9) menu
        if(match(linha,"^\\s*[a-zA-Z ]*[Mm]+[Ee]+[Nn]+[Uu]+\\s*[a-zA-Z ]*$")
          ||match(linha,"^\\s*[a-zA-Z ]*[Oo]+[Pp]+[Cc]+[Oo]+[Ee]+[Ss]+\\s*[a-zA-Z ]*$")
          ||match(linha,"^\\s*[a-zA-Z ]*[Oo]+[Pp]+[Ee]+[Rr]+[Aa]+[Cc]+[Oo]+[Ee]+[Ss]+\\s*[a-zA-Z ]*$"))
            operacao = MENU;
        //(0) sair
        if(match(linha,"^\\s*[a-zA-Z ]*[Ss]+[Aa]+[Ii]+[Rr]+\\s*[a-zA-Z ]*$"))
            operacao = SAIR;
        
        //--------------CODIGO NUMERICO
        if(match(linha,"^\\s*[a-zA-Z ]*[0-99999]\\s*[a-zA-Z ]*$"))
            operacao = atoi(linha);
        if(operacao > 9 || operacao < 0 || operacao == 8) 
            operacao = INVALID;
        
        free(linha);
    }
    return operacao;
}

void imprimirSaudacoes(){
    printf("|\tPROGRAMA GERADOR DE ARQUIVOS\n|\n");
    //sleep(0.8);
    printf("|\tBem vindo ao programa responsavel por gerar arquivos de acordo suas\n");
    printf("| preferencias de construcao e por procurar os dados armazenados.\n");
    //sleep(0.8);
    printf("| Os arquivos gerados possuirao dados de dominios governamentais.\n");
    //sleep(0.8);
    printf("| Voce pode escolher qualquer opcao abaixo digitando o codigo da ope-\n");
    printf("| racao desejada ou seu nome/palavra chave.\n\n");
    //sleep(1.5);
    printf("Operacoes para Gerar arquivo\n");
    //sleep(0.5);
    printf("\t(1) Indicador de tamanho\n");
    printf("\t(2) Delimitadores entre registros\n");
    printf("\t(3) Número fixo de campo\n\n");
    //sleep(1.2);
    printf("Operacoes para Recuperar arquivo\n");
    //sleep(0.5);
    printf("\t(4) Visualizar todos os registros\n");
    printf("\t(5) Busca por domínio\n");        
    printf("\t(6) Registro completo por RRN\n");
    printf("\t(7) Campo de um registro por RRN\n\n");
    //sleep(1.2);
    printf("Operacoes Gerais do Sistema\n");
    //sleep(0.5);
    printf("\t(9) Exibir menu de operacoes\n");
    printf("\t(0) Sair\n\n");
    //sleep(1.2);
    printf("| Digite a operacao que deseja realizar: ");
    return;
}