#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo_lista.h"

//Tipo de dados criado para facilitar a representacao do atributo
//de uma dimensao qualquer.
typedef char* atributo;

//Enum utilizada para armazenar as cores a serem utilizadas durante a busca em profundidade
typedef enum cor {
    preto,
    branco,
} cor;

//Struct que armazena os dados das dimensoes. Sigla para o grafo de derivacao,
//nome por extenso, quantidade de atributos cadastrados, atributos cadastrados, indice da dimensao
//que serve como agregacao da dimensao atual, e cor (utilizado para busca em profundidade)
typedef struct dimensao {
    //Sigla que sera utilizada no grafo de derivacao
    char sigla;
    //Nome por extenso do atributo, usado para a impressao no terminal
    char *nome;
    //quantidade de atributos cadastrados para dimensao. comeca com 0.
    int qtd_atributos;
    //vetor de atributos. tem tamanho variavel.
    atributo *atributos;
    //guarda o indice da dimensao que eh agregacao da dimensao representada por
    //uma instancia dessa struct. comeca com -1 para indicar que ainda nao foi cadastrada
    //uma agregacao
    int agregacao;
    //campo cor usado para a busca recursiva identificar por quais lugares a busca
    //ja passou.
    cor cor;
} dimensao;

//Estrutura para armazenar todas as dimensoes cadastradas. Guarda um vetor de dimensoes e
//a quantidade de dimensoes ja cadastradas.
typedef struct dimensoes {
    dimensao *lista_dimensoes;
    int qtd_dimensoes;
} dimensoes;

//Funcao utilitaria que inverte um vetor de inteiros
int *inverte_vetor(int **vetor, int qtd_elems);

//Funcao que imprime na tela as opcoes do menu
void exibe_menu();

//Funcao que imprime na tela todas as dimensoes cadastradas
void exibe_dimensoes(dimensoes dims);

//Funcao que imprime na tela uma unica dimensao (utilizada pela funcao que imprime todas as dimensoes)
void exibe_dimensao(dimensao dim);

//Funcao que pede ao usuario que digite os dados de uma nova dimensao,
//le os dados, aloca uma nova dimensao e a retorna
dimensao le_nova_dimensao();

//Funcao que chama a funcao que le um novo atributo da tela, e então
//coloca o atributo lido na dimensao escolhida para o atributo
dimensao *acrescenta_atributo_numa_dimensao(dimensao *dim);

//Funcao que pede que o usuario digite os dados do novo atributo e os lê.
//Usada pela funcao acima, que coloca o atributo em uma dimensao.
atributo le_atributo();

//Funcao que pede que o usuario escolha uma dimensao. Eh utilizado por uma serie de funcoes,
//como a que cadastra um novo atributo para uma dimensao, ou a que cadastra uma agregacao
int escolhe_dimensao(dimensoes dims);

//Exibe a hierarquia de agregacoes de uma dimensao
void exibe_dimensao_com_agregacoes(dimensoes *dims, int indice_dimensao, int primeiro);

//Exibe todas as hierarquias de agregacoes cadastradas no sistema
void exibe_dimensoes_com_agregacoes(dimensoes dims);

//Pinta todas as dimensoes da cor especificada no parametro c. usado para as buscas
void pinta_todas_as_dimensoes(dimensoes *dims, cor c);

//Grava todas as dimensoes em um arquivo binario chamado dimensoes.dat
void grava_dados_arquivo(dimensoes dims);

//Carrega todas as dimensoes de um arquivo binario chamado dimensoes.dat
dimensoes *carrega_dados_arquivo();

//Recebe o codigo dot (lingugem utilizada pelo graphviz para visualizacao de grafos)
//e grava em um arquivo chamado grafo.dot
void grava_codigo_dot_em_arquivo(char *codigo_dot);

//Gera uma matriz com as hierarquias, estrutura de dados utilizada para tornar mais conveniente
//a geracao do grafo de derivacao
int **gera_hierarquias(dimensoes dims, int *quantidade_hier, int **qtd_dimensoes_por_hier);

//Busca em profundidade utilizada para recuperar todas as hierarquias cadastradas.
//IMPORTANTE: Para funcionar, foi necessario incluir uma restricao no programa.
//As dimensoes ancestrais devem ser cadastradas ANTES das dimensoes descendentes.
int *descobre_hierarquia(dimensoes *dims, int indice_dim, int **hierarquia_dimensao,
                         int *qtd_agregacoes_hierarquia);

