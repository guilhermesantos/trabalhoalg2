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
    //quantidade de vertices no grafo
    int qtd_vertices;
} grafo;

//Funcao que aloca espaco pro grafo e retorna seu ponteiro
grafo *cria_grafo();

//Funcao que insere vertice no grafo
grafo *insere_vertice(grafo *g, char *valor);

//Imprime um grafo
void imprime_grafo(grafo *g);

//Gera o codigo dot, linguagem do graphviz, do grafo e o retorna
//como uma string que pode ser gravada em arquivo ou exibida na tela
char *gera_codigo_dot(grafo *g);

//insere uma aresta entre dois vertices com base em seu indice no vetor de vertices.
//Acabou por ser usada apenas internamente, o programa principal utiliza a funcao que insere aresta por valor
grafo *insere_aresta_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino);

//Faz a mesma coisa que o insere aresta por numero, mas faz a aresta bidirecional
grafo *insere_aresta_bidirecional_por_numero(grafo *g, int numero_vertice_origem, int numero_vertice_destino);

//Insere a aresta com base no valor dos vertices. Utiliza internamente a funcao insere aresta por numero
grafo *insere_aresta_por_valor(grafo *g, char *valor_vertice_origem, char *valor_vertice_destino);

//Faz a mesma coisa que o insere aresta por valor, mas faz a aresta bidrecional
grafo *insere_aresta_bidirecional_por_valor(grafo *g, char *valor_vertice_origem, char *valor_vertice_destino);
#endif // GRAFO
