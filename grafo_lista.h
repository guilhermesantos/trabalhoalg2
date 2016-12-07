#ifndef GRAFO
#define GRAFO
#include <stdio.h>
#include <stdlib.h>

//Estrutura que representa a lista de adjacencias de cada vertice
typedef struct no_adj {
    //num_vertice eh o indice do vertice adjacente no vetor de vertices do grafo
    int num_vertice;
    //proxima adjacencia
    struct no_adj *prox;
} no_adj;

//Estrutura que representa o vertice do grafo
typedef struct vertice {
    //Guarda o valor do vertice. No caso, uma string que vai ser
    //impressa no grafo de derivacao
    char *valor;
    //guarda quantidade de nos a lista de adjacencia deste vertice tera
    int qtd_adj;
    //Lista de adjacencia do vertice atual
    struct no_adj* lista_adj;
} vertice;

//Estrutura do TAD grafo
typedef struct grafo {
    //Vetor de vertices
    vertice *vertices;
    int qtd_vertices;
} grafo;

grafo *cria_grafo();
grafo *insere_vertice(grafo *g, char *valor);
void imprime_grafo(grafo *g);
char *gera_codigo_dot(grafo *g);

grafo *insere_aresta_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino);
grafo *insere_aresta_bidirecional_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino);
grafo *insere_aresta_por_valor(grafo *g, char *valor_vertice_origem, char *valor_vertice_destino);
grafo *insere_aresta_bidirecional_por_valor(grafo *g, char *valor_vertice_origem, char *valor_vertice_destino);
#endif // GRAFO