//Gera o grafo de derivacao a partir da matriz de hierarquias
grafo *insere_vertices_agregados(dimensoes dims,int **hierarquias, int quantidade_hierarquias,
                                int *quantidade_dimensoes_por_hierarquia);

//Funcao utilitaria que concatena
char *concatena(char *entrada, dimensoes dims, int **hierarquias, int *quantidade_dimensoes_por_hierarquia, int quantidade_hierarquias, int i, int j);

int main() {
    //Guarda a opcao do menu que usuario vai digitar
    int opcao_menu = 0;
    //Guarda a dimensao que o usuario vai escolher nas opcoes pertinentes
    int opcao_dimensao = 0;
    //Guarda o indice da dimensao que o usuario vai escolher como ancestral no cadastro de agregacoes
    int ancestral;
    //Idem ao anterior, mas para a dimensao escolhia como descendente da agregacao
    int descendente;
    //Indices de iteracao
    int i, j;

    //Guarda a quantidade de hierarquias cadastradas. Variavel preenchida na busca recursiva
    //por hierarquias
    int quantidade_hierarquias;
    //Vetor que guarda a quantidade de dimensoes em uma hierarquia
    //Cada posicao do vetor guarda a quantidade de dimensoes da hierarquia correspondente
    //Ex:[3][4][7]. A hierarquia 0 tem 3 dimensoes, a hierarquia 1 tem 4 dimensoes e a hierarquia 2 tem 7
    //dimensoes.
    int *quantidade_dimensoes_por_hierarquia;

    //Guarda as dimensoes em cada hierarquia de agregacoes
    //ex: [0]3 1 2
    //    [1]4 0 5
    //A hierarquia 0 tem as dimensoes 3, 1 e 2. A hierarquia 1 tem as dimensoes 4, 0 e 5.
    int **hierarquias;

    //Estrutura que guarda todas as dimensoes.
    dimensoes dims;
    //Coloca null na lista pra nao dar problema na alocacao do ponteiro depois
    dims.lista_dimensoes = NULL;
    //Comeca com 0 dimensoes cadastradas
    dims.qtd_dimensoes = 0;

    //Ponteiro pro grafo que eh gerado na visualizacao do grafo de derivacao
    grafo *g;

    //Variavel que guarda o codigo dot que eh gerado na visualizacao do grafo de derivacao
    //Essa string eh usada para gravar o codigo dot num arquivo
    char *codigo_dot;

    //Eh oferecida a opcao do usuario
    char gera_visualizacao = 'n';

    //Executa o programa enquanto o usuario nao digitar a opcao '9' (sair)
    while(opcao_menu != 9) {
        //Exibe as opcoes do menu na tela
        exibe_menu();
        scanf("%d", &opcao_menu);

        //switch pra controle do menu
        switch(opcao_menu) {
        case 1://cadastrando nova dimensao
            //
            printf("Ao inserir dimensoes, tenha em mente que a dimensao que sera ancestral\n");
            printf("na hierarquia de agregacoes deve ser cadastrada PRIMEIRO.\n");
            dims.qtd_dimensoes++;
            dims.lista_dimensoes = realloc(dims.lista_dimensoes, sizeof(dimensao)*dims.qtd_dimensoes);
            dims.lista_dimensoes[dims.qtd_dimensoes-1] = le_nova_dimensao();
            exibe_dimensoes(dims);
            break;
        case 2://cadastrando novo atributo para uma dimensao
            opcao_dimensao = escolhe_dimensao(dims);
            dims.lista_dimensoes[opcao_dimensao] =
            *acrescenta_atributo_numa_dimensao(&(dims.lista_dimensoes[opcao_dimensao]));
            exibe_dimensoes(dims);
            break;
        case 3://exibindo dimensoes cadastradas
            exibe_dimensoes(dims);
            break;
        case 4://configura uma dimensao como uma agregacao de outra dimensao
            exibe_dimensoes(dims);
            printf("Digite o NUMERO mostrado ao lado da dimensao para escolher.\n");
            printf("Escolha o ancestral: ");
            scanf("%d", &ancestral);
            printf("\n");

            exibe_dimensoes(dims);
            printf("Digite o NUMERO mostrado ao lado da dimensao para escolher.\n");
            printf("Escolha o descendente: ");
            scanf("%d", &descendente);
            printf("\n");

            dims.lista_dimensoes[ancestral].agregacao = descendente;
            exibe_dimensoes_com_agregacoes(dims);
            break;
        case 5://exibe as dimensoes e suas agregacoes
            exibe_dimensoes_com_agregacoes(dims);
            break;
        case 6://constroi e exibe o grafo de derivacao
            //NAO MEXER
            //A hierarquias é uma matriz auxiliar, usada pra navegar entre as agregacoes cadastradas.
            hierarquias = gera_hierarquias(dims, &quantidade_hierarquias, &quantidade_dimensoes_por_hierarquia);

            if(quantidade_hierarquias > 2){
                printf("ATENÇÃO. Foi necessario impor uma restrição de funcionalidade.s\n");
                printf("A geracao do grafo de derivacao só é realizada por este programa\n");
                printf("para ATE no maximo 2 hierarquias, funcionando para 2 hierarquias de tamanho qualquer cada uma.\n");
                printf("Dimensoes isoladas sao contadas como uma hierarquia. Por favor,\n");
                printf("coloque as dimensoes isoladas em uma hierarquia. Se mesmo assim houver mais\n");
                printf("de uma hierarquia, por favor reinicie o programa.\n");
                break;
            }

            //Gera o gravo de derivacao e coloca no grafo g
            g = insere_vertices_agregados(dims, hierarquias, quantidade_hierarquias, quantidade_dimensoes_por_hierarquia);

            //Gera o codigo do graphviz com base no grafo de derivacao g
            codigo_dot = gera_codigo_dot(g);
            //Grava o codigo do graphviz em um arquivo chamado grafo.dot
            grava_codigo_dot_em_arquivo(codigo_dot);

            //Pergunta se deseja invocar o graphviz para gera a visualizacao do grafo a partir
            //do codigo dot criado
            printf("Foi gerado o arquivo grafo.dot com o codigo para gerar a visualizacao do grafo.\n");
            printf("Se tiver o graphviz instalado, E REGISTRADO NA VARIAVEL DE AMBIENTE PATH,\n");
            printf("eh possivel gerar o arquivo de visualizacao do grafo.\n");
            printf("Gerar visualizacao? s/n: ");
            scanf(" %c", &gera_visualizacao);
            if(gera_visualizacao == 's') {
                //gera visualizacao
                printf("Gerando visualizacao...\n");
                //chamada pro graphviz. SO FUNCIONA SE O GRAPHVIZ ESTIVER NO PATH DO USUARIO!!!!!
                system("dot -Tpng grafo.dot -o grafo.png");
                printf("Visualizacao gerada. Verificar o arquivo grafo.png.\n\n");
            } else {
                //nao gera
                printf("Foi digitado 'n' ou algum caracter invalido, portanto a visualizacao nao sera gerada.\n\n");
            }
            //gera_grafo_derivacao(dims);

            break;
        case 7://grava os dados em um arquivo
            grava_dados_arquivo(dims);
            break;
        case 8://carrega os dados de um arquivo
            dims = *carrega_dados_arquivo();
            break;
        case 9://sai do programa
            exit(0);
            break;
        default://digitou um valor que nao eh uma opcao do menu
            printf("\n Opcao invalida.\n");
            break;
        }
    }
    return 0;
}

