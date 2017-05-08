#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

/* 
* -- int escreverDelimitador()
* Escreve um arquivo de saida com os dados do arquivo de entrada organizados em
* registros separados por delimitador (#) com campos de tamanho fixo e variavel
*/
int escreverDelimitador(){
	//0. Variaveis da funcao...
	char delimitador = '#';
	int type = DELIMITADOR_ENTRE_REG;

	fwrite(&type, sizeof(int), 1, fSaida);
	// Descobrindo tamanho do arquivo de entrada
	fseek(fEntrada, 0, SEEK_END);
	tamEntrada = ftell(fEntrada);
	rewind(fEntrada);
	
	while(ftell(fEntrada) != tamEntrada){
		// Dominio	
		if(salvaCampoVariavel(fEntrada, fSaida, "dominio") == -1)
	long tamanhoDaEntrada = 0;
	FILE *arquivoDeEntrada = NULL, *arquivoDeSaida = NULL;
	
	//1. Descobrindo tamanho do arquivo de entrada...
	arquivoDeEntrada = fopen_("turmaA-dadosDominios.csv","rb");
	fseek(arquivoDeEntrada, 0, SEEK_END);
	tamanhoDaEntrada = ftell(arquivoDeEntrada);
	rewind(arquivoDeEntrada);

	//2. Escrevendo, com base no arquivo de entrada, um arquivode saida com de_
	//limitadores de tamanho...
	arquivoDeSaida = fopen_("output.txt", "wb+");
	while(ftell(arquivoDeEntrada) != tamanhoDaEntrada){
		//a. escrevendo o Dominio	
		if(salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "dominio") == -1)
			return 0;

		//b. escrevendo o numero Documento
		if(!salvaCampoFixo(arquivoDeEntrada, arquivoDeSaida, 19, "documento"))
			return 0;	

		//c. escrevendo o Nome
		if(salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "nome") == -1)
			return 0;

		//d. escrevendo o nome da Cidade
		if(salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "cidade") == -1)
			return 0;

		//e. escrevendo o UF
		if(salvaCampoVariavel(arquivoDeEntrada, arquivoDeSaida, "UF") == -1)
			return 0;

		//f. escrevendo a Data-hora do cadastro
		if(!salvaCampoFixo(arquivoDeEntrada, arquivoDeSaida, 19, "dataHoraCadastro"))
			return 0;

		//g. escrevendo a Data-hora da atualizacao
		if(!salvaCampoFixo(arquivoDeEntrada, arquivoDeSaida, 19, "dataHoraAtualiza"))
			return 0;

		//h. escrevendo o Ticket
		if(!salvaCampoLong(arquivoDeEntrada, arquivoDeSaida, "ticket"))
			return 0;

		//i. Escrevendo delimitador de fim de registro
		fwrite(&delimitador, sizeof(char), 1, arquivoDeSaida);

		//j. lendo o ENTER que separa os campos
		fgetc(arquivoDeEntrada);
	}
	
	//3. Fechando os arquivos abertos e encerrando a funcao...
	fclose(arquivoDeEntrada);
	fclose(arquivoDeSaida);
	return 1;
}

// Imprime na tela o registro para o qual o arquivo passado aponta...
void imprimirRegistroDelimitadores(FILE *arquivo){
	vizualizaCampoVariavel(arquivo, "Dominio");		
	vizualizaCampoFixo(arquivo, "Documento", 19);
	vizualizaCampoVariavel(arquivo, "Nome");
	vizualizaCampoVariavel(arquivo, "Cidade");
	vizualizaCampoVariavel(arquivo, "UF");
	vizualizaCampoFixo(arquivo, "dataHoraCadastro", 19);
	vizualizaCampoFixo(arquivo, "dataHoraAtualiza", 19);
	vizualizaCampoLong(arquivo, "Ticket");
	fgetc(arquivo); 	// get delimitador (#)	
	printf("\n");
}

// Busca registros com dominio igual ao fornecido pelo usuario
int buscaDominioDelimitadores(char *dominioProcurado){
	char c, *dominioAtual = NULL;

	//1. Abrindo o arquivo no qual se deseja realizar a busca
	FILE *arquivo = fopen_("output.txt", "rb");
    fseek(arquivo,0,SEEK_SET);
    
    //2. Iniciando a busca que para quando encontra o registro procurado ou
    //quando acaba o arquivo de busca...
    while(!feof(arquivo)){
    	//a. Definindo o dominio atual
	    dominioAtual = campoVariavel(arquivo);
	    //b. Caso o dominio atual seja o dominio procurado, encerra-se a busca...
	    if(!strcmp(dominioAtual, dominioProcurado)){
	    	fseek(arquivo, - strlen(dominioProcurado) - sizeof(int), SEEK_CUR);
	    	imprimirRegistroDelimitadores(arquivo);
	    	fclose(arquivo);
	    	return 1;
	    }
	    //c. Caso contrario continua-se a percorrer o arquivo...
	    do{
	    	c = fgetc(arquivo);
	    }while(c != EOF && c != '#');
	} 
	
	//3. Fechando o arquivo aberto e encerrando a funcao...
	fclose(arquivo);
	return 0;
}

// Mostra em tela todos os registros armazenados
void visualizarTodosDelimitadores(){
	// O que esta acontecendo com o registro 167?
	int counter = 0;
	FILE *arquivo = NULL;

	//1. 
	arquivo = fopen_("output.txt", "rb");
    fseek(arquivo,0,SEEK_SET);
    
    //2. 
    while(!feof(arquivo)){
    	
        if(counter % 10 == 0 && counter != 0){
            printf("\n:: Aperte ENTER para continuar o browsing ::");
            getchar();
        }
        
        imprimirRegistroDelimitadores(arquivo);	
		printf("\nRegistro %d\n", counter);
		counter++;
		
		printf("%c", fgetc(fSaida));
		printf("%c", fgetc(arquivo));
		printf("%c", fgetc(arquivo));
	}
	
	//3. Fechando os arquivos e encerrando a funcao...
	fclose(arquivo);
	return;
}

void buscaRRNDelimitadores(int rrn){
	//0. Variaveis da funcao
	char c;
	int counter = 0;
	FILE *arquivo = NULL;

	//1. Abrindo o arquivo sobre o qual sera realizada a busca...
	arquivo = fopen_("output.txt", "rb");
    fseek(arquivo,0,SEEK_SET);
    
    //2. Realizando a busca enquanto o contador for menor que o rrn e
    //nao for o final do arquivo...
    while(!feof(arquivo) && counter < rrn){
	    do{
	    	c = fgetc(arquivo);
	    } while(c != EOF && c != '#');
	    counter++;
	}
	
	//3. Se o contador for igual ao rrn, o registro eh impresso na tela...
	//- Caso contrario eh exibido uma mensagem de erro...
	if(counter == rrn){
    	imprimirRegistroDelimitadores(arquivo);
    } else {
    	printf("::Nao foi possivel recuperar o registro::\n");
    }
    
    //4. Fechando o arquivo e retornando a funcao...
    fclose(arquivo);
	return;
}