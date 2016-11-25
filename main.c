#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo_lista.h"

typedef char* atributo;

typedef struct dimensao {
    char sigla;
    char *nome;
    int qtd_atributos;
    atributo *atributos;
    struct dimensao *agregacao;
} dimensao;

typedef struct dimensoes {
    dimensao *lista_dimensoes;
    int qtd_dimensoes;
} dimensoes;

void exibe_menu();
void exibe_dimensoes(dimensoes dims);
void exibe_dimensao(dimensao dim);
dimensao le_nova_dimensao();
dimensao *acrescenta_atributo_numa_dimensao(dimensao *dim);
atributo le_atributo();
int escolhe_dimensao(dimensoes dims);

int main() {
    int opcao_menu = 0;
    int opcao_dimensao = 0;

    dimensoes dims;
    dims.lista_dimensoes = NULL;
    dims.qtd_dimensoes = 0;

    while(opcao_menu != 4) {
        exibe_menu();
        scanf("%d", &opcao_menu);

        switch(opcao_menu) {
        case 1:
            dims.qtd_dimensoes++;
            dims.lista_dimensoes = realloc(dims.lista_dimensoes, sizeof(dimensao)*dims.qtd_dimensoes);
            dims.lista_dimensoes[dims.qtd_dimensoes-1] = le_nova_dimensao();
            exibe_dimensoes(dims);
            break;
        case 2:
            opcao_dimensao = escolhe_dimensao(dims);
            dims.lista_dimensoes[opcao_dimensao] =
            *acrescenta_atributo_numa_dimensao(&(dims.lista_dimensoes[opcao_dimensao]));

            break;
        case 3:
            exibe_dimensoes(dims);
            break;
        case 4:
            printf("\nEscolha o ancestral: \n");
            opcao_dimensao = escolhe_dimensao(dims);
            int ancestral = opcao_dimensao;

            printf("\nEscolha o descendente: \n");
            opcao_dimensao = escolhe_dimensao(dims);
            int descendente = opcao_dimensao;
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            exit(0);
        default:
            printf("\n Opcao invalida.\n");
            break;
        }
    }
    return 0;
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
    int i = 0;
    for(i=0; i < dims.qtd_dimensoes; i++) {
        printf("%d\.", i);
        exibe_dimensao(dims.lista_dimensoes[i]);
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
    d.agregacao = NULL;
    d.atributos = NULL;
    d.qtd_atributos = 0;
    return d;
}

void exibe_menu() {
    printf("Sistema de data warehousing\n");
    printf("1. Adicionar nova dimensao\n");
    printf("2. Acrescentar um novo atributo a uma dimensao\n");
    printf("3. Exibir dimensoes\n");
    printf("4. Adicionar nova relacao de agregacao\n");
    printf("5. Exibir dimensoes agregadas\n");
    printf("6. Exibir grafo\n");
    printf("7. Gravar em arquivo\n");
    printf("8. Carregar de arquivo\n");
    printf("9. Sair\n");
    printf("Opcao: ");
}
