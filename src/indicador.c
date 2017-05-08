#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <regexCustom.h>
#include <utils.h>

int destroiBuffers(char **bufferDeString, int *bufferDeInt, int size){
	int i;

	// Liberando a memoria das string utilizadas. o espaço usado 
	for (i = 0; i < 7; i++) {
		free(bufferDeString[i]);
	}

	free(bufferDeString);
	free(bufferDeInt);

	return 0;
} 

int escreverIndicador(FILE *fEntrada, FILE *fSaida){
	char **bufferDeString = (char **) malloc(7 * sizeof(char *));
	int *bufferDeInt = (int *) malloc(7 * sizeof(int));
	int i, tamEntrada, tamRegistro = 0;
	int type = INDICADOR_DE_TAMANHO;

	fwrite(&type, sizeof(int), 1, fSaida);

	// Descobrindo tamanho do arquivo de entrada
	fseek(fEntrada, 0, SEEK_END);
	tamEntrada = ftell(fEntrada);
	rewind(fEntrada);

	while(ftell(fEntrada) != tamEntrada) {
		// Lendo os dados da memoria, armazenando-os em um buffer. Parao o indicador de tamanho
		// do registro deve ser inserido no inicio do registro, é necessário ler
		// todos os dados Para que se obtenha o tamanho. 

		for (i = 0; i < 7; i++) {
			lerCampo(&(bufferDeString[i]), &(bufferDeInt[i]), fEntrada);
			tamRegistro += bufferDeInt[i];
			//printf("%s - %d\n", bufferDeString[i], bufferDeInt[i]);
		}

		// Soma oito do tamannho do long int
		tamRegistro += sizeof(long int);
		fwrite(&tamRegistro, sizeof(int), 1, fSaida);

		// Dominio	
		if(salvaCampoVariavelAlt(fEntrada, fSaida, bufferDeString[0], &bufferDeInt[0], "dominio") == -1)
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Documento
		if(!salvaCampoFixoAlt(fEntrada, fSaida, bufferDeString[1], &bufferDeInt[1], 19, "documento"))
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Nome
		if(salvaCampoVariavelAlt(fEntrada, fSaida, bufferDeString[2], &bufferDeInt[2], "nome") == -1)
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Cidade
		if(salvaCampoVariavelAlt(fEntrada, fSaida, bufferDeString[3], &bufferDeInt[3], "cidade") == -1)
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// UF
		if(salvaCampoVariavelAlt(fEntrada, fSaida, bufferDeString[4], &bufferDeInt[4], "UF") == -1)
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Data-hora cadastro
		if(!salvaCampoFixoAlt(fEntrada, fSaida, bufferDeString[5], &bufferDeInt[5], 19, "dataHoraCadastro"))
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Data-hora atualiza
		if(!salvaCampoFixoAlt(fEntrada, fSaida, bufferDeString[6], &bufferDeInt[6], 19, "dataHoraAtualiza"))
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Ticket
		if(!salvaCampoLong(fEntrada, fSaida, "ticket"))
			return destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Liberando a memoria das string utilizadas. o espaço usado 
		for (i = 0; i < 7; i++) {
			free(bufferDeString[i]);
		}

		// Pegando o ENTER Para ir Para a proxima linha
		fgetc(fEntrada);
	}

	free(bufferDeString);
	free(bufferDeInt);

	return 1;
}

void registroIndicadores(FILE *fSaida){
	// Pegando o tamanho do registro
	lerCharToInt(fSaida);

	// Mostra na tela o registro atual do arquivo
	vizualizaCampoVariavel(fSaida, "Dominio");		
	vizualizaCampoFixo(fSaida, "Documento", 19);
	vizualizaCampoVariavel(fSaida, "Nome");
	vizualizaCampoVariavel(fSaida, "Cidade");
	vizualizaCampoVariavel(fSaida, "UF");
	vizualizaCampoFixo(fSaida, "dataHoraCadastro", 19);
	vizualizaCampoFixo(fSaida, "dataHoraAtualiza", 19);
	vizualizaCampoLong(fSaida, "Ticket");
}


bool buscaDominioIndicadores(char *dominioBuscado, FILE *fp){
// Busca registros com dominio igual ao fornecido pelo usuario
	char *dominioAtual;
	bool achou = FALSE;
	int tamRegistro;

	if(freopen("output.txt", "rb", fp) != NULL) {
	    fseek(fp, 0, SEEK_SET);

	    while(!feof(fp)){
	    	tamRegistro = lerCharToInt(fp);
		    dominioAtual = campoVariavel(fp);

		    if(!strcmp(dominioAtual, dominioBuscado)){
		    	achou = TRUE;
		    	fseek(fp, - strlen(dominioBuscado) - sizeof(int), SEEK_CUR);
		    	registroIndicadores(fp);
		    	printf("\n");
		    } else {
		    	fseek(fp, (- strlen(dominioBuscado) - sizeof(int) + tamRegistro), SEEK_CUR);
		    }

		    free(dominioAtual);
		}
	}

	return achou;
}

void visualizarIndicadores(FILE *fSaida){
// Mostra em tela todos os registros armazenados
	int contador = 0;

	if(freopen("output.txt", "rb", fSaida) != NULL) {
	    while(!feof(fSaida)){
	        if(contador % 10 == 0 && contador != 0) {
	            printf("\n:: Aperte ENTER para continuar o browsing ::");
	            getchar();
	        }

	        registroIndicadores(fSaida);	
			printf("\nRegistro %d\n", contador);
			contador++;
		}
	}
}

void registroRRNIndicadores(int rrn, FILE *fp){
	int contador = 0, tamRegistro;

	if(freopen("output.txt", "rb", fp) != NULL) {
	    fseek(fp, 0, SEEK_SET);
	    
	    while(!feof(fp) && contador < rrn){
		    tamRegistro = lerCharToInt(fp);
		    fseek(fp, tamRegistro, SEEK_CUR);
		    contador++;
		} 

		if(contador == rrn){
	    	registroIndicadores(fp);
	    	printf("\n");
	    } else {
	    	printf("::Nao foi possivel recuperar o registro::\n");
	    }
	}
}