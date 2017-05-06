/* 
|    SCC0215 . Organização de Arquivos . Turma A
|    Matheus Aparecido do Carmo Alves - nº 9791114
|    Juliana de Mello Crivelli - nº 8909303
|    Joao Pedro Ramos Belmiro - nº
|    Gabriel Cyrillo dos Santos Cerqueira - nº
|
|Dados dos domínios governamentais registrados no Registro.br (coletado em http://dados.gov.br/dataset/dominios-gov-br), de forma que cada registro indique um domínio governamental. 
|    • Campos de tamanho fixo: 
|        ticket (número do ticket vinculado ao registro da URL no cadastro do Registro.br), 
|        documento (número do documento informado ao Registro.br no cadastro da URL), 
|        dataHoraCadastro (data e hora de quando a URL foi cadastrada), 
|        dataHoraAtualiza (data e hora de quando a URL foi modificada). 
|    • Campos de tamanho variável (deve-se usar o método indicador de tamanho):
|        dominio (URL cadastrada no Registro.br), 
|        nome (nome do Órgão ou Entidade responsável pela URL), 
|        cidade (cidade cadastrada no Registro.br), 
|        uf (nome do estado por extenso).
|
|Gerar arquivo
|    (1) Indicador de tamanho
|    (2) Delimitadores entre registros
|    (3) Número fixo de campos
|Recuperar arquivo
|    (4) Visualizar todos os registros
|    (5) Busca por domínio
|    (6) Registro completo por RRN
|    (7) Campo de um registro por RRN
|
|[1] O arquivo de dados deve ser gravado em disco no modo binário. O modo texto não deve ser usado. 
|[2] Os dados do registro descrevem os nomes dos campos, os quais não podem ser alterados. Ademais, todos os campos devem estar presentes na implementação, e nenhum campo adicional pode ser incluído. O tamanho de cada campo deve ser determinado pelo grupo. Essa escolha deve ser explicada na documentação externa. 
|[3] Devem ser exibidos avisos ou mensagens de erro sempre que apropriado. 
|[4] Os dados devem ser escritos e lidos campo a campo. Pode-se usar também a serialização (memcpy).
*/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <delimitador.h>
#include <fixo.h>
#include <indicador.h>

int main (int argc, char **argv){
    //0. Variaveis e arquivos utilizados no programa...
    int operacao = INVALID;

    //1. Apresentando o programa ao usuario e inicializando o loop de operacoes...
    imprimirSaudacoes();
    
    while(operacao){
        //2. Lendo a operacao que o usuario deseja realizar
        operacao = lerOperacao(stdin);
        
        //3. Realizando a operacao lida a cima... 
        switch(operacao){     
            case INDICADOR_DE_TAMANHO:
                if(escreverIndicador( arquivoDeEntrada,  arquivoDeSaida))
                    printf("Escrita realizada com sucesso\n\n");
                else
                    printf(":: Erro na escrita ::\n\n");
            break;
                
            case DELIMITADOR_ENTRE_REG:
                if(escreverDelimitador( arquivoDeEntrada,  arquivoDeSaida))
                    printf("Escrita realizada com sucesso\n\n");
                else
                    printf(":: Erro na escrita ::\n\n");
            break;
                
            case NUMERO_DE_CAMPS_FIXO:
                if(escreverFixo( arquivoDeEntrada,  arquivoDeSaida))
                    printf("Escrita realizada com sucesso\n\n");
                else
                    printf(":: Erro na escrita ::\n\n");
            break;
                    
            case VIZUALIZAR_TODOS:
                visualizar(arquivoDeSaida);
            break;
                
            case VIZUALIZAR_DOMINIO:
                if(!buscaDominio(arquivoDeSaida))
                    printf(":: Dominio nao encontrado ::\n\n");
            break;
                
            case VIZUALIZAR_REG_RRN:
                registroRRN(arquivoDeSaida);
            break;
                
            case VIZUALIZAR_CAMPO_RRN:
                //campoResgistro(fSaida);
            break;
                
            case SAIR:
                printf("| Obrigado por usar nosso programa! Ate a proxima!\n --- \n");
                printf("|    SCC0215 . Organização de Arquivos . Turma A\n");
                printf("| Gabriel Cyrillo dos Santos Cerqueira - nº\n");
                printf("| Joao Pedro Ramos Belmiro             - nº\n");
                printf("| Juliana de Mello Crivelli            - nº 8909303\n");
                printf("| Matheus Aparecido do Carmo Alves     - nº 9791114\n --- \n");
            break;
                
            default:
                printf("| Qual outra operacao deseja realizar?\n| > ");
            break;
        }
    }
    
    //4. Encerrando o programa...
    return 0;
}
