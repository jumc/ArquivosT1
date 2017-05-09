#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <utils.h>

void escreverDelimitador(){
// Escreve um arquivo de saida com os dados do arquivo de entrada organizados em registros separados por delimitador (#) com campos de tamanho fixo e variavel
	FILE *arquivoDeEntrada = fopen_("turmaA-dadosDominios.csv","rb");
	FILE *arquivoDeSaida = fopen_("output.txt","wb");

	long tamEntrada;
	char delimitador = '#';

	//Indicando tipo de organizacao do arquivo
	int type = DELIMITADOR_ENTRE_REG;
	fwrite(&type, sizeof(int), 1, arquivoDeSaida);

	// Descobrindo tamanho do arquivo de entrada
	fseek(arquivoDeEntrada, 0, SEEK_END);
	tamEntrada = ftell(arquivoDeEntrada);
	rewind(arquivoDeEntrada);

	while(ftell(arquivoDeEntrada) != tamEntrada) {
		// Dominio	
		if(!salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "dominio"))
			exit(0);
		// Documento
		if(!salvaCampoFixo(arquivoDeEntrada, arquivoDeSaida, 19, "documento"))
			exit(0);
		// Nome
		if(!salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "nome"))
			exit(0);
		// UF
		if(!salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "UF"))
			exit(0);
		// Cidade
		if(!salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "cidade"))
			exit(0);
		// Data-hora cadastro
		if(!salvaCampoFixo(arquivoDeEntrada, arquivoDeSaida, 19, "dataHoraCadastro"))
			exit(0);
		// Data-hora atualiza
		if(!salvaCampoFixo(arquivoDeEntrada, arquivoDeSaida, 19, "dataHoraAtualiza"))
			exit(0);
		// Ticket
		if(!salvaCampoLong(arquivoDeEntrada, arquivoDeSaida, "ticket"))
			exit(0);

		// Escrevendo delimitador de fim de registro
		fwrite(&delimitador, sizeof(char), 1, arquivoDeSaida);

		// Pegando o ENTER para ir para a proxima linha
		fgetc(arquivoDeEntrada);
	}

	fclose(arquivoDeEntrada);
	fclose(arquivoDeSaida);
}

void registroDelimitadores(FILE *arquivoDeSaida){
// Mostra na tela o registro atual do arquivo
	visualizaCampoVariavel(arquivoDeSaida, "Dominio");		
	visualizaCampoFixo(arquivoDeSaida, "Documento", 19);
	visualizaCampoVariavel(arquivoDeSaida, "Nome");
	visualizaCampoVariavel(arquivoDeSaida, "UF");
	visualizaCampoVariavel(arquivoDeSaida, "Cidade");
	visualizaCampoFixo(arquivoDeSaida, "dataHoraCadastro", 19);
	visualizaCampoFixo(arquivoDeSaida, "dataHoraAtualiza", 19);
	visualizaCampoLong(arquivoDeSaida, "Ticket");
	// Pegando o # para ir para o proximo registro
	fgetc(arquivoDeSaida);
}

void buscaDominioDelimitadores(FILE *arquivoDeSaida, char *dominioBuscado){
	// Busca registros com dominio igual ao fornecido pelo usuario
	char *dominioAtual;
	//calculando tamanho do offset para o registro de acordo com
	//o tamanho da string do dominio a ser buscado mais o tamanho int
	int offset = - strlen(dominioBuscado) - sizeof(int);
	
	long int tamEntrada;
	// Descobrindo tamanho do arquivo de entrada
	fseek(arquivoDeSaida, 0, SEEK_END);
	tamEntrada = ftell(arquivoDeSaida);
	fseek(arquivoDeSaida, 4, SEEK_SET);
    while(ftell(arquivoDeSaida) != tamEntrada){
	    dominioAtual = campoVariavel(arquivoDeSaida);
	    if(!strcmp(dominioAtual, dominioBuscado)){
	    	fseek(arquivoDeSaida, offset, SEEK_CUR);
	    	registroDelimitadores(arquivoDeSaida);
	    	printf("\n");
	    	if(dominioAtual != NULL) free(dominioAtual);
	    	return;
	    }
	    ignorarRegistro(arquivoDeSaida);
	    fgetc(arquivoDeSaida);
	    if(dominioAtual != NULL) free(dominioAtual);
	} 
	
	printf("::Nao foi possivel recuperar o registro::\n");
}

