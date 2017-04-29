#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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