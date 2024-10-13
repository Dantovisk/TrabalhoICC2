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

// Quick Sort com celular
void quickSort(CELULA *vet[], int inicio, int fim, bool isPrior){
    int i = inicio;
    int j = fim;
    CELULA *pivo = vet[(i + j)/2]; // pivo medio, pode ser usado mediana de 3
    
    while(i <= j){
        if (isPrior)
        {
            while(vet[i]->prior < pivo->prior) i++;
            while(vet[j]->prior > pivo->prior) j--;
        }
        else{
            while(vet[i]->valor > pivo->valor) i++;
            while(vet[j]->valor < pivo->valor) j--;
        }

        if(i <= j){
            CELULA *aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            i++;
            j--;
        }
        
    }
    
    if(j > inicio) quickSort(vet, inicio, j, isPrior);
    if(i < fim) quickSort(vet, i, fim, isPrior);

    return;
}

//começando do fim da lista, vai deslocando todos os elementos
//um índice à direita até queencontre a posição ideal para inserir
void insereOrdenado(CELULA *c1, PROCESSOS *terminal){
    if(terminal->tam != MAXN){
        int i;
        for(i = terminal->tam - 1; terminal->celulaPrior[i]->prior > c1->prior && i >= 0; i--)
            terminal->celulaPrior[i + 1] = terminal->celulaPrior[i];

        terminal->celulaPrior[i + 1] = c1;

        for(i = terminal->tam - 1; terminal->celulaTempo[i]->valor < c1->valor && i >= 0; i--)
            terminal->celulaTempo[i + 1] = terminal->celulaTempo[i];

        terminal->celulaTempo[i + 1] = c1;

        terminal->tam++;
    }

    return; 
}

// começando do fim da lista, vai deslocando todos os elementos
// um índice à esquerda até que chegue no elemento a ser removido
void removeCelula(PROCESSOS *terminal){
    char key;
    scanf(" -%c", &key);

    if(terminal->tam != 0){
        if(key == 'p'){
            for(int i = 0; i < terminal->tam; i++){
                if(terminal->celulaTempo[i] == terminal->celulaPrior[terminal->tam - 1])
                    for(int j = i; j < terminal->tam - 1; j++)
                        terminal->celulaTempo[j] = terminal->celulaTempo[j + 1];
            }
            free(terminal->celulaPrior[terminal->tam - 1]);
            terminal->celulaPrior[terminal->tam - 1] = NULL;
        }
        else if(key == 't'){
            for(int i = 0; i < terminal->tam; i++){
                if(terminal->celulaPrior[i] == terminal->celulaTempo[terminal->tam - 1])
                    for(int j = i; j < terminal->tam - 1; j++)
                        terminal->celulaPrior[j] = terminal->celulaPrior[j + 1];
            }
            free(terminal->celulaTempo[terminal->tam - 1]);
            terminal->celulaTempo[terminal->tam - 1] = NULL;
        }
    }

    terminal->tam--;
    return;
}

CELULA *criarCelula(void){
    CELULA *c = (CELULA *) malloc(sizeof(CELULA));
    if(c != NULL){
        scanf("%d %d:%d:%d %s", &c->prior, &c->chegada.hh, &c->chegada.mm, &c->chegada.ss, c->descricao);
        c->valor = (c->chegada.hh << 20) + (c->chegada.mm << 10) + c->chegada.ss;
    }

    return c;
}

// VAI TER QUE FAZER BUSCA BINARIA SE PA!
void mudarCelula(PROCESSOS *terminal){
    char key;
    scanf(" -%c", &key);

    int i;
    if(key == 'p'){
        int prior_old, prior_new;
        scanf("%d|%d", &prior_old, &prior_new);

        for(i = 0; i < terminal->tam; i++){
            if(terminal->celulaPrior[i]->prior == prior_old){
                terminal->celulaPrior[i]->prior = prior_new;
                break;
            }
        }
    } else if(key == 't'){
        int hh_old, mm_old, ss_old, hh_new, valor_old;
        HORARIO new;
        scanf("%d:%d:%d|%d:%d:%d", &hh_old, &mm_old, &ss_old, &new.hh, &new.mm, &new.ss);
        valor_old = (hh_old << 20) + (mm_old << 10) + ss_old;
        
        for(i = 0; i < terminal->tam; i++){
            if(terminal->celulaTempo[i]->valor == valor_old){
                terminal->celulaTempo[i]->chegada = new;
                terminal->celulaTempo[i]->valor = (new.hh << 20) + (new.mm << 10) + new.ss;
                break;
            }
        }
    }

    return;
}

