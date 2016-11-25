#include "grafo_lista.h"
#include <string.h>

int busca_numero_do_vertice_por_valor(grafo *g, char *valor);

grafo *cria_grafo() {
    grafo *g = (grafo*)malloc(sizeof(grafo));
    g->qtd_vertices = 0;
    g->vertices = NULL;
    return g;
}

grafo *insere_vertice(grafo *g, char *val) {
    g->qtd_vertices++;
    g->vertices = realloc(g->vertices, sizeof(vertice)*g->qtd_vertices);
    g->vertices[g->qtd_vertices-1].valor = val;
    g->vertices[g->qtd_vertices-1].qtd_adj = 0;
    g->vertices[g->qtd_vertices-1].lista_adj = NULL;
    return g;
}

grafo *insere_aresta_bidirecional_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino) {
    insere_aresta_por_numero(g, numero_vertice_origem, numero_vertice_destino);
    insere_aresta_por_numero(g, numero_vertice_destino, numero_vertice_origem);
    return g;
}

grafo *insere_aresta_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino) {
    no_adj *antigo_inicio_da_lista = g->vertices[numero_vertice_origem].lista_adj;

    no_adj *novo_inicio_da_lista = (no_adj*)malloc(sizeof(no_adj));
    novo_inicio_da_lista->num_vertice = numero_vertice_destino;
    novo_inicio_da_lista->prox = antigo_inicio_da_lista;

    g->vertices[numero_vertice_origem].lista_adj = novo_inicio_da_lista;
    g->vertices[numero_vertice_origem].qtd_adj++;
    return g;
}

grafo *insere_aresta_por_valor(grafo *g, char *valor_vertice_origem,
                            char *valor_vertice_destino) {
    int numero_vertice_origem = busca_numero_do_vertice_por_valor(g, valor_vertice_origem);
    int numero_vertice_destino = busca_numero_do_vertice_por_valor(g, valor_vertice_destino);
    insere_aresta_por_numero(g, numero_vertice_origem, numero_vertice_destino);

    return g;
}

grafo *insere_aresta_bidirecional_por_valor(grafo *g, char *valor_vertice_origem,
                                            char *valor_vertice_destino) {
    insere_aresta_por_valor(g, valor_vertice_origem, valor_vertice_destino);
    insere_aresta_por_valor(g, valor_vertice_destino, valor_vertice_origem);

    return g;
}

int busca_numero_do_vertice_por_valor(grafo *g, char *valor) {
    int i;
    for(i=0; i < g->qtd_vertices; i++) {
        if(strcmp(g->vertices[i].valor, valor) == 0) {
            return i;
        }
    }
    return -1;
}

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
        for(i=0; i < g->qtd_vertices; i++) {
            printf("[%d|%s]", i, g->vertices[i].valor);
            no_adj *adjacencia = g->vertices[i].lista_adj;
            for(j = 0; j < g->vertices[i].qtd_adj; j++) {
                printf("->[%d|%s]", adjacencia->num_vertice, g->vertices[adjacencia->num_vertice].valor);
                adjacencia = adjacencia->prox;
            }
            printf("\n");
        }
    }
}
