# ArquivosT1
Trabalho 1 da disciplina Organização de Arquivos do ICMC-USP, 2017 - Turma A
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