void visualizarDelimitadores(FILE *arquivoDeSaida){
// Mostra em tela todos os registros armazenados

	// O que esta acontecendo com o registro 167?
	int counter = 0;
	long int tamEntrada;
	// Descobrindo tamanho do arquivo de entrada
	fseek(arquivoDeSaida, 0, SEEK_END);
	tamEntrada = ftell(arquivoDeSaida);
	fseek(arquivoDeSaida, 4, SEEK_SET);
	
    while(ftell(arquivoDeSaida) != tamEntrada){
        if(counter % 10 == 0 && counter != 0){
            printf("\n:: Aperte ENTER para continuar o browsing ::");
            getchar();
        }
		printf("\nRegistro %d\n", counter);
		registroDelimitadores(arquivoDeSaida);	
		counter++;
	}
}

void registroRRNDelimitadores(FILE *arquivoDeSaida, int rrn){
// Mostra na tela um registro com rrn especificado pelo usuario
	int counter = 0;
    long int tamEntrada;
	// Descobrindo tamanho do arquivo de entrada
	fseek(arquivoDeSaida, 0, SEEK_END);
	tamEntrada = ftell(arquivoDeSaida);
	fseek(arquivoDeSaida, 4, SEEK_SET);
	
    while(ftell(arquivoDeSaida) < tamEntrada && counter < rrn) {
    	ignorarCampoVariavel(arquivoDeSaida);
	    ignorarRegistro(arquivoDeSaida);
	    fgetc(arquivoDeSaida);
	    counter++;
	}

	if(ftell(arquivoDeSaida) < tamEntrada){
    	registroDelimitadores(arquivoDeSaida);
    	printf("\n");
    } else {
    	printf("::Nao foi possivel recuperar o registro::\n");
    }
}

void campoRegistroDelimitadores(FILE *arquivoDeSaida, int rrn, int campo){
	long int tamEntrada;
	int i, counter = 0;
	// Descobrindo tamanho do arquivo de entrada
	fseek(arquivoDeSaida, 0, SEEK_END);
	tamEntrada = ftell(arquivoDeSaida);
	fseek(arquivoDeSaida, 4, SEEK_SET);
	
	//mudando
    while(ftell(arquivoDeSaida) < tamEntrada && counter < rrn) {
    	ignorarCampoVariavel(arquivoDeSaida);
	    ignorarRegistro(arquivoDeSaida);
	    fgetc(arquivoDeSaida);
	    counter++;
	}
	
	if(ftell(arquivoDeSaida) != tamEntrada){
		switch(campo){
			case 1:
				visualizaCampoVariavel(arquivoDeSaida, "Dominio");	
				break;
			case 2:
				ignorarCampoVariavel(arquivoDeSaida);
				visualizaCampoFixo(arquivoDeSaida, "Documento", 19);
				break;
			case 3:
				ignorarCampoVariavel(arquivoDeSaida);
				fseek(arquivoDeSaida,19,SEEK_CUR);
				visualizaCampoVariavel(arquivoDeSaida, "Nome");
				break;
			case 4:
				ignorarCampoVariavel(arquivoDeSaida);
				fseek(arquivoDeSaida,19,SEEK_CUR);
				ignorarCampoVariavel(arquivoDeSaida);
				visualizaCampoVariavel(arquivoDeSaida, "UF");
				break;
			case 5:
				ignorarCampoVariavel(arquivoDeSaida);
				fseek(arquivoDeSaida,19,SEEK_CUR);
				for(i = 0; i < 2; i++){
					ignorarCampoVariavel(arquivoDeSaida);	
				}
				visualizaCampoVariavel(arquivoDeSaida, "Cidade");
				break;
			case 6:
				ignorarCampoVariavel(arquivoDeSaida);
				fseek(arquivoDeSaida,19,SEEK_CUR);
				for(i = 0; i < 3; i++){
					ignorarCampoVariavel(arquivoDeSaida);	
				}
				visualizaCampoFixo(arquivoDeSaida, "dataHoraCadastro", 19);
				break;
			case 7:
				ignorarCampoVariavel(arquivoDeSaida);
				fseek(arquivoDeSaida,19,SEEK_CUR);
				for(i = 0; i < 3; i++){
					ignorarCampoVariavel(arquivoDeSaida);	
				}				
				fseek(arquivoDeSaida,19,SEEK_CUR);
				visualizaCampoFixo(arquivoDeSaida, "dataHoraAtualiza", 19);
				break;
			case 8:
				ignorarCampoVariavel(arquivoDeSaida);
				fseek(arquivoDeSaida,19,SEEK_CUR);
				for(i = 0; i < 3; i++){
					ignorarCampoVariavel(arquivoDeSaida);	
				}				
				fseek(arquivoDeSaida,19 * 2,SEEK_CUR);
				visualizaCampoLong(arquivoDeSaida, "Ticket");
				break;
			default:
				printf("::Nao foi possivel recuperar o campo::\n");
				break;
			}
    	printf("\n");
	} else {
		printf("::Nao foi possivel recuperar o registro::\n");
	}
}