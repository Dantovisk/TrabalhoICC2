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
} horario;

/* Armazena informações de um processo */
typedef struct {
    int prior;
    horario chegada;
    char descricao[MAX_DESCR];

    // variavel auxiliar que concatena os tempos de chegada
    // dessa forma será muito mais fácil ordenar os elementos
    int valor;
} celula;

/* Lista para lidar com os processos */
typedef struct {
    int tam;
    celula *celulaPrior [MAXN];
    celula *celulaTempo [MAXN];
} processos;

void quicksort(){

}

//começando do fim da lista, vai deslocando todos os elementos
//um índice à direita até queencontre a posição ideal para inserir
void insereOrdenado(){

}

// começando do fim da lista, vai deslocando todos os elementos
// um índice à esquerda até que chegue no elemento a ser removido
void removeCelula(){

}

celula *ciarCelula(int prior, int hh, int mm, int ss, const char descricao[]){
    celula *c1 = (celula*)malloc(sizeof(celula));
    c1->chegada.hh = hh;
    c1->chegada.mm = mm;
    c1->chegada.ss = ss;

    c1->valor = (hh << 20) + (mm << 10) + ss;

    c1->prior = prior;
    strcpy(c1->descricao, descricao);

    return c1;
}

void PrintarProcessos(celula *celulas[], int n){
    for(int i=0; i<n; i++){
        printf("%02d %02d:%02d:%02d %s\n", celulas[i]->prior, celulas[i]->chegada.hh, 
        celulas[i]->chegada.mm, celulas[i]->chegada.ss, celulas[i]->descricao);
    }
    
}

int main(){
    bool quickViavel = true;
    char keyword[10] ;

    scanf("%s", keyword);

    processos terminal1;

    while(strcmp(keyword, "quit") != 0){
        int hh, mm, ss, prior;
        char descricao[100];

        if(strcmp(keyword, "add") == 0){
            scanf("%d %d:%d:%d %s", &prior, &hh, &mm, &ss, descricao);
            celula *c1 = ciarCelula(prior, hh, mm, ss, descricao);
            terminal1.celulaPrior[terminal1.tam] = c1;
            terminal1.celulaTempo[terminal1.tam] = c1;
            terminal1.tam++;

            //caso ja tenha realizado o quicksort
            if(!quickViavel) insereOrdenado();
        }
        else if(strcmp(keyword, "next") == 0){
            if(quickViavel){
                quicksort();
                quickViavel = false;
            }

        } 
        else if(strcmp(keyword, "exec") == 0){
            removeCelula();
        }
        else if(strcmp(keyword, "print") == 0){
            if(quickViavel){
                quicksort();
                quickViavel = false;
            }

            char key;
            scanf(" -%c", &key);

            if(key == 'p'){
                PrintarProcessos(terminal1.celulaPrior, terminal1.tam);
            } 
            else if(key =='t'){
                PrintarProcessos(terminal1.celulaTempo, terminal1.tam);
            }
            
            
        }


        scanf("%s", keyword);
    }

}