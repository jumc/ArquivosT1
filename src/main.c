/* 
|    SCC0215 . Organização de Arquivos . Turma A
|    Matheus Aparecido do Carmo Alves       - nº 9791114
|    Juliana de Mello Crivelli              - nº 8909303
|    Joao Pedro Ramos Belmiro               - nº 9791198
|    Gabriel Cyrillo dos Santos Cerqueira   - nº 9763022
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
        printf("| Digite a operacao que deseja realizar: ");
        operacao = lerOperacao(stdin);
        
        //3. Realizando a operacao lida a cima... 
        switch(operacao){     
            case INDICADOR_DE_TAMANHO:
                escreverIndicador();
                printf("Escrita realizada com sucesso\n\n");
            break;
                
            case DELIMITADOR_ENTRE_REG:
                escreverDelimitador();
                printf("Escrita realizada com sucesso\n\n");
            break;
                
            case NUMERO_DE_CAMPS_FIXO:
                escreverFixo();
                printf("Escrita realizada com sucesso\n\n");
            break;
                    
            case VISUALIZAR_TODOS:
                visualizar();
            break;
                
            case VISUALIZAR_DOMINIO:
                buscaDominio();
            break;
                
            case VISUALIZAR_REG_RRN:
                registroRRN();
            break;
                
            case VISUALIZAR_CAMPO_RRN:
                campoRegistro();
            break;
            
            case MENU:
                printf("Operacoes para Gerar arquivo\n");
                printf("\t(1) Indicador de tamanho\n");
                printf("\t(2) Delimitadores entre registros\n");
                printf("\t(3) Número fixo de campos\n\n");
                printf("Operacoes para Recuperar arquivo\n");
                printf("\t(4) Visualizar todos os registros\n");
                printf("\t(5) Busca por domínio\n");        
                printf("\t(6) Registro completo por RRN\n");
                printf("\t(7) Campo de um registro por RRN\n\n");
                printf("Operacoes Gerais do Sistema\n");
                printf("\t(9) Exibir menu de operacoes\n");
                printf("\t(0) Sair\n\n");            
            break;
                
            case SAIR:
                printf("| Obrigado por usar nosso programa! Ate a proxima!\n --- \n");
                printf("| SCC0215 . Organização de Arquivos . Turma A\n");
                printf("| Gabriel Cyrillo dos Santos Cerqueira - nº 9763022\n");
                printf("| Joao Pedro Ramos Belmiro             - nº 9791198\n");
                printf("| Juliana de Mello Crivelli            - nº 8909303\n");
                printf("| Matheus Aparecido do Carmo Alves     - nº 9791114\n --- \n");
            break;
                
            default:
                printf("| Qual outra operacao deseja realizar?\n");
            break;
        }
    }
    
    //4. Encerrando o programa...
    return 0;
}
