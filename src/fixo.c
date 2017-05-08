#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <regexCustom.h>
#include <utils.h>



int escreverFixo(FILE *fEntrada, FILE *fSaida){
    // Escreve um arquivo de saida com os dados do arquivo de entrada organizado
    //em numero fixo de campos, e com indicadores de tamanho dos campos
	long tamEntrada;
	//variavel que salva tamanho de campo fixo
	int tamCampo;
	// Descobrindo tamanho do arquivo de entrada
	fseek(fEntrada, 0, SEEK_END);
	tamEntrada = ftell(fEntrada);
	rewind(fEntrada);

	//Este laco lê um registro e salva o tamanho do campo e seu conteudo
	//fazendo esse algoritimo campo a campo
	while(ftell(fEntrada) != tamEntrada){
		// Dominio
		if(salvaCampoVariavel(fEntrada, fSaida, "dominio") == -1)
			return 0;

		// Documento
		tamCampo = 19;
		if(!salvaInt(&tamCampo, fSaida))
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
		tamCampo = 19;
		if(!salvaInt(&tamCampo, fSaida))
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraCadastro"))
			return 0;

		// Data de Atualizacao
		tamCampo = 19;
		if(!salvaInt(&tamCampo, fSaida))
		if(!salvaCampoFixo(fEntrada, fSaida, 19, "dataHoraAtualiza"))
			return 0;

		// Ticket
		tamCampo = sizeof(long);
		if(salvaInt(&tamCampo, fSaida))
			return 0;
		
		if(!salvaCampoLong(fEntrada, fSaida, "ticket"))
			return 0;

		// Pegando o ENTER para ir para a proxima linha
		fgetc(fEntrada);
	}
	return 1;
	
	
}

void registroFixo(FILE *fSaida){
// Mostra na tela o registro atual do arquivo
	vizualizaCampoVariavel(fSaida, "Dominio");		
	vizualizaCampoVariavel(fSaida, "Documento");
	vizualizaCampoVariavel(fSaida, "Nome");
	vizualizaCampoVariavel(fSaida, "Cidade");
	vizualizaCampoVariavel(fSaida, "UF");
	vizualizaCampoVariavel(fSaida, "dataHoraCadastro");
	vizualizaCampoVariavel(fSaida, "dataHoraAtualiza");
	vizualizaCampoVariavel(fSaida, "Ticket");
}

void visualizarFixo(FILE *fSaida){
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