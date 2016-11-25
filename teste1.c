#include <stdio.h>
#include <string.h>

void inserir(char in[]){

}

void remover(char in[]){

}

void imprimir(char in[]){

}

int main(){
	char input[100];
	//tipos de char a serem ignorados na tokenizacao
	const char separador[] = ">";
	char *token;
	while(1){
		system("clear");
		printf(">> ");
		scanf("%s", input);

		switch(input[0]){
			//input
			case 'i':
				inserir(input);
				break;
			case 'r':
				remover(input);
				break;
			case 'p':
				imprimir(input);
				break;
		}

	}


}