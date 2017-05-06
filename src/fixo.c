#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <regexCustom.h>
#include <utils.h>


int salvarIndicadorDeTamanho(int tamanho, FILE *fSaida){
	fwrite(&tamanho, sizeof(int), 1, fSaida);
}

int escreverFixo(FILE *fEntrada, FILE *fSaida){
    // Escreve um arquivo de saida com os dados do arquivo de entrada organizado
    //em numero fixo de campos, e com indicadores de tamanho dos campos
	long tamEntrada;
	
	// Descobrindo tamanho do arquivo de entrada
	fseek(fEntrada, 0, SEEK_END);
	tamEntrada = ftell(fEntrada);
	rewind(fEntrada);

	freopen("output.txt", "wb+", fSaida);
	//Este laco lê um registro e salva o tamanho do campo e seu conteudo
	//fazendo esse algoritimo campo a campo
	while(ftell(fEntrada) != tamEntrada){
		// Dominio	
		if(salvaCampoVariavel(fEntrada, fSaida, "dominio") == -1)
			return 0;

		// Documento
		if(salvaInt(19, fSaida))
			return 0;
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

		// Data Cadastro
		if(salvaInt(19, fSaida))
			return 0;
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraCadastro"))
			return 0;

		// Data de Atualizacao
		if(salvaInt(19, fSaida))
			return 0;
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraAtualiza"))
			return 0;

		// Ticket
		if(salvaInt(sizeof(long), fSaida))
			return 0;
		
		if(!salvaCampoLong(fEntrada, fSaida, "ticket"))
			return 0;

		// Pegando o ENTER para ir para a proxima linha
		fgetc(fEntrada);
	}
	return 1;
}