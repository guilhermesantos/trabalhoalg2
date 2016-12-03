#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo_lista.h"

typedef char* atributo;

typedef enum cor {
    preto,
    branco,
} cor;

typedef struct dimensao {
    char sigla;
    char *nome;
    int qtd_atributos;
    atributo *atributos;
    int agregacao;
    cor cor;
} dimensao;

typedef struct dimensoes {
    dimensao *lista_dimensoes;
    int qtd_dimensoes;
} dimensoes;


int *inverte_vetor(int **vetor, int qtd_elems);
void exibe_menu();
void exibe_dimensoes(dimensoes dims);
void exibe_dimensao(dimensao dim);

dimensao le_nova_dimensao();
dimensao *acrescenta_atributo_numa_dimensao(dimensao *dim);
atributo le_atributo();

int escolhe_dimensao(dimensoes dims);

void exibe_dimensao_com_agregacoes(dimensoes *dims, int indice_dimensao, int primeiro);
void exibe_dimensoes_com_agregacoes(dimensoes dims);

void pinta_todas_as_dimensoes(dimensoes *dims, cor c);
void grava_dados_arquivo(dimensoes dims);
dimensoes *carrega_dados_arquivo();
void grava_codigo_dot_em_arquivo(char *codigo_dot);
int **gera_hierarquias(dimensoes dims, int *quantidade_hier, int **qtd_dimensoes_por_hier);
int *descobre_hierarquia(dimensoes *dims, int indice_dim, int **hierarquia_dimensao,
                         int *qtd_agregacoes_hierarquia);

void insere_vertices_agregados(grafo *g, dimensoes dim,int **hier, int *qtd_h, int **qtd_dim_h);
void insere_vertices_agregados_recursao(dimensoes *dims, int indice_dimensao, int primeiro);

