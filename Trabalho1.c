#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DESCR 100
#define MAXN 1000

/* Armazena um horário de chegada */
typedef struct {
    int hh;
    int mm;
    int ss;
} HORARIO;

/* Armazena informações de um processo */
typedef struct {
    int prior;
    HORARIO chegada;
    char descricao[MAX_DESCR];

    // variavel auxiliar que concatena os tempos de chegada
    // dessa forma será muito mais fácil ordenar os elementos
    int valor;
} CELULA;

/* Lista para lidar com os processos */
typedef struct {
    int tam;
    CELULA *celulaPrior [MAXN];
    CELULA *celulaTempo [MAXN];
} PROCESSOS;

// Quick Sort baseado em um vetor de inteiros por enquanto
void quickSort(int *vet, int inicio, int fim){
    int i = inicio;
    int j = fim;
    int pivo = vet[(i + j)/2]; // pivo medio, pode ser usado mediana de 3
    
    while(i <= j){
        while(vet[i] < pivo) i++;
        while(vet[j] > pivo) j--;
        
        if(i <= j){
            int aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            i++;
            j--;
        }
    }

    if(j > inicio) quickSort(vet, inicio, j);
    if(i < fim) quickSort(vet, i, fim);

    return;
}

//começando do fim da lista, vai deslocando todos os elementos
//um índice à direita até queencontre a posição ideal para inserir
void insereOrdenado(){

}

// começando do fim da lista, vai deslocando todos os elementos
// um índice à esquerda até que chegue no elemento a ser removido
void removeCelula(){

}

CELULA *criarCelula(void){
    CELULA *c = (CELULA *) malloc(sizeof(CELULA));
    if(c != NULL){
        scanf("%d %d:%d:%d %s", &c->prior, &c->chegada.hh, &c->chegada.mm, &c->chegada.ss, c->descricao);
        c->valor = (c->chegada.hh << 20) + (c->chegada.mm << 10) + c->chegada.ss;
    }

    return c;
}

void printarProcessos(CELULA *celulas[], int n){
    for(int i=0; i<n; i++){
        printf("%02d %02d:%02d:%02d %s\n", celulas[i]->prior, celulas[i]->chegada.hh, 
        celulas[i]->chegada.mm, celulas[i]->chegada.ss, celulas[i]->descricao);
    }
    
}

int main(){
    bool quickViavel = true; // O quickSort é feito somente no primeiro print.
    char keyword[10];
    PROCESSOS terminal1;

    do{
        scanf("%s", keyword);

        // Comando "add".
        if(strcmp(keyword, "add") == 0){
            CELULA *c1 = criarCelula();
            terminal1.celulaPrior[terminal1.tam] = c1;
            terminal1.celulaTempo[terminal1.tam] = c1;
            terminal1.tam++;

            //caso ja tenha realizado o quicksort
            if(!quickViavel) 
                insereOrdenado();
        }

        // Comando "next"
        else if(strcmp(keyword, "next") == 0){
            if(quickViavel){
                quickSort();
                quickViavel = false;
            }
        }

        // Comando "exec"
        else if(strcmp(keyword, "exec") == 0)
            removeCelula();
        
        // Comando "pinto"
        else if(strcmp(keyword, "print") == 0){
            if(quickViavel){
                quickSort();
                quickViavel = false;
            }

            char key;
            scanf(" -%c", &key);

            if(key == 'p')
                printarProcessos(terminal1.celulaPrior, terminal1.tam);
            else if(key == 't')
                printarProcessos(terminal1.celulaTempo, terminal1.tam);
        }

    } while(strcmp(keyword, "quit") != 0);

}