//Gera o grafo de derivacao a partir da matriz de hierarquias
grafo *insere_vertices_agregados(dimensoes dims, int **hierarquias, int quantidade_hierarquias, int *quantidade_dimensoes_por_hierarquia){
    //Coloca o primeiro vertice do grafo, que tem a sigla de todas as dimensoes concatenadas
    int i, j, ii, jj;
    grafo *g = cria_grafo();
    char *todas_siglas = malloc(quantidade_hierarquias * sizeof(char));
    todas_siglas[quantidade_hierarquias - 1] = '\0';

    //Daqui ate o fim da funcao: realiza logica combinatoria para gerar todas os vertices para o grafo de derivacao
    //Da maneira como esta implementado, infelizmente apenas funciona para 2 hierarquias que podem ter tamanhos iguais
    //ou diferentes.
    for(i=0; i < quantidade_hierarquias; i++) {
        for(j = 0; j < quantidade_dimensoes_por_hierarquia[i]; j++) {
            char *entrada = malloc(2 * sizeof(char));
            entrada[0] = dims.lista_dimensoes[(hierarquias[i][j])].sigla;
            entrada[1] = '\0';
            insere_vertice(g, entrada);
            if(j > 0){
                insere_aresta_por_numero(g, hierarquias[i][j - 1], hierarquias[i][j]);
            }
        }
    }

    for(i = 0; i < quantidade_dimensoes_por_hierarquia[0]; i++){
        for(j = 0; j < quantidade_dimensoes_por_hierarquia[1]; j++){
            if((j == quantidade_dimensoes_por_hierarquia[1] - 1) && (i == quantidade_dimensoes_por_hierarquia[0] - 1)){
                //esta na ponta
                char *auXx = malloc(3 * sizeof(char));
                auXx[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                auXx[1] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                auXx[2] = '\0';
                insere_vertice(g, auXx);
                char *auxRr = malloc(2 * sizeof(char));
                auxRr[0] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                auxRr[1] = '\0';
                insere_vertice(g, auxRr);
                insere_aresta_por_valor(g, auXx, auxRr);
                char *auxDd = malloc(2 * sizeof(char));
                auxDd[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                auxDd[1] = '\0';
                insere_vertice(g, auxDd);
                insere_aresta_por_valor(g, auXx, auxDd);
            }else if(j == quantidade_dimensoes_por_hierarquia[1] - 1){
                //esta na extrema abaixo
                char *auXx = malloc(3 * sizeof(char));
                auXx[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                auXx[1] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                auXx[2] = '\0';
                insere_vertice(g, auXx);
                char *auxDd = malloc(2 * sizeof(char));
                auxDd[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                auxDd[1] = '\0';
                insere_vertice(g, auxDd);
                insere_aresta_por_valor(g, auXx, auxDd);
                char *auxRr = malloc(3 * sizeof(char));
                auxRr[0] = dims.lista_dimensoes[hierarquias[0][i + 1]].sigla;
                auxRr[1] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                auxRr[2] = '\0';
                insere_vertice(g, auxRr);
                insere_aresta_por_valor(g, auXx, auxRr);
            }else if(i == quantidade_dimensoes_por_hierarquia[0] - 1){
                //esta na extrema direita
                char *auXx = malloc(3 * sizeof(char));
                auXx[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                auXx[1] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                auXx[2] = '\0';
                insere_vertice(g, auXx);
                char *auxRr = malloc(2 * sizeof(char));
                auxRr[0] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                auxRr[1] = '\0';
                insere_vertice(g, auxRr);
                insere_aresta_por_valor(g, auXx, auxRr);
                char *auxDd = malloc(3 * sizeof(char));
                auxDd[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                auxDd[1] = dims.lista_dimensoes[hierarquias[1][j + 1]].sigla;
                auxDd[2] = '\0';
                insere_vertice(g, auxDd);
                insere_aresta_por_valor(g, auXx, auxDd);
            }else if((j < quantidade_dimensoes_por_hierarquia[1]) && (i < quantidade_dimensoes_por_hierarquia[0])){
                //meio
                char *aux = malloc(3 * sizeof(char));
                aux[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                aux[1] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                aux[2] = '\0';
                char *auxR = malloc(3 * sizeof(char));
                auxR[0] = dims.lista_dimensoes[hierarquias[0][i + 1]].sigla;
                auxR[1] = dims.lista_dimensoes[hierarquias[1][j]].sigla;
                auxR[2] = '\0';
                char *auxD = malloc(3 * sizeof(char));
                auxD[0] = dims.lista_dimensoes[hierarquias[0][i]].sigla;
                auxD[1] = dims.lista_dimensoes[hierarquias[1][j + 1]].sigla;
                auxD[2] = '\0';
                insere_vertice(g, aux);
                insere_vertice(g, auxR);
                insere_vertice(g, auxD);
                insere_aresta_por_valor(g, aux, auxR);
                insere_aresta_por_valor(g, aux, auxD);
            }
            //imprime_grafo(g);
        }
    }


    //NAO MEXER
    //Coloca a dimensao "Vazio" no final do grafo de derivacao
    insere_vertice(g, "vazio");
    for(i = 0; i < quantidade_hierarquias; i++){
        char *entrada = malloc(2 * sizeof(char));
        entrada[0] = dims.lista_dimensoes[(hierarquias[i][quantidade_dimensoes_por_hierarquia[i] - 1])].sigla;
        entrada[1] = '\0';
        insere_aresta_por_valor(g, entrada, "vazio");
    }

    return g;
}

//Funcao utilitaria que concatena um caractere numa string qualquer.
char *concatena(char *entrada, dimensoes dims, int **hierarquias, int *quantidade_dimensoes_por_hierarquia, int quantidade_hierarquias, int i, int j){
    //tenta contatenar o vazio (j ultrapassa limite)
    if(j >= quantidade_dimensoes_por_hierarquia[i]) return NULL;
    if(i >= quantidade_hierarquias) return NULL;
    char *saida = malloc((strlen(entrada) + 1) * sizeof(char));
    strcpy(saida, entrada);
    saida[strlen(entrada)] = dims.lista_dimensoes[(hierarquias[i][j])].sigla;
    return saida;
}

//Funcao utilitaria que gera a matriz de hierarquias utilizada para facilitar
//a geracao do grafo de derivacao. Consiste numa chamada a uma busca recursiva
//entre as structs dimensao, comecando pela primeira dimensao cadastrada e continua por todas
//as suas agregacoes, pintando de preto as dimensoes encontradas.

//IMPORTANTE: Eh por causa dessa funcao que as dimensoes usadas como ancestrais devem ser
//cadastradas antes de seus descendentes.
int **gera_hierarquias(dimensoes dims, int *quantidade_hier, int **qtd_dimensoes_por_hier) {
    int i = 0, j = 0;

    int qtd_hierarquias = 0;
    int **hierarquias = NULL;
    int *qtd_dimensoes_por_hierarquia = NULL;

    //Pinta todas as dimensoes de branco, a fim de posteriormente pinta-las de preto
    //para marcar aquelas que ja foram contabilizadas
    pinta_todas_as_dimensoes(&dims, branco);

    //Itera sobre todas as dimensoes, buscando recursivamente suas agregacoes e colocando na matriz de
    //hierarquias
    for(i=0; i < dims.qtd_dimensoes; i++) {
        if(dims.lista_dimensoes[i].cor != preto) {
            qtd_hierarquias++;
            hierarquias = realloc(hierarquias, sizeof(int*)*qtd_hierarquias);
            hierarquias[qtd_hierarquias-1] = NULL;
            qtd_dimensoes_por_hierarquia = realloc(qtd_dimensoes_por_hierarquia, sizeof(int)*qtd_hierarquias);
            qtd_dimensoes_por_hierarquia[qtd_hierarquias-1] = 0;
            hierarquias[qtd_hierarquias-1] = descobre_hierarquia(&dims, i, &hierarquias[qtd_hierarquias-1],
                                                           &qtd_dimensoes_por_hierarquia[qtd_hierarquias-1]);
        }
    }
    //Reordenada cada hierarquia para que os ancestrais fiquem posicionados antes dos descendentes
    for(i=0; i < qtd_hierarquias; i++) {
        hierarquias[i] = inverte_vetor(&hierarquias[i], qtd_dimensoes_por_hierarquia[i]);
    }

    *quantidade_hier = qtd_hierarquias;
    qtd_dimensoes_por_hier[0] = qtd_dimensoes_por_hierarquia;
    return hierarquias;
}

//Funcao utilitaria que inverte um vetor de inteiros e substitui no
//vetor referenciado pelo ponteiro enviado por argumento
int *inverte_vetor(int **vetor, int qtd_elems) {
    //Indices pra controle de iteracao
    //Indice ascendente
    int i;
    //Indice descendente
    int j = qtd_elems-1;
    int *novo_vetor = malloc(sizeof(int)*qtd_elems);
    //Itera sobre os elementos do vetor original e inverte os valores
    for(i=0; i < qtd_elems; i++) {
        novo_vetor[j] = vetor[0][i];
        j--;
    }

    for(i=0; i < qtd_elems; i++) {
        vetor[0][i] = novo_vetor[i];
    }
    return vetor[0];
}

//Funcao recursiva que faz parte da busca em profundidade que gera
//a matriz de hierarquias, usada para gerar o grafo de derivacao.s
int *descobre_hierarquia(dimensoes *dims, int indice_dim, int **hierarquia_dimensao,
                         int *qtd_agregacoes_hierarquia) {
    //Condicao de parada da recursao. a dimensao nao tem mais nenhuma agregacao
    if(indice_dim == -1 || dims->lista_dimensoes[indice_dim].cor == preto) {
        return;
    }
    //chamada recursiva, passando a agregacao da dimensao atual
    descobre_hierarquia(dims, dims->lista_dimensoes[indice_dim].agregacao,
                        hierarquia_dimensao, qtd_agregacoes_hierarquia);
    //pinta a dimensao atual de preto, pra que, se a busca encontrar duas vezes a mesma dimensao,
    //ela nao vai ser adicionada duas vezes
    dims->lista_dimensoes[indice_dim].cor = preto;
    (*qtd_agregacoes_hierarquia)++;

    //coloca na matriz de hierarquias
    hierarquia_dimensao[0] = realloc(hierarquia_dimensao[0], sizeof(int)*(*qtd_agregacoes_hierarquia));
    hierarquia_dimensao[0][(*qtd_agregacoes_hierarquia)-1] = indice_dim;
    return hierarquia_dimensao[0];
}

//Recebe uma string contendo o codigo dot (linguagem do graphviz)
//e escreve em um arquivo.
void grava_codigo_dot_em_arquivo(char *codigo_dot) {
    FILE *f = fopen("grafo.dot", "w");
    //Tenta criar o arquivo. Se nao conseguir, é porque tem algo muito errado
    //e a funcao deve encerrar.
    if(f == NULL) {
        printf("Erro. O arquivo do grafo nao pode ser criado.\n\n");
        return;
    }
    //Escreve o codigo dot no arquivo
    fwrite(codigo_dot, sizeof(char), strlen(codigo_dot)+1, f);
    //Fecha o arquivo
    fclose(f);
    //Avisa o usuario
    printf("Foi gerado o arquivo grafo.dot com a visualizacao do grafo criado.\n\n");
}

//Grava todas as dimensoes e hierarquias cadastradas em um arquivo binario.
void grava_dados_arquivo(dimensoes dims) {
    FILE *f = fopen("dimensoes.dat", "wb");
    //Se nao conseguiu criar o arquivo, significa que houve um problema e funcao deve encerrar
    if(f == NULL) {
        printf("Erro. O arquivo especificado nao pode ser criado.\n");
        return;
    }
    //Escreve aquantidade de dimensoes
    fwrite(&(dims.qtd_dimensoes), sizeof(int), 1, f);

    //Variaveis auxiliares.
    //i e j sao indices pra controle do loop
    //tam_nome eh a variavel auxiliar pra colocar o tamanho do nome da dimensao
    //pra gravar no arquivo, porque a funcao fwrite exige um ponteiro
    //tam_atributo eh usada pra gravar o tamanho de cada atributo, pela mesma razao da anterior
    //cor. guarda a cor do vertice.
    int i, j, tam_nome, tam_atributo, cor;

    //Varre as dimensoes e grava uma a uma
    for(i=0; i < dims.qtd_dimensoes; i++) {
        //grava a sigla
        fwrite(&(dims.lista_dimensoes[i].sigla), sizeof(char), 1, f);
        tam_nome = strlen(dims.lista_dimensoes[i].nome)+1;
        //grava o tamanho do nome. isso eh necessario porque o vetor de chars tem tamanho variavel,
        //entao a funcao que vai ler depois tem que saber onde parar.
        fwrite(&tam_nome, sizeof(int), 1, f);
        //grava o nome da dimensao
        fwrite(dims.lista_dimensoes[i].nome, sizeof(char), strlen(dims.lista_dimensoes[i].nome)+1, f);
        //grava a quantidade de atributos da dimensao, pra conseguir recuperar depois
        fwrite(&(dims.lista_dimensoes[i].qtd_atributos), sizeof(int), 1, f);

        for(j=0; j < dims.lista_dimensoes[i].qtd_atributos; j++) {
            tam_atributo = strlen(dims.lista_dimensoes[i].atributos[j])+1;
            fwrite(&tam_atributo, sizeof(int), 1, f);
            fwrite(dims.lista_dimensoes[i].atributos[j], sizeof(char), tam_atributo, f);
        }

        fwrite(&(dims.lista_dimensoes[i].agregacao), sizeof(int), 1, f);
    }
    printf("Dados gravados com sucesso.\n\n");
    fclose(f);
}

//Procura o arquivo binario com dados de dimensoes e hierarquias e o carrega
dimensoes *carrega_dados_arquivo() {
    dimensoes dims;
    FILE *f = fopen("dimensoes.dat", "rb");
    //Se nao conseguiu abrir o arquivo, signfica que ele nao existe ou houve algum outro problema.
    if(f == NULL) {
        printf("O arquivo especificado nao existe ou nao pode ser aberto.\n");

        return NULL;
    }
    fread(&(dims.qtd_dimensoes), sizeof(int), 1, f);
    dims.lista_dimensoes = malloc(sizeof(dimensao)*dims.qtd_dimensoes);

    int i, j, k = 0, tam_nome = 0, tam_atributo = 0;

    //Carrega e le cada uma das dimensoes cadastradas
    for(i = 0; i < dims.qtd_dimensoes; i++) {
        dimensao dim;
        dim.nome = NULL;
        dim.atributos = NULL;
        fread(&(dim.sigla), sizeof(char), 1, f);
        fread(&(tam_nome), sizeof(int), 1, f);
        dim.nome = malloc(sizeof(char)*tam_nome);
        fread(dim.nome, sizeof(char), tam_nome, f);
        fread(&(dim.qtd_atributos), sizeof(int), 1, f);
        dim.atributos = malloc(sizeof(atributo)*dim.qtd_atributos);

        //Carrega e le cada um dos atributos
        for(j=0; j < dim.qtd_atributos; j++) {
            fread(&tam_atributo, sizeof(int), 1, f);
            dim.atributos[j] = malloc(sizeof(char)*tam_atributo);
            fread(dim.atributos[j], sizeof(char), tam_atributo, f);
        }
        fread(&(dim.agregacao), sizeof(int), 1, f);
        dims.lista_dimensoes[i] = dim;
    }
    printf("Dados carregados com sucesso.\n\n");
    fclose(f);
    return &dims;
}

//Funcao utilitaria usada na busca em profundidade. Pinta todas as dimensoes
//da cor especificada no parametro c
void pinta_todas_as_dimensoes(dimensoes *dims, cor c) {
    int i = 0;
    //Itera sobre as dimensoes, pintando uma a uma
    for(i=0; i < dims->qtd_dimensoes; i++) {
        dims->lista_dimensoes[i].cor = c;
    }
}

//Exibe todas as dimensoes com suas agregacoes. Para isso, realiza uma busca em profundidade
//para cada dimensao
void exibe_dimensoes_com_agregacoes(dimensoes dims) {
    printf("\n=====================\n");
    printf("AGREGACOES CADASTRADAS\n");
    printf("=====================\n");
    if(dims.qtd_dimensoes == 0) {
    //Usuario ainda nao cadastrou nada
        printf("Nao ha dimensoes cadastradas.\n");
    } else {
        //Usuario ja cadastrou alguma dimensao
        int i = 0;
        pinta_todas_as_dimensoes(&dims, branco);
        //Varre todas as dimensoes e imprime uma a uma,
        //chamando a funcao que imprime uma única dimensao
        for(i=0; i < dims.qtd_dimensoes; i++) {
            exibe_dimensao_com_agregacoes(&dims, i, 1);
            printf("\n");
        }
    }
    printf("\n");
}

//Imprime uma única dimensao com todas suas agregacoes por meio de uma busca recursiva
void exibe_dimensao_com_agregacoes(dimensoes *dims, int indice_dimensao, int primeiro) {
    //Encerra a busca em recursiva pra dimensao atual se a sua lista de agregacoes acabou
    if(dims->lista_dimensoes[indice_dimensao].cor == preto || indice_dimensao == -1) {
        return;
    }
    //chamada recursiva
    exibe_dimensao_com_agregacoes(dims, dims->lista_dimensoes[indice_dimensao].agregacao, 0);

    //imprime a dimensao atual
    printf("%s", dims->lista_dimensoes[indice_dimensao].nome);
    if(primeiro == 0) {
        //se nao for a ultima dimensao da hierarquia, coloca um "<" pra separar as agregacoes
        printf(" < ");
    }
    //pinta a dimensao atual de preto. usado pra contrlar as dimensoes pelas quais a busca ja passou
    dims->lista_dimensoes[indice_dimensao].cor = preto;

    return;
}

//Exibe todas as opcoes do menu
void exibe_menu() {
    printf("Sistema de data warehousing\n");
    printf("1. Adicionar nova dimensao\n");
    printf("2. Acrescentar um novo atributo a uma dimensao\n");
    printf("3. Exibir dimensoes e seus atributos\n");
    printf("4. Adicionar nova relacao de agregacao\n");
    printf("5. Exibir agregacoes\n");
    printf("6. Exibir grafo\n");
    printf("7. Gravar em arquivo\n");
    printf("8. Carregar de arquivo\n");
    printf("9. Sair\n");
    printf("Opcao: ");
}

//Funcao utilitaria que manda o usuario escolher uma dimensao,
//por meio dos numeros que a identifica na tela.
//Usado no cadastro de atributos e de agregacoes.
int escolhe_dimensao(dimensoes dims) {
    int opcao_dimensao = 0;
    exibe_dimensoes(dims);
    printf("\nEscolha a dimensao pelo NUMERO: ");
    scanf("%d", &opcao_dimensao);
    //Valida se a opcao digitada nao eh maior do que o numero de dimensoes cadastradas
    if(opcao_dimensao > dims.qtd_dimensoes-1) {
        printf("Numero de dimensao invalido.\n");
    } else {
        return opcao_dimensao;
    }
}

//Funcao que invoca o metodo que le um atributo e então coloca
//esse atributo na lista de atributos da dimensao passada como argumento
dimensao *acrescenta_atributo_numa_dimensao(dimensao *dim) {
    dim->qtd_atributos++;
    dim->atributos = realloc(dim->atributos, sizeof(atributo)*dim->qtd_atributos);
    atributo novo_atributo = le_atributo();
    dim->atributos[dim->qtd_atributos-1] = novo_atributo;
    return dim;
}

//Funcao que pede para o usuario digita o nome de um novo atributo na tela.
//Le esse atributo e retorna. Restricao: deve ter ate 50 caracteres.
atributo le_atributo() {
    atributo novo_atributo = malloc(sizeof(char)*50);
    printf("Digite o atributo: ");
    scanf("%s", novo_atributo);
    printf("\n");
    novo_atributo = realloc(novo_atributo, sizeof(char)*(strlen(novo_atributo)+1));
    return novo_atributo;
}

//Exibe todas as dimensoes com seus atributos
void exibe_dimensoes(dimensoes dims) {
    printf("\n=====================\n");
    printf("DIMENSOES CADASTRADAS\n");
    printf("=====================\n");
    if(dims.qtd_dimensoes == 0) {
        //Caso em que nenhuma dimensao foi cadastrada
        printf("Nao ha dimensoes cadastradas.\n");
    } else {
        //Caso em que ha dimensoes cadastradas.
        int i = 0;
        //Itera sobre cada uma das dimensoes cadastradas,
        //invocando a funcao que exibe uma unica dimensao com seus atributos
        for(i=0; i < dims.qtd_dimensoes; i++) {
            printf("%d\.", i);
            exibe_dimensao(dims.lista_dimensoes[i]);
        }
    }
    printf("\n");
}

//Funcao que exibe a dimensao recebida por argumento, com todos seus atributos
void exibe_dimensao(dimensao dim) {
    printf("%s", dim.nome);
    printf("(%c)\n", dim.sigla);
    int i;
    //Itera sobre cada um dos atributos da dimensao
    for(i=0; i < dim.qtd_atributos; i++) {
        printf("    Atr.%d: %s\n", i, dim.atributos[i]);
    }
}

//Le da tela todos os atributos de uma dimensao, cria a dimensao com esses atributos e retorna.
//Usado no cadastro de uma nova dimensao
dimensao le_nova_dimensao() {
    dimensao d;
    //O nome pode ter no maximo 50 caracteres
    char *nome_dimensao = malloc(sizeof(char)*50);
    char sigla_dimensao;
    //Le o nome
    printf("Digite o nome da dimensao: ");
    scanf("%s", nome_dimensao);
    //Realoca o nome pra ter o tamanho correto da string
    nome_dimensao = realloc(nome_dimensao, sizeof(char)*(strlen(nome_dimensao)+1));
    //Le a sigla
    printf("Digite a sigla que representara essa dimensao: ");
    scanf(" %c", &sigla_dimensao);
    d.nome = nome_dimensao;
    d.sigla = sigla_dimensao;
    //Coloca a agregacao da dimensao como -1, significando que nao tem agregacao cadastrada
    //para esta dimensao ainda
    d.agregacao = -1;
    //Inicializa a lista de atributos com null
    d.atributos = NULL;
    //A dimensao comeca com 0 atributos
    d.qtd_atributos = 0;
    return d;
}
