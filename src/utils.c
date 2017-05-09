#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <regexCustom.h>
#include <utils.h>
#include <delimitador.h>
#include <indicador.h>
#include <fixo.h>

char *lerLinha(FILE *arquivo, int *tamanho, char delimitador) {
    // Retorna uma linha lida de um arquivo do seu ponto atual ate um delimitador, fim de arquivo, enter ou outros indicadores de fim de linha
    char c = 0;
    int contador = 0;
    char *linha = NULL;
    
    do{
        c = fgetc(arquivo);
        linha = (char *)realloc(linha, sizeof(char)*(contador+1));
        linha[contador++] = c;
    } while(c != 10 && c != 11 && c != 12 && c != 13 && c != EOF && c != delimitador);

    linha[contador-1] = '\0';
    
    *tamanho = contador - 1;

    return linha;
}

char *lerNchar(FILE *arquivo, int n){
// Retorna uma linha de n chars lida a partir de um arquivo a nao ser que encontre fim de arquivo
    char c = 0;
    int contador = 0;
    char *linha = (char *) malloc(sizeof(char) * (n + 1));
  
    do{
        c = fgetc(arquivo);
        linha[contador++] = c;
    } while(contador < n && !feof(arquivo));

    linha[contador] = '\0';
    
    return linha;
}

int lerInt(FILE *arquivo){
    int n, bytes = 0;
    bytes += fread(&n, sizeof(int), 1, arquivo);

    return n;
}

long int lerLong(FILE *arquivo){
    long int n;
    int bytes = 0;

    bytes += fread(&n, sizeof(long int), 1, arquivo);

    return n;
}

int lercharToInt(FILE *arquivo){
// Le digitos de um arquivo ate que o caracter lido nao seja um algarismo. Retorna convertido para int. Caso nao tenha lido ao menos 1 numero, retorna INT_MIN
    char c = '0';
    int contador = 0;
    char *linha = NULL;
    int i;

    while(isdigit(c)){
        c = fgetc(arquivo);
        linha = (char *)realloc(linha,sizeof(char)*(contador+1));
        linha[contador++] = c;
    };

    // Se leu ao menos digito valido
    if(contador > 1){
        linha[contador-1] = '\0';
        i = atoi(linha);
        free(linha);
        return i;
    } else{ // Se nao, retorna INT_MIN
        free(linha);
        return INT_MIN;
    }
}

long int lercharToLong(FILE *arquivo){
    char c = '0';
    int contador = 0;
    char *linha = NULL;
    long int i;

    while(isdigit(c)){
        c = fgetc(arquivo);
        linha = (char *)realloc(linha,sizeof(char)*(contador+1));
        linha[contador++] = c;
    };

    // Se leu ao menos digito valido
    if(contador > 1){
        linha[contador-1] = '\0';
        i = atol(linha);
        free(linha);
        return i;
    } else{ // Se nao, retorna INT_MIN
        free(linha);
        return LONG_MIN;
    }
}

void lerCampo(char **linha, int *tamCampo, FILE *arquivoDeEntrada){
    *linha = lerLinha(arquivoDeEntrada, tamCampo, ';');
}

int salvaLinha(char *linha, int tamCampo, FILE *arquivoDeSaida){
    int escrito;

    escrito = fwrite(linha, sizeof(char), tamCampo, arquivoDeSaida);

    if(escrito != tamCampo) // Caso nao conseguiu escrever todos os bytes do texto...
        return 0;

    return 1;
}

int salvaInt(int *n, FILE *arquivoDeSaida){
    int escrito;

    escrito = fwrite(n, sizeof(int), 1, arquivoDeSaida);

    if(escrito != 1) // Caso nao conseguiu escrever todos os bytes do int...
        return 0;

    return 1;
}

int salvaLong(long int *n, FILE *arquivoDeSaida){
    int escrito;

    escrito = fwrite(n, sizeof(long int), 1, arquivoDeSaida);

    if(escrito != 1) // Caso nao conseguiu escrever todos os bytes do long int...
        return 0;

    return 1;
}

int salvaCampoVariavel(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, char *nomeCampo){
    char *linha;
    int tamCampo;

    lerCampo(&linha, &tamCampo, arquivoDeEntrada);

    if(match(linha, "^\\s*[Nn][Uu][Ll][Ll]\\s*$")){ // Se o valor do campo for nulo...
        tamCampo = 0;
    }

    if(!salvaInt(&tamCampo, arquivoDeSaida)) {
        printf("::Erro ao salvar tamanho do %s::\n", nomeCampo);
        free(linha);
        return 0;
    }

    if(tamCampo > 0) {
        if(!salvaLinha(linha, tamCampo, arquivoDeSaida)) {
            printf("::Erro ao salvar %s::\n", nomeCampo);
            free(linha);
            return 0;
        }
    }

    free(linha);
    return 1;
}

