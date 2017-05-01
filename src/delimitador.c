#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <regexCustom.h>
#include <utils.h>

int escreverDelimitador(FILE *fEntrada, FILE *fSaida){
// Escreve um arquivo de saida com os dados do arquivo de entrada organizados em registros separados por delimitador (#) com campos de tamanho fixo e variavel
	long tamEntrada;
	char delimitador = '#';
	
	// Descobrindo tamanho do arquivo de entrada
	fseek(fEntrada, 0, SEEK_END);
	tamEntrada = ftell(fEntrada);
	rewind(fEntrada);

	freopen("output.txt", "wb+", fSaida);
	while(ftell(fEntrada) != tamEntrada){
		// Dominio	
		if(salvaCampoVariavel(fEntrada, fSaida, "dominio") == -1)
			return 0;

		// Documento
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "documento"))
			return 0;	

		// Nome
		if(salvaCampoVariavel(fEntrada, fSaida, "nome") == -1)
			return 0;

		// Cidade
		if(salvaCampoVariavel(fEntrada, fSaida, "cidade") == -1)
			return 0;

		// UF
		if(salvaCampoVariavel(fEntrada, fSaida, "UF") == -1)
			return 0;

		// Data-hora cadastro
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraCadastro"))
			return 0;

		// Data-hora atualiza
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraAtualiza"))
			return 0;

		// Ticket
		if(!salvaCampoLong(fEntrada, fSaida, "ticket"))
			return 0;

		// Escrevendo delimitador de fim de registro
		fwrite(&delimitador, sizeof(char), 1, fSaida);

		// Pegando o ENTER para ir para a proxima linha
		fgetc(fEntrada);
	}
	return 1;
}

void registroDelimitadores(FILE *fSaida){
// Mostra na tela o registro atual do arquivo
	vizualizaCampoVariavel(fSaida, "Dominio");		
	vizualizaCampoFixo(fSaida, "Documento", 19);
	vizualizaCampoVariavel(fSaida, "Nome");
	vizualizaCampoVariavel(fSaida, "Cidade");
	vizualizaCampoVariavel(fSaida, "UF");
	vizualizaCampoFixo(fSaida, "dataHoraCadastro", 19);
	vizualizaCampoFixo(fSaida, "dataHoraAtualiza", 19);
	vizualizaCampoLong(fSaida, "Ticket");
	// Pegando o # para ir para o proximo registro
	fgetc(fSaida);	
}

int buscaDominioDelimitadores(char *dominioBuscado, FILE *fp){
// Busca registros com dominio igual ao fornecido pelo usuario
	char *dominioAtual, c;
	int achou = 0;

	freopen("output.txt", "rb", fp);
    fseek(fp,0,SEEK_SET);
    
    while(!feof(fp)){
	    dominioAtual = campoVariavel(fp);
	    if(!strcmp(dominioAtual, dominioBuscado)){
	    	achou = 1;
	    	fseek(fp, - strlen(dominioBuscado) - sizeof(int), SEEK_CUR);
	    	registroDelimitadores(fp);
	    	printf("\n");
	    }
	    do{
	    	c = fgetc(fp);
	    } while(c != EOF && c != '#');
	} 

	return achou;
}

void visualizarDelimitadores(FILE *fSaida){
// Mostra em tela todos os registros armazenados

	// O que esta acontecendo com o registro 167?
	int counter = 0;

	freopen("output.txt", "rb", fSaida);
    fseek(fSaida,0,SEEK_SET);
    while(!feof(fSaida)){
        if(counter % 10 == 0 && counter != 0){
            printf("\n:: Aperte ENTER para continuar o browsing ::");
            getchar();
        }
        registroDelimitadores(fSaida);	
		printf("\nRegistro %d\n", counter);
		counter++;
		printf("%c", fgetc(fSaida));
	}
}