int main() {
    int opcao_menu = 0;
    int opcao_dimensao = 0;
    int ancestral;
    int descendente;
    int i, j;

    int quantidade_hierarquias;
    int *quantidade_dimensoes_por_hierarquia;
    int **hierarquias;

    dimensoes dims;
    dims.lista_dimensoes = NULL;
    dims.qtd_dimensoes = 0;

    grafo *g;
    char *codigo_dot;
    char gera_visualizacao = 'n';

    while(opcao_menu != 9) {
        exibe_menu();
        scanf("%d", &opcao_menu);

        switch(opcao_menu) {
        case 1://cadastrando nova dimensao
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
            printf("Escolha o ancestral: ");
            scanf("%d", &ancestral);
            printf("\n");

            exibe_dimensoes(dims);
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
            g = cria_grafo();
            
            //aqui funciona!!!
            /*
            insere_vertice(g, "a");
            insere_vertice(g, "b");
            insere_vertice(g, "c");
            insere_vertice(g, "d");
            insere_vertice(g, "e");
            insere_vertice(g, "f");
            insere_vertice(g, "g");

            insere_aresta_por_valor(g, "a", "b");
            insere_aresta_por_valor(g, "a", "c");
            insere_aresta_por_valor(g, "c", "d");
            insere_aresta_por_valor(g, "c", "e");
            insere_aresta_por_valor(g, "e", "f");
            insere_aresta_por_valor(g, "e", "g");
            insere_aresta_por_valor(g, "f", "f");
            */

            //hierarquias, quantidade_hierarquias e quantidade_dimensoes_por_hierarquia
            //foram declaradas no comeco da main porque o C da problema se declarar variavel no
            //meio do switch
            //Isso eh um exemplo de como varrer a matriz de hierarquias. Esse caso imprime a matriz
            
            hierarquias = gera_hierarquias(dims, &quantidade_hierarquias, &quantidade_dimensoes_por_hierarquia);
            char aux[2];
            for(i=0; i < quantidade_hierarquias; i++) {
                for(j = 0; j < quantidade_dimensoes_por_hierarquia[i]; j++) {
                    
                    aux[0] = dims.lista_dimensoes[(hierarquias[i][j])].sigla;
                    aux[1] = '\0';
                    insere_vertice(g, aux);
                    
                }
                printf("%s %d %d \n", aux, quantidade_hierarquias, quantidade_dimensoes_por_hierarquia[i]);
                printf("saiu do J\n");
            }
            //repete a ultima dimensao... pq?
            imprime_grafo(g);
            for(i = 0; i < quantidade_hierarquias - 1; i++){
                insere_aresta_por_numero(g, i, i + 1);
            }

            //insere_vertices_agregados(g, dims, hierarquias, &quantidade_hierarquias, &quantidade_dimensoes_por_hierarquia);
            codigo_dot = gera_codigo_dot(g);
            grava_codigo_dot_em_arquivo(codigo_dot);
            printf("Foi gerado o arquivo grafo.dot com o codigo para gerar a visualizacao do grafo.\n");

            printf("Se tiver o graphviz instalado, E REGISTRADO NA VARIAVEL DE AMBIENTE PATH,\n");
            printf("eh possivel gerar o arquivo de visualizacao do grafo.\n");
            printf("Gerar visualizacao? s/n: ");
            scanf(" %c", &gera_visualizacao);
            if(gera_visualizacao == 's') {
                printf("Gerando visualizacao...\n");
                system("dot -Tpng grafo.dot -o grafo.png");
                printf("Visualizacao gerada. Verificar o arquivo grafo.png.\n\n");
            } else {
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


void insere_vertices_agregados(grafo *g, dimensoes dims,int **hier, int *qtd_h, int **qtd_dim_h){
    //versao sem combinacao (teste)
    int i, j;
    //insere vertices
    for(i = 0; i < *qtd_h; i++){
        for(j = 0; j < **qtd_dim_h - 1; j++){
            char aux[2] = {dims.lista_dimensoes[(hier[i][j])].sigla, '\0'};
            insere_vertice(g, aux);
            printf("%s \n", aux);
        }
    }
    //liga hierarquias
    for(i = 0; i < *qtd_h; i++){
        for(j = 0; j < **qtd_dim_h; j++){
            char aux[2] = {dims.lista_dimensoes[(hier[i][j])].sigla, '\0'};
            char auxp[2] = {dims.lista_dimensoes[(hier[i][j + 1])].sigla, '\0'};
            insere_aresta_por_valor(g, aux, auxp);
            printf("%s \t %s \n", aux, auxp);
        }
    }
}




























int **gera_hierarquias(dimensoes dims, int *quantidade_hier, int **qtd_dimensoes_por_hier) {
    int i = 0, j = 0;

    int qtd_hierarquias = 0;
    int **hierarquias = NULL;
    int *qtd_dimensoes_por_hierarquia = NULL;

    pinta_todas_as_dimensoes(&dims, branco);

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

    for(i=0; i < qtd_hierarquias; i++) {
        hierarquias[i] = inverte_vetor(&hierarquias[i], qtd_dimensoes_por_hierarquia[i]);
    }

    *quantidade_hier = qtd_hierarquias;
    qtd_dimensoes_por_hier[0] = qtd_dimensoes_por_hierarquia;
    return hierarquias;
}

int *inverte_vetor(int **vetor, int qtd_elems) {
    int i;
    int j = qtd_elems-1;
    int *novo_vetor = malloc(sizeof(int)*qtd_elems);
    for(i=0; i < qtd_elems; i++) {
        novo_vetor[j] = vetor[0][i];
        j--;
    }

    for(i=0; i < qtd_elems; i++) {
        vetor[0][i] = novo_vetor[i];
    }
    return vetor[0];
}

int *descobre_hierarquia(dimensoes *dims, int indice_dim, int **hierarquia_dimensao,
                         int *qtd_agregacoes_hierarquia) {
    if(indice_dim == -1 || dims->lista_dimensoes[indice_dim].cor == preto) {
        return;
    }

    descobre_hierarquia(dims, dims->lista_dimensoes[indice_dim].agregacao,
                        hierarquia_dimensao, qtd_agregacoes_hierarquia);
    dims->lista_dimensoes[indice_dim].cor = preto;
    (*qtd_agregacoes_hierarquia)++;
    hierarquia_dimensao[0] = realloc(hierarquia_dimensao[0], sizeof(int)*(*qtd_agregacoes_hierarquia));
    hierarquia_dimensao[0][(*qtd_agregacoes_hierarquia)-1] = indice_dim;
    return hierarquia_dimensao[0];
}

void grava_codigo_dot_em_arquivo(char *codigo_dot) {
    FILE *f = fopen("grafo.dot", "w");
    if(f == NULL) {
        printf("Erro. O arquivo do grafo nao pode ser criado.\n\n");
        return;
    }
    fwrite(codigo_dot, sizeof(char), strlen(codigo_dot)+1, f);
    fclose(f);
    printf("Foi gerado o arquivo grafo.dot com a visualizacao do grafo criado.\n\n");
}

void grava_dados_arquivo(dimensoes dims) {
    FILE *f = fopen("dimensoes.dat", "wb");
    if(f == NULL) {
        printf("Erro. O arquivo especificado nao pode ser criado.\n");
        return;
    }
    fwrite(&(dims.qtd_dimensoes), sizeof(int), 1, f);
    int i, j, tam_nome, tam_atributo, cor;


    for(i=0; i < dims.qtd_dimensoes; i++) {
        fwrite(&(dims.lista_dimensoes[i].sigla), sizeof(char), 1, f);
        tam_nome = strlen(dims.lista_dimensoes[i].nome)+1;
        fwrite(&tam_nome, sizeof(int), 1, f);
        fwrite(dims.lista_dimensoes[i].nome, sizeof(char), strlen(dims.lista_dimensoes[i].nome)+1, f);
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

dimensoes *carrega_dados_arquivo() {
    dimensoes dims;
    FILE *f = fopen("dimensoes.dat", "rb");
    if(f == NULL) {
        printf("O arquivo especificado nao existe ou nao pode ser aberto.\n");

        return NULL;
    }
    fread(&(dims.qtd_dimensoes), sizeof(int), 1, f);
    dims.lista_dimensoes = malloc(sizeof(dimensao)*dims.qtd_dimensoes);

    int i, j, k = 0, tam_nome = 0, tam_atributo = 0;

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

void pinta_todas_as_dimensoes(dimensoes *dims, cor c) {
    int i = 0;
    for(i=0; i < dims->qtd_dimensoes; i++) {
        dims->lista_dimensoes[i].cor = c;
    }
}

void exibe_dimensoes_com_agregacoes(dimensoes dims) {
    printf("\n=====================\n");
    printf("AGREGACOES CADASTRADAS\n");
    printf("=====================\n");
    if(dims.qtd_dimensoes == 0) {
        printf("Nao ha dimensoes cadastradas.\n");
    } else {
        int i = 0;
        pinta_todas_as_dimensoes(&dims, branco);
        for(i=0; i < dims.qtd_dimensoes; i++) {
            exibe_dimensao_com_agregacoes(&dims, i, 1);
            printf("\n");
        }
    }
    printf("\n");
}

void exibe_dimensao_com_agregacoes(dimensoes *dims, int indice_dimensao, int primeiro) {
    if(dims->lista_dimensoes[indice_dimensao].cor == preto || indice_dimensao == -1) {
        return;
    }
    exibe_dimensao_com_agregacoes(dims, dims->lista_dimensoes[indice_dimensao].agregacao, 0);

    printf("%s", dims->lista_dimensoes[indice_dimensao].nome);
    if(primeiro == 0) {
        printf(" < ");
    }
    dims->lista_dimensoes[indice_dimensao].cor = preto;

    return;
}

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

int escolhe_dimensao(dimensoes dims) {
    int opcao_dimensao = 0;
    exibe_dimensoes(dims);
    printf("\nEscolha a dimensao: ");
    scanf("%d", &opcao_dimensao);

    if(opcao_dimensao > dims.qtd_dimensoes-1) {
        printf("Numero de dimensao invalido.\n");
    } else {
        return opcao_dimensao;
    }
}

dimensao *acrescenta_atributo_numa_dimensao(dimensao *dim) {
    dim->qtd_atributos++;
    dim->atributos = realloc(dim->atributos, sizeof(atributo)*dim->qtd_atributos);
    atributo novo_atributo = le_atributo();
    dim->atributos[dim->qtd_atributos-1] = novo_atributo;
    return dim;
}

atributo le_atributo() {
    atributo novo_atributo = malloc(sizeof(char)*50);
    printf("Digite o atributo: ");
    scanf("%s", novo_atributo);
    printf("\n");
    novo_atributo = realloc(novo_atributo, sizeof(char)*(strlen(novo_atributo)+1));
    return novo_atributo;
}

void exibe_dimensoes(dimensoes dims) {
    printf("\n=====================\n");
    printf("DIMENSOES CADASTRADAS\n");
    printf("=====================\n");
    if(dims.qtd_dimensoes == 0) {
        printf("Nao ha dimensoes cadastradas.\n");
    } else {
        int i = 0;
        for(i=0; i < dims.qtd_dimensoes; i++) {
            printf("%d\.", i);
            exibe_dimensao(dims.lista_dimensoes[i]);
        }
    }
    printf("\n");
}

void exibe_dimensao(dimensao dim) {
    printf("%s", dim.nome);
    printf("(%c)\n", dim.sigla);
    int i;
    for(i=0; i < dim.qtd_atributos; i++) {
        printf("    Atr.%d: %s\n", i, dim.atributos[i]);
    }
}

dimensao le_nova_dimensao() {
    dimensao d;
    char *nome_dimensao = malloc(sizeof(char)*50);
    char sigla_dimensao;
    printf("Digite o nome da dimensao: ");
    scanf("%s", nome_dimensao);
    nome_dimensao = realloc(nome_dimensao, sizeof(char)*(strlen(nome_dimensao)+1));
    printf("Digite a sigla que representara essa dimensao: ");
    scanf(" %c", &sigla_dimensao);
    d.nome = nome_dimensao;
    d.sigla = sigla_dimensao;
    d.agregacao = -1;
    d.atributos = NULL;
    d.qtd_atributos = 0;
    return d;
}
