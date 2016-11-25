#ifndef GRAFO
#define GRAFO
#include <stdio.h>
#include <stdlib.h>


typedef struct no_adj {
    int num_vertice;
    struct no_adj *prox;
} no_adj;

typedef struct vertice {
    char *valor;
    int qtd_adj;
    struct no_adj* lista_adj;
} vertice;

typedef struct grafo {
    vertice *vertices;
    int qtd_vertices;
} grafo;

grafo *cria_grafo();
grafo *insere_vertice(grafo *g, char *valor);
void imprime_grafo(grafo *g);

grafo *insere_aresta_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino);
grafo *insere_aresta_bidirecional_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino);
grafo *insere_aresta_por_valor(grafo *g, char *valor_vertice_origem, char *valor_vertice_destino);
grafo *insere_aresta_bidirecional_por_valor(grafo *g, char *valor_vertice_origem, char *valor_vertice_destino);
#endif // GRAFO
