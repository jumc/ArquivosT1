#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <utils.h>
#include <regexCustom.h>

void destroiBuffers(char **bufferDeString, int *bufferDeInt, int size){
	int i;

	// Liberando a memoria das string utilizadas. o espaço usado 
	for (i = 0; i < 7; i++) {
		free(bufferDeString[i]);
	}

	free(bufferDeString);
	free(bufferDeInt);

	exit(0);
}

void escreverIndicador(){
	FILE *arquivoDeEntrada = fopen_("turmaA-dadosDominios.csv","rb");
	FILE *arquivoDeSaida = fopen_("output.txt","wb");

	char **bufferDeString = (char **) malloc(7 * sizeof(char *));
	int *bufferDeInt = (int *) malloc(7 * sizeof(int));
	int i, tamEntrada, tamRegistro = 0;
	int type = INDICADOR_DE_TAMANHO;

	fwrite(&type, sizeof(int), 1, arquivoDeSaida);

	// Descobrindo tamanho do arquivo de entrada
	fseek(arquivoDeEntrada, 0, SEEK_END);
	tamEntrada = ftell(arquivoDeEntrada);
	rewind(arquivoDeEntrada);

	while(ftell(arquivoDeEntrada) != tamEntrada) {
		tamRegistro = 0;
		// Lendo os dados da memoria, armazenando-os em um buffer. Parao o indicador de tamanho
		// do registro deve ser inserido no inicio do registro, é necessário ler
		// todos os dados Para que se obtenha o tamanho. 

		for (i = 0; i < 7; i++) {
			lerCampo(&(bufferDeString[i]), &(bufferDeInt[i]), arquivoDeEntrada);

			if(!(match(bufferDeString[i], "^\\s*[Nn][Uu][Ll][Ll]\\s*$"))){ // Se o valor do campo for nulo...
				tamRegistro += bufferDeInt[i];

		    } else {
		    	if(i == 1 || i == 5 || i == 6) {
		    		tamRegistro += 19;
		    	} else {
		    		bufferDeInt[i] = 0;
		    	}
		    }

			//printf("%s - %d\n", bufferDeString[i], bufferDeInt[i]);
		}

		// Soma oito do tamannho do long int
		tamRegistro += sizeof(long int) + 4 * sizeof(int);

		//printf("\n\ntamReg %d\n\n", tamRegistro);

		fwrite(&tamRegistro, sizeof(int), 1, arquivoDeSaida);

		// Dominio	
		if(!salvaCampoVariavelAlt(arquivoDeEntrada, arquivoDeSaida, bufferDeString[0], &bufferDeInt[0], "dominio"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Documento
		if(!salvaCampoFixoAlt(arquivoDeEntrada, arquivoDeSaida, &bufferDeString[1], &bufferDeInt[1], 19, "documento"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Nome
		if(!salvaCampoVariavelAlt(arquivoDeEntrada, arquivoDeSaida, bufferDeString[2], &bufferDeInt[2], "nome"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// UF
		if(!salvaCampoVariavelAlt(arquivoDeEntrada, arquivoDeSaida, bufferDeString[3], &bufferDeInt[3], "UF"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Cidade
		if(!salvaCampoVariavelAlt(arquivoDeEntrada, arquivoDeSaida, bufferDeString[4], &bufferDeInt[4], "cidade"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Data-hora cadastro
		if(!salvaCampoFixoAlt(arquivoDeEntrada, arquivoDeSaida, &bufferDeString[5], &bufferDeInt[5], 19, "dataHoraCadastro"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Data-hora atualiza
		if(!salvaCampoFixoAlt(arquivoDeEntrada, arquivoDeSaida, &bufferDeString[6], &bufferDeInt[6], 19, "dataHoraAtualiza"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Ticket
		if(!salvaCampoLong(arquivoDeEntrada, arquivoDeSaida, "ticket"))
			destroiBuffers(bufferDeString, bufferDeInt, 7);

		// Liberando a memoria das string utilizadas. o espaço usado 
		for (i = 0; i < 7; i++) {
			free(bufferDeString[i]);
		}

		// Pegando o ENTER Para ir Para a proxima linha
		fgetc(arquivoDeEntrada);
	}

	free(bufferDeString);
	free(bufferDeInt);

	fclose(arquivoDeEntrada);
	fclose(arquivoDeSaida);
}

void registroIndicador(FILE *arquivoDeSaida){
	// Pegando o tamanho do registro
	lerInt(arquivoDeSaida);

	// Mostra na tela o registro atual do arquivo
	if(!feof(arquivoDeSaida)) 
		visualizaCampoVariavel(arquivoDeSaida, "Dominio");		
	if(!feof(arquivoDeSaida)) 
		visualizaCampoFixo(arquivoDeSaida, "Documento", 19);
	if(!feof(arquivoDeSaida)) 
		visualizaCampoVariavel(arquivoDeSaida, "Nome");
	if(!feof(arquivoDeSaida)) 
		visualizaCampoVariavel(arquivoDeSaida, "UF");
	if(!feof(arquivoDeSaida)) 
		visualizaCampoVariavel(arquivoDeSaida, "Cidade");
	if(!feof(arquivoDeSaida)) 
		visualizaCampoFixo(arquivoDeSaida, "dataHoraCadastro", 19);
	if(!feof(arquivoDeSaida)) 
		visualizaCampoFixo(arquivoDeSaida, "dataHoraAtualiza", 19);
	if(!feof(arquivoDeSaida)) 
		visualizaCampoLong(arquivoDeSaida, "Ticket");
}

bool buscaDominioIndicador(FILE *arquivo, char *dominioBuscado) {
	// Busca registros com dominio igual ao fornecido pelo usuario
	char *dominioAtual;
	bool achou = FALSE;
	int tamRegistro, nbytes = 0;

	while(!feof(arquivo)) {
	    nbytes = fread(&tamRegistro, sizeof(int), 1, arquivo);

		if(nbytes > 0) {
			dominioAtual = campoVariavel(arquivo);

			if(feof(arquivo)) {
				break;
			}

			if(!strcmp(dominioAtual, dominioBuscado)) {
			   	achou = TRUE;
			    fseek(arquivo, -strlen(dominioAtual) - 2 * sizeof(int), SEEK_CUR);
			    registroIndicador(arquivo);
			    printf("\n");
			} else {
			    fseek(arquivo, tamRegistro - strlen(dominioAtual) - sizeof(int), SEEK_CUR);
			}

			free(dominioAtual);
		}
	}

	return achou;
}

void visualizarIndicador(FILE *arquivoDeSaida){
// Mostra em tela todos os registros armazenados
	int contador = 0;

	long int tamEntrada;
	// Descobrindo tamanho do arquivo de entrada
	fseek(arquivoDeSaida, 0, SEEK_END);
	tamEntrada = ftell(arquivoDeSaida);
	fseek(arquivoDeSaida, 4, SEEK_SET);
	
    while(ftell(arquivoDeSaida) != tamEntrada){
	    if(contador % 10 == 0 && contador != 0) {
	        printf("\n:: Aperte ENTER para continuar o browsing ::");
	        getchar();
	    }

	    printf("\nRegistro %d\n", contador);
	    registroIndicador(arquivoDeSaida);
		contador++;
	}
}

void registroRRNIndicador(FILE *arquivo, int rrn){
	int contador = 0, tamRegistro, nbytes = 0;

	fseek(arquivo, 0, SEEK_SET);
	nbytes = fread(&nbytes, sizeof(int), 1, arquivo);

	while(!feof(arquivo) && contador < rrn) {
		nbytes += fread(&tamRegistro, sizeof(int), 1, arquivo);
		fseek(arquivo, tamRegistro, SEEK_CUR);
		contador++; 
	}

	if(contador == rrn) {
	   	registroIndicador(arquivo);
	   	printf("\n");
	} else {
	   	printf("::Nao foi possivel recuperar o registro::\n");
	}
}

void campoRegistroIndicador(FILE *arquivoDeSaida, int rrn, int campo) {
	int contador = 0, tamRegistro, nbytes = 0, i;

	fseek(arquivoDeSaida, 0, SEEK_SET);
	nbytes += fread(&nbytes, sizeof(int), 1, arquivoDeSaida);

	while(!feof(arquivoDeSaida) && contador < rrn) {
		nbytes += fread(&tamRegistro, sizeof(int), 1, arquivoDeSaida);
		fseek(arquivoDeSaida, tamRegistro, SEEK_CUR);
		contador++; 
	}

	if(contador == rrn) {
		nbytes += fread(&nbytes, sizeof(int), 1, arquivoDeSaida);
		
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