void printarProcessos(CELULA *celulas[], int n){
    for(int i = n-1; i >= 0; i--){
        printf("%02d %02d:%02d:%02d %s\n", celulas[i]->prior, celulas[i]->chegada.hh, 
        celulas[i]->chegada.mm, celulas[i]->chegada.ss, celulas[i]->descricao);
    }
    
}

int main(){
    bool quickViavel = true; // O quickSort é feito somente no primeiro print.
    char keyword[10];
    PROCESSOS terminal1;
    terminal1.tam = 0;
    
    do{
        scanf("%s", keyword);

        // Comando "add".
        if(strcmp(keyword, "add") == 0){
            CELULA *c1 = criarCelula();

            if(quickViavel){
                terminal1.celulaPrior[terminal1.tam] = c1;
                terminal1.celulaTempo[terminal1.tam] = c1;
                terminal1.tam++;
            }
            else //caso ja tenha realizado o quicksort
                insereOrdenado(c1, &terminal1);
        }

        // Comando "exec"
        else if(strcmp(keyword, "exec") == 0){
            if(quickViavel){
                quickSort(terminal1.celulaPrior, 0, terminal1.tam-1, true);
                quickSort(terminal1.celulaTempo, 0, terminal1.tam-1, false);
                quickViavel = false;
            }

            removeCelula(&terminal1);
        }

        // Comando "next"
        else if(strcmp(keyword, "next") == 0){
            if(quickViavel){
                quickSort(terminal1.celulaPrior, 0, terminal1.tam-1, true);
                quickSort(terminal1.celulaTempo, 0, terminal1.tam-1, false);
                quickViavel = false;
            }

            char key;
            scanf(" -%c", &key);

            if(key == 'p')
                printf("%02d %02d:%02d:%02d %s\n", terminal1.celulaPrior[terminal1.tam - 1]->prior, terminal1.celulaPrior[terminal1.tam - 1]->chegada.hh, 
                terminal1.celulaPrior[terminal1.tam - 1]->chegada.mm, terminal1.celulaPrior[terminal1.tam - 1]->chegada.ss, terminal1.celulaPrior[terminal1.tam - 1]->descricao);
            else if(key == 't')
                printf("%02d %02d:%02d:%02d %s\n", terminal1.celulaTempo[terminal1.tam - 1]->prior, terminal1.celulaTempo[terminal1.tam - 1]->chegada.hh, 
                terminal1.celulaTempo[terminal1.tam - 1]->chegada.mm, terminal1.celulaTempo[terminal1.tam - 1]->chegada.ss, terminal1.celulaTempo[terminal1.tam - 1]->descricao);
        }
        
        // sim dante, esse comando está feito com a bunda, fiquei com preguiça de mover a célula antiga pra posição ordenada, lide com isso!
        else if(strcmp(keyword, "change") == 0){
            mudarCelula(&terminal1);

            quickSort(terminal1.celulaPrior, 0, terminal1.tam-1, true);
            quickSort(terminal1.celulaTempo, 0, terminal1.tam-1, false);
        }
        
        // Comando "pinto"
        // HAaha muito engraçado dantovisk
        else if(strcmp(keyword, "print") == 0){
            if(quickViavel){
                quickSort(terminal1.celulaPrior, 0, terminal1.tam-1, true);
                quickSort(terminal1.celulaTempo, 0, terminal1.tam-1, false);
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

    for(int i = 0; i < terminal1.tam; i++){
        free(terminal1.celulaPrior[i]);
        terminal1.celulaPrior[i] = NULL;
    }

    return 0;
}