int salvaCampoFixo(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, int limite, char *nomeCampo){
    char *linha ;
    int tamCampo;

    lerCampo(&linha, &tamCampo, arquivoDeEntrada);

    if(match(linha, "^\\s*[Nn][Uu][Ll][Ll]\\s*$")){
        // Se o valor do campo for nulo...
        free(linha);
        linha = (char *) calloc(sizeof(char),  limite);
        tamCampo = limite;
    }

    if(tamCampo != limite) {
        printf("::%s de tamanho invalido::\n", nomeCampo);
        free(linha);
        return 0;
    }

    if(!salvaLinha(linha, limite, arquivoDeSaida)) {
        printf("::Erro ao salvar %s::\n", nomeCampo);
        free(linha);
        return 0;
    }

    free(linha);
    return 1;
}

int salvaCampoLong(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, char *nomeCampo){
    long int n;

    n = lercharToLong(arquivoDeEntrada);
    // Se o valor do campo for linha ou nulo salvara como LONG_MIN
    if(!salvaLong(&n, arquivoDeSaida)){
        printf("::Erro ao salvar %s::\n", nomeCampo);
        return 0;
    }
    
    return 1;
}

char *campoVariavel(FILE *arquivo){
    int tamanho, nbytes = 0;

    nbytes += fread(&tamanho, sizeof(int), 1, arquivo);

    if(tamanho <= 0)
        return NULL;

    char *campo = lerNchar(arquivo, tamanho);

    return campo;
}

int salvaCampoVariavelAlt(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, char *linha, int *tamCampo, char *nomeCampo){
// Salva um campo de tamanho variavel em um arquivo de saida utilizando
// o metodo de indicador de tamanho. Retorna a quantidade de bytes escritos
// caso obtenha sucesso, -1 caso contrario   

    if(!salvaInt(tamCampo, arquivoDeSaida)){
        printf("::Erro ao salvar tamanho do %s::\n", nomeCampo);
        return 0;
    }

    if(*tamCampo > 0) {
        if(!salvaLinha(linha, *tamCampo, arquivoDeSaida)) {
            printf("::Erro ao salvar %s::\n", nomeCampo);
            return 0;
        }
    }

    return 1;
}

int salvaCampoFixoAlt(FILE *arquivoDeEntrada, FILE *arquivoDeSaida, char **linha, int *tamCampo, int limite, char *nomeCampo){
   if(match(*linha, "^\\s*[Nn][Uu][Ll][Ll]\\s*$")){
        // Se o valor do campo for nulo...
        free(*linha);
        *linha = (char *) calloc(sizeof(char), limite);
        *tamCampo = limite;
    }

    if((*tamCampo) != limite) {
        printf("::%s de tamanho invalido::\n", nomeCampo);
        free(*linha);
        return 0;
    }

    if(!salvaLinha(*linha, limite, arquivoDeSaida)) {
        printf("::Erro ao salvar %s::\n", nomeCampo);
        free(*linha);
        return 0;
    }

    return 1;
}

void visualizaCampoVariavel(FILE *arquivo, char *nomeCampo){
// Mostra na tela um campo de tamanho variavel
    char *campo = campoVariavel(arquivo);

    if(campo){ // Se o valor do campo nao for nulo...
        if(!feof(arquivo))
            printf("%s: %s\n", nomeCampo, campo);

        free(campo);
    } else {
        printf("%s:\n", nomeCampo);
    }
}

void visualizaCampoFixo(FILE *arquivo, char *nomeCampo, int tamanho){
    char *campo = lerNchar(arquivo, tamanho);

    if(!feof(arquivo))
        printf("%s: %s\n", nomeCampo, campo);

    free(campo);
}

void visualizaCampoLong(FILE *arquivo, char *nomeCampo) {
    long int n;
    int bytes = 0;

    bytes += fread(&n, sizeof(long int), 1, arquivo);
    if(n == LONG_MIN){
        if(!feof(arquivo))
            printf("%s:\n", nomeCampo);
    } else {
        printf("%s: %ld\n", nomeCampo, n);
    }
    return;
}

void visualizar(){
    int tipo;
    FILE *arquivoDeSaida = fopen_("output.txt", "r");

    if(fread(&tipo, sizeof(int), 1, arquivoDeSaida) > 0) {

        switch(tipo) {
            case INDICADOR_DE_TAMANHO:
                visualizarIndicador(arquivoDeSaida);
                break;
            case DELIMITADOR_ENTRE_REG: 
                visualizarDelimitadores(arquivoDeSaida);
                break;
            case NUMERO_DE_CAMPS_FIXO:
                visualizarFixo(arquivoDeSaida);
                break;
            default :
                printf("::Arquivo não inicializado ou com formatação incorreta::\n\n");
        }
    }

    fclose(arquivoDeSaida);
}

void buscaDominio(){
    printf("Buscar pelo dominio: ");
    FILE *arquivoDeSaida = fopen_("output.txt", "rb");
    int tipo, tamanho;
    char *dominioBuscado = lerLinha(stdin, &tamanho, '\n');
    printf("\n");

    if(fread(&tipo, sizeof(int), 1, arquivoDeSaida) > 0) {
        switch(tipo) {
            case INDICADOR_DE_TAMANHO:
                buscaDominioIndicador(arquivoDeSaida, dominioBuscado);
                break;
            case DELIMITADOR_ENTRE_REG: 
                buscaDominioDelimitadores(arquivoDeSaida, dominioBuscado);
                break;
            case NUMERO_DE_CAMPS_FIXO:
                buscaDominioFixo(arquivoDeSaida, dominioBuscado);
                break;
            default :
                printf("::Arquivo não inicializado ou com formatação incorreta::\n\n");
        }
    }    

    if(dominioBuscado != NULL) free(dominioBuscado);
    fclose(arquivoDeSaida);
}

void registroRRN(){
    // Mostra o registro com o RRN especificado pelo usuario, de acordo com o tipo de organizacao do arquivo de saida
    FILE *arquivoDeSaida = fopen_("output.txt", "rb");
    
    printf("Buscar pelo RRN: ");
    int rrn = lercharToInt(stdin);
    printf("\n");
    
    int tipo;  
    if(fread(&tipo, sizeof(int), 1, arquivoDeSaida) > 0) {
        switch(tipo) {
            case INDICADOR_DE_TAMANHO:
                registroRRNIndicador(arquivoDeSaida, rrn);
                break;
            case DELIMITADOR_ENTRE_REG: 
                registroRRNDelimitadores(arquivoDeSaida, rrn);
                break;
            case NUMERO_DE_CAMPS_FIXO:
                registroRRNFixo(arquivoDeSaida, rrn);
                break;
            default :
                printf("::Arquivo não inicializado ou com formatação incorreta::\n\n");
        }
    }    
    fclose(arquivoDeSaida);
}

void campoRegistro(){
// Mostra na tela um campo especifico de um registro. Usuario fornece campo e rrn  
    FILE *arquivoDeSaida = fopen_("output.txt", "rb");
    
    printf("Buscar pelo RRN: ");
    int rrn = lercharToInt(stdin);
    printf("Digite o codigo do campo desejado:\n");
    printf("\t(1) Dominio\n");
    printf("\t(2) Documento\n");
    printf("\t(3) Nome\n");
    printf("\t(4) UF\n");
    printf("\t(5) Cidade\n");
    printf("\t(6) dataHoraCadastro\n");
    printf("\t(7) dataHoraAtualiza\n");
    printf("\t(8) Ticket\n");
    int campo = lercharToInt(stdin);
    printf("\n");

    int tipo;  
    if(fread(&tipo, sizeof(int), 1, arquivoDeSaida) > 0) {
        switch(tipo) {
            case INDICADOR_DE_TAMANHO:
                campoRegistroIndicador(arquivoDeSaida, rrn, campo);
                break;
            case DELIMITADOR_ENTRE_REG: 
                campoRegistroDelimitadores(arquivoDeSaida, rrn, campo);
                break;
            case NUMERO_DE_CAMPS_FIXO:
                campoRegistroFixo(arquivoDeSaida, rrn, campo);
                break;
            default :
                printf("::Arquivo não inicializado ou com formatação incorreta::\n\n");
        }
    }    
    fclose(arquivoDeSaida);
}

void ignorarCampoVariavel(FILE *arquivo){
	int tamanho;
	fread(&tamanho, sizeof(int), 1, arquivo);
	fseek(arquivo, tamanho, SEEK_CUR);
}

void ignorarRegistro(FILE *arquivo){
	fseek(arquivo, 19, SEEK_CUR);
	
	ignorarCampoVariavel(arquivo);
	ignorarCampoVariavel(arquivo);
	ignorarCampoVariavel(arquivo);
	
	fseek(arquivo, 19, SEEK_CUR);
	fseek(arquivo, 19, SEEK_CUR);
	fseek(arquivo, sizeof(long), SEEK_CUR);
	return;
}

FILE *fopen_(char *nomeDoArquivo, char *modoDeOperacao){
    FILE *arquivo = fopen(nomeDoArquivo,modoDeOperacao);
    if(arquivo == NULL){
        printf("Nao foi possivel abrir o arquivo de entrada\n");
        exit(1);
    }
    return arquivo;
}

int lerOperacao(){
    int operacao = INVALID, tamanho;
    char *linha = lerLinha(stdin, &tamanho, 10);
    if(linha){
        //-----------------COMANDOS PUROS COMO NO MENU (SEM NUMERO)
            //(1) gerar arquivo - indicador de tamanho
            if(match(linha,"^\\s*[a-zA-Z ]*\\s*[Ii]+[Nn]+[Dd]+[Ii]+[Cc]+[Aa]+[Dd]+[Oo]+[Rr]+\\s*[a-zA-Z ]*[Tt]+[Aa]+[Mm]+[Aa]+[Nn]+[Hh]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = INDICADOR_DE_TAMANHO;
            //(2) gerar arquivo - delimitador entre registros
            if(match(linha,"^\\s*[a-zA-Z ]*[Dd]+[Ee]+[Ll]+[Ii]+[Mm]+[Ii]+[Tt]+[Aa]+[Dd]+[Oo]+[Rr]+[Ee]*[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = DELIMITADOR_ENTRE_REG;
            //(3) gerar arquivo - campos fixos
            if(match(linha,"^\\s*[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s+[a-zA-Z ]*[Ff]+[Ii]+[Xx]+[Oo]+[Ss]+\\s*[a-zA-Z ]*$"))
                operacao = NUMERO_DE_CAMPS_FIXO;
            if(match(linha,"^\\s*[a-zA-Z ]*[Ff]+[Ii]+[Xx]+[Oo]+[Ss]*\\s+[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = NUMERO_DE_CAMPS_FIXO;
            
            if(match(linha,"^\\s*[a-zA-Z ]*\\s*[Rr]+[Rr]+[Nn]+\\s*[a-zA-Z ]*$")){
                //(6) buscar por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Rr]+[Ee]+[Gg]+[Ii]+[Ss]+[Tt]+[Rr]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VISUALIZAR_REG_RRN;
                //(7) buscar campo por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VISUALIZAR_CAMPO_RRN;
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
            //(3) gerar arquivo - campos fixos
            if(match(linha,"^\\s*[a-zA-Z ]*[Ff]+[Ii]+[Xx]+[Oo]+\\s*[a-zA-Z ]*$"))
                operacao = NUMERO_DE_CAMPS_FIXO;
        }
        //-----------------BUSCAS
        if( match(linha,"^\\s*[a-zA-Z ]*\\s*[Vv]+[Ii]+[Ss]+[Uu]+[Aa]+[Ll]+[Ii]+[Zz]+[Aa]+[Rr]*\\s*[a-zA-Z ]*$")
          || match(linha,"^\\s*[a-zA-Z ]*\\s*[Bb]+[Uu]+[Ss]+[Cc]+[Aa]+[Rr]*\\s*[a-zA-Z ]*$")){
            //(4) todos os registros  
            if(match(linha,"^\\s*[a-zA-Z ]*[Tt]+[Oo]+[Dd]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = VISUALIZAR_TODOS;
            //(5) buscar por dominio
            if(match(linha,"^\\s*[a-zA-Z ]*[Dd]+[Oo]+[Mm]+[Ii]+[Nn]+[Ii]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                operacao = VISUALIZAR_DOMINIO;
            if(match(linha,"^\\s*[a-zA-Z ]*\\s*[Rr]+[Rr]+[Nn]+\\s*[a-zA-Z ]*$")){
                //(6) buscar por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Rr]+[Ee]+[Gg]+[Ii]+[Ss]+[Tt]+[Rr]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VISUALIZAR_REG_RRN;
                //(7) buscar campo por rnn
                if(match(linha,"^\\s*[a-zA-Z ]*[Cc]+[Aa]+[Mm]+[Pp]+[Oo]+[Ss]*\\s*[a-zA-Z ]*$"))
                    operacao = VISUALIZAR_CAMPO_RRN;
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
    printf("|\tBem vindo ao programa responsavel por gerar arquivos de acordo suas\n");
    printf("| preferencias de construcao e por procurar os dados armazenados.\n");
    printf("| Os arquivos gerados possuirao dados de dominios governamentais.\n");
    printf("| Voce pode escolher qualquer opcao abaixo digitando o codigo da ope-\n");
    printf("| racao desejada ou seu nome/palavra chave.\n\n");
    printf("Operacoes para Gerar arquivo\n");
    printf("\t(1) Indicador de tamanho\n");
    printf("\t(2) Delimitadores entre registros\n");
    printf("\t(3) Número fixo de campos\n\n");
    printf("Operacoes para Recuperar arquivo\n");
    printf("\t(4) Visualizar todos os registros\n");
    printf("\t(5) Busca por domínio\n");        
    printf("\t(6) Registro completo por RRN\n");
    printf("\t(7) Campo de um registro por RRN\n\n");
    printf("Operacoes Gerais do Sistema\n");
    printf("\t(9) Exibir menu de operacoes\n");
    printf("\t(0) Sair\n\n");
    return;
}