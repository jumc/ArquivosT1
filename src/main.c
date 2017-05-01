/* 
SCC0215 . Organização de Arquivos . Turma A
    Matheus Aparecido do Carmo Alves - nº 9791114
    Juliana de Mello Crivelli - nº 8909303
    Joao Pedro Ramos Belmiro - nº
    Gabriel Cyrillo dos Santos Cerqueira - nº

Dados dos domínios governamentais registrados no Registro.br (coletado em http://dados.gov.br/dataset/dominios-gov-br), de forma que cada registro indique um domínio governamental. 
    • Campos de tamanho fixo: 
        ticket (número do ticket vinculado ao registro da URL no cadastro do Registro.br), 
        documento (número do documento informado ao Registro.br no cadastro da URL), 
        dataHoraCadastro (data e hora de quando a URL foi cadastrada), 
        dataHoraAtualiza (data e hora de quando a URL foi modificada). 
    • Campos de tamanho variável (deve-se usar o método indicador de tamanho):
        dominio (URL cadastrada no Registro.br), 
        nome (nome do Órgão ou Entidade responsável pela URL), 
        cidade (cidade cadastrada no Registro.br), 
        uf (nome do estado por extenso).

Gerar arquivo
    (1) Indicador de tamanho
    (2) Delimitadores entre registros
    (3) Número fixo de campos
Recuperar arquivo
    (4) Visualizar todos os registros
    (5) Busca por domínio
    (6) Registro completo por RRN
    (7) Campo de um registro por RRN

[1] O arquivo de dados deve ser gravado em disco no modo binário. O modo texto não deve ser usado. 
[2] Os dados do registro descrevem os nomes dos campos, os quais não podem ser alterados. Ademais, todos os campos devem estar presentes na implementação, e nenhum campo adicional pode ser incluído. O tamanho de cada campo deve ser determinado pelo grupo. Essa escolha deve ser explicada na documentação externa. 
[3] Devem ser exibidos avisos ou mensagens de erro sempre que apropriado. 
[4] Os dados devem ser escritos e lidos campo a campo. Pode-se usar também a serialização (memcpy).
*/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <delimitador.h>
#include <fixo.h>
#include <indicador.h>

int main (int argc, char **argv){
    // Variaveis utilizadas no programa...
    int op = -1;

    FILE *fEntrada = fopen("turmaA-dadosDominios.csv", "rb");
    if(fEntrada == NULL){
        printf("Nao foi possivel abrir o arquivo de entrada\n");
        return 0;
    } 
    FILE *fSaida = fopen("output.txt","ab+");
    if(fSaida == NULL){
        printf("Nao foi possivel abrir/criar o arquivo de saida\n");
        return 0;
    }   

    while(op){
        // 1. Menu
        printf("------------------------------------ MENU\n\n");
        printf("Gerar arquivo\n");
        printf("\t(1) Indicador de tamanho\n");
        printf("\t(2) Delimitadores entre registros\n");
        printf("\t(3) Número fixo de campos");
        printf("\nRecuperar arquivo\n");
        printf("\t(4) Visualizar todos os registros\n");
        printf("\t(5) Busca por domínio\n");
        printf("\t(6) Registro completo por RRN\n");
        printf("\t(7) Campo de um registro por RRN\n");
        printf("(0) Sair\n");
        printf("\nDigite o codigo da operacao: ");
        op = lerCharToInt(stdin);
        
        //2. Realizando as operacoes... 
        switch(op){
            case 1:
                if(escreverIndicador(fEntrada, fSaida)){
                    printf("Escrita realizada com sucesso\n\n");
                } else{
                    printf("::Erro na escrita::\n\n");
                }
            break;
            case 2:
                if(escreverDelimitador(fEntrada, fSaida)){
                    printf("Escrita realizada com sucesso\n\n");
                } else{
                    printf("::Erro na escrita::\n\n");
                }
            break;
            case 3:
                if(escreverFixo(fEntrada, fSaida)){
                    printf("Escrita realizada com sucesso\n\n");
                } else{
                    printf("::Erro na escrita::\n\n");
                }
            break;
            case 4:
                visualizar(fSaida);
            break;
            case 5:
                if(!buscaDominio(fSaida)){
                    printf("::Dominio nao encontrado::\n\n");
                }
            break;
            case 6:
                registroRRN(fSaida);
            break;
            /*case 7:
                campoResgistro(fSaida);
            break;*/  
            case 0:
                printf("Obrigado por usar nosso programa! Ate a proxima!\n");
            break;
            default:
                printf(":: Comando Invalido ::\n\n");
            break;
        }
    }
    
    //3. Encerrando o programa...
    fclose(fEntrada);
    fclose(fSaida);
    return 0;
}