#include "grafo_lista.h"
#include <string.h>

int busca_numero_do_vertice_por_valor(grafo *g, char *valor);
char *stringfica_aresta(vertice v1, vertice v2);

//Funcao que aloca espaco pro grafo e retorna seu ponteiro
grafo *cria_grafo() {
    grafo *g = (grafo*)malloc(sizeof(grafo));
    g->qtd_vertices = 0;
    g->vertices = NULL;
    return g;
}

//Funcao que insere vertice no grafo
grafo *insere_vertice(grafo *g, char *val) {
    if(busca_numero_do_vertice_por_valor(g, val) != -1) return g;
    g->qtd_vertices++;
    //aloca espaco pro vertice
    g->vertices = realloc(g->vertices, sizeof(vertice)*g->qtd_vertices);
    //coloca o vertice no final do vetor de vertices
    g->vertices[g->qtd_vertices-1].valor = val;
    g->vertices[g->qtd_vertices-1].qtd_adj = 0;
    g->vertices[g->qtd_vertices-1].lista_adj = NULL;
    return g;
}

//Insere aresta bidrecional com base nos indices dos vertices no vetor de vertices
//Funciona simplesmente chamando duas vezes a funcao que insere aresta unidirecional, trocando
//origem e destino
grafo *insere_aresta_bidirecional_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino) {
    insere_aresta_por_numero(g, numero_vertice_origem, numero_vertice_destino);
    insere_aresta_por_numero(g, numero_vertice_destino, numero_vertice_origem);
    return g;
}

//Insere aresta unidirecional com base nos indices dos vertices no vetor de vertices
grafo *insere_aresta_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino) {
    no_adj *antigo_inicio_da_lista = g->vertices[numero_vertice_origem].lista_adj;

    //Aloca espaco pro novo no da lista de adjacencia do vertice de origem
    no_adj *novo_inicio_da_lista = (no_adj*)malloc(sizeof(no_adj));
    novo_inicio_da_lista->num_vertice = numero_vertice_destino;
    novo_inicio_da_lista->prox = antigo_inicio_da_lista;

    //Insere o novo no no inicio da lista de adjacencias do vertice de origem
    g->vertices[numero_vertice_origem].lista_adj = novo_inicio_da_lista;
    g->vertices[numero_vertice_origem].qtd_adj++;
    return g;
}

//Insere aresta unidirecional com base no valor dos vertices que devem ser ligados
grafo *insere_aresta_por_valor(grafo *g, char *valor_vertice_origem,
                            char *valor_vertice_destino) {
    //Busca o numero do indice dos dois vertices a serem ligados com base no valor
    int numero_vertice_origem = busca_numero_do_vertice_por_valor(g, valor_vertice_origem);
    int numero_vertice_destino = busca_numero_do_vertice_por_valor(g, valor_vertice_destino);
    //Insere aresta com base nos numeros encontrados
    insere_aresta_por_numero(g, numero_vertice_origem, numero_vertice_destino);

    return g;
}

//Insere aresta bidirecional com base nos valores dos vertices que devem ser ligados
//Funciona chamando duas vezes a funcao que insere aresta unidirecional, mas invertendo origem e destino
//cada vez
grafo *insere_aresta_bidirecional_por_valor(grafo *g, char *valor_vertice_origem,
                                            char *valor_vertice_destino) {
    insere_aresta_por_valor(g, valor_vertice_origem, valor_vertice_destino);
    insere_aresta_por_valor(g, valor_vertice_destino, valor_vertice_origem);

    return g;
}

//Funcao utilitaria. Busca o indice do vertice no vetor de vertices
//com base no valor do argumento
int busca_numero_do_vertice_por_valor(grafo *g, char *valor) {
    int i;
    for(i=0; i < g->qtd_vertices; i++) {
        if(strcmp(g->vertices[i].valor, valor) == 0) {
            return i;
        }
    }
    return -1;
}

//Gera codigo dot do grafo recebido como argumento, utilizado pelo graphviz para gerar
//a visualizacao do grafo. Retorna o codigo no formato de string para impressao na tela
//ou escrita em arquivo
char *gera_codigo_dot(grafo *g) {
    char declaracao[] = "digraph derivacao {\n";
    char encerramento[] = "}";
    char *codigo = malloc(sizeof(char)*4000);
    strcpy(codigo, declaracao);

    int i;
    no_adj *adj_atual;
    //Itera sobre todos os vertices
    for(i=0; i < g->qtd_vertices; i++) {
        vertice v = g->vertices[i];
        adj_atual = g->vertices[i].lista_adj;
        //Itera sobre a lista de adjacencia do vertice, gerando uma representacao
        //da aresta em string e concatenando no codigo gerado ate a iteracao atual
        while(adj_atual != NULL) {
            strcat(codigo, "\t");
            strcat(codigo, stringfica_aresta(v, g->vertices[adj_atual->num_vertice]));
            strcat(codigo, ";\n");
            adj_atual = adj_atual->prox;
        }
    }

    strcat(codigo, encerramento);
    return codigo;
}

//Funcao utiliraria que recebe dois vertices e retorna uma string formatada
//representando a aresta entre os dois. Utilizada pela funcao gera_codigo_dot.
char *stringfica_aresta(vertice v1, vertice v2) {
    //Concatena o valor dos dois vertices com um -> entre eles
    //o -> indica uma aresta direcionada na linguagem do graphviz
    char *resultado = malloc(strlen(v1.valor)+strlen(" -> ")+strlen(v2.valor)+1);
    strcpy(resultado, v1.valor);
    strcat(resultado, " -> ");
    strcat(resultado, v2.valor);
    return resultado;
}

//Imprime grafo na tela para fins de debug.
void imprime_grafo(grafo *g) {
    printf("=================\n");
    printf("IMPRIMINDO\n");
    printf("=================\n");
    if(g->qtd_vertices == 0) {
        printf("O grafo esta vazio.\n");
    } else {
        printf("Quantidade de nos no grafo: %d\n", g->qtd_vertices);
        int i;
        int j;
        //Itera sobre todos os vertices
        for(i=0; i < g->qtd_vertices; i++) {
            printf("[%d|%s]", i, g->vertices[i].valor);
            no_adj *adjacencia = g->vertices[i].lista_adj;
            //Itera sobre a lista de adjacencia de cada vertice, imprimindo cada um
            for(j = 0; j < g->vertices[i].qtd_adj; j++) {
                printf("->[%d|%s]", adjacencia->num_vertice, g->vertices[adjacencia->num_vertice].valor);
                adjacencia = adjacencia->prox;
            }
            printf("\n");
        }
    }
}
