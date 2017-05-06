#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <regexCustom.h>
#include <utils.h>

#define TAMANHODOC 20
#define TAMANHODATA 20

int escreverFixo(FILE *fEntrada, FILE *fSaida){
    // Escreve um arquivo de saida com os dados do arquivo de entrada organizado
    //em numero fixo de campos, 
	long tamEntrada;
	
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
		tamanho = TAMANHODOC;
		fwrite(&tamanho, sizeof(int), 1, fSaida);
		
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
		tamanho = TAMANHODATA;
		fwrite(&tamanho, sizeof(int), 1, fSaida);
		
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraCadastro"))
			return 0;

		// Data-hora atualiza;
		tamanho = TAMANHODATA;
		fwrite(&tamanho, sizeof(int), 1, fSaida);
		
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraAtualiza"))
			return 0;

		// Ticket
		tamanho = sizeof(long);
		fwrite(&tamanho, sizeof(int), 1, fSaida);
		
		if(!salvaCampoLong(fEntrada, fSaida, "ticket"))
			return 0;

		// Pegando o ENTER para ir para a proxima linha
		fgetc(fEntrada);
	}
	return 1;
}