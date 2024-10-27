#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DESCR 100
#define MAXN 1000
#define INF 100000007  

/* Armazena um horário de chegada 
hh: horas
mm: minutos
ss: segundos
*/
typedef struct horario_ {
    int hh;
    int mm;
    int ss;
} HORARIO;

/* Armazena informações de um processo 
prior: número na fila de prioridade
valor: variável auxiliar com o horário concatenado
chegada: struct que guarda o horário em campos (horas, minutos e segundos)
descricao[MAX_DESCR]: descrição do processo
*/
typedef struct celula_ {
    int prior;
    int valor;
    HORARIO chegada;
    char descricao[MAX_DESCR];
} CELULA;

/* Lista para lidar com os processos
tam: quantidade de elementos
celulaPrior: uma célula ordenada pela prioridade
celulaTempo: uma célula ordenada pelo tempo 
*/
typedef struct processos_ {
    int tam;
    int priorNaoOrdenados; //numero de elementos recentemente adicionados em celulaPrior
    int tempoNaoOrdenados; //numero de elementos recentemente adicionados em celulaTempo
    CELULA *celulaPrior [MAXN];
    CELULA *celulaTempo [MAXN];
    CELULA *bestPrior;
    CELULA *bestTempo;
} PROCESSOS;


/* A função inicializa as células com valores absurdos para ser possível*/
CELULA *inicializaBestCelula() {
    CELULA *celula = (CELULA *) malloc(sizeof(CELULA));
    if(celula == NULL) {
        printf("Erro na alocação");
        exit(1);
    } 
    celula->prior = -1;
    celula->valor = INF;
    memset(celula->descricao, 0, sizeof(celula->descricao));
    celula->chegada = (HORARIO) {INF, INF, INF};
    return celula;
}

/* A função faz a alocação dinâmica da memória utilizada para a struct CELULA, já recebendo do usuário os valores que serão
inicializados nos campos da struct, sejam eles: a prioridade, o horário, a descrição. Faz-se a conversão do valor.
*/
CELULA *criarCelula(PROCESSOS *terminal){
    CELULA *c = (CELULA *) malloc(sizeof(CELULA));
    if(c != NULL){
        scanf("%d %d:%d:%d %s", &c->prior, &c->chegada.hh, &c->chegada.mm, &c->chegada.ss, c->descricao);
        c->valor = (c->chegada.hh << 20) + (c->chegada.mm << 10) + c->chegada.ss;
        if(terminal->bestPrior == NULL || (c->prior > terminal->bestPrior->prior)) {
            terminal->bestPrior = c;
        }
        if(terminal->bestTempo == NULL || (c->valor < terminal->bestTempo->valor)) {
            terminal->bestTempo = c;
        }
    }
    return c;
}

//Sabemos que 8=D, mas sabemos se C=8?   if(C==8) printf("C=8");

/* A função faz um Quicksort recursivamente, a fim de tornar o vetor ordenado em O(n log n)
*/
void quickSort(CELULA *vet[], int inicio, int fim, bool isPrior){
    int i = inicio;
    int j = fim;
    // Pivô medio
    CELULA *pivo = vet[(i + j)/2]; 
    
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

void insertionSort(CELULA *vet[], int n, bool isPrior){
    int i, j;
    CELULA *x;

    for(i = 1; i < n; i++){
        x = vet[i];
        j = i - 1;

        if (isPrior) {
            // Ordena com base no campo 'prior'
            while(j >= 0 && vet[j]->prior > x->prior){
                vet[j + 1] = vet[j];
                j--;
            }
        } else {
            // Ordena com base no campo 'valor'
            while(j >= 0 && vet[j]->valor < x->valor){
                vet[j + 1] = vet[j];
                j--;
            }
        }
        
        vet[j + 1] = x;
    }

    return;
}

void ordenar(PROCESSOS *terminal, bool isPrior){
    int x = (isPrior ? terminal->priorNaoOrdenados : terminal->tempoNaoOrdenados);

    //quando o numero de elementos nao ordenados for pequeno realizaremos insertion sort
    if (x <= 5)
    {insertionSort((isPrior ? terminal->celulaPrior :terminal->celulaTempo), terminal->tam, isPrior); printf("Insertion sort da silva\n");}
    else {
        printf("Quicksort da silva\n");
        quickSort((isPrior ? terminal->celulaPrior :terminal->celulaTempo), 0, terminal->tam-1, isPrior);
    }

    if (isPrior) terminal->priorNaoOrdenados = 0;
    else terminal->tempoNaoOrdenados = 0;
}

/* A função faz a inserção da célula, sem se importar com ordenação, apenas se insere no próximo espaço vago
*/
void inserir(CELULA *c1, PROCESSOS *terminal){
    if(terminal->tam < MAXN){
        terminal->celulaTempo[terminal->tam] = c1;
        terminal->celulaPrior[terminal->tam] = c1;
        terminal->tam++;
        terminal->tempoNaoOrdenados++;
        terminal->priorNaoOrdenados++;
    }

    return; 
}

void removeCelula(PROCESSOS *terminal){
    char key;
    scanf(" -%c", &key);

    if (terminal->tam == 0) return;

    if (key == 'p') {
        ordenar(terminal, true);
        CELULA *aux = terminal->celulaPrior[terminal->tam-1];
        for(int i=0; i < terminal->tam; i++) {
            if(terminal->celulaTempo[i]->prior == aux->prior) {
                for(int j=i; j < terminal->tam-1; j++) {
                    terminal->celulaTempo[j] = terminal->celulaTempo[j+1];
                }
                break;
            }
        }
        terminal->bestPrior = terminal->celulaPrior[terminal->tam-2];
        free(aux);
        aux = NULL;
    } else if (key == 't') {
        ordenar(terminal, false);
        CELULA *aux = terminal->celulaTempo[terminal->tam-1];
        for(int i=0; i < terminal->tam; i++) {
            if(terminal->celulaPrior[i]->valor == aux->valor) {
                for(int j = i; j < terminal->tam-1; j++) {
                    terminal->celulaPrior[j] = terminal->celulaPrior[j+1];
                }
            }
        }
        terminal->bestTempo = terminal->celulaTempo[terminal->tam-2];
        free(aux);
        aux = NULL;
    }
    terminal->tam--;
    if(terminal->tam == 0){
        terminal->bestTempo = NULL;
        terminal->bestPrior = NULL;
    }
    return;
}



/* A função faz a mudança das informações da célula e, caso ela seja melhor que as outras em tempo e/ou prioridade, troca a vigente
*/
void mudarCelula(PROCESSOS *terminal){
    char key;
    scanf(" -%c", &key);

    if(key == 'p'){

        int prior_old, prior_new;
        scanf("%d|%d", &prior_old, &prior_new);

        for(int i = 0; i < terminal->tam; i++){
            // TROCAR EM PRIOR
            if(terminal->celulaPrior[i]->prior == prior_old){
                terminal->celulaPrior[i]->prior = prior_new;
            }
            // TROCAR EM TEMPO
            if(terminal->celulaTempo[i]->prior == prior_old){
                terminal->celulaTempo[i]->prior = prior_new;
            }
        }
        ordenar(terminal, true);

        terminal->bestPrior = terminal->celulaPrior[terminal->tam-1];

    } else if(key == 't'){

        int hh_old, mm_old, ss_old, valor_new, valor_old;
        HORARIO new;
        scanf("%d:%d:%d|%d:%d:%d", &hh_old, &mm_old, &ss_old, &new.hh, &new.mm, &new.ss);
        valor_old = (hh_old << 20) + (mm_old << 10) + ss_old;
        valor_new = (new.hh << 20) + (new.mm << 10) + new.ss;

        for(int i = 0; i < terminal->tam; i++) {
            // TROCAR EM TEMPO
            if(terminal->celulaTempo[i]->valor == valor_old){
                terminal->celulaTempo[i]->chegada = new;
                terminal->celulaTempo[i]->valor = valor_new;
            }
            // TROCAR EM PRIOR
            if(terminal->celulaPrior[i]->valor == valor_old){
                terminal->celulaPrior[i]->chegada = new;
                terminal->celulaPrior[i]->valor = valor_new;
            }
        }
        ordenar(terminal, false);
        terminal->bestTempo = terminal->celulaTempo[terminal->tam-1];
    }
    return;
}

/* A função printa todos os processos, com a prioridade, o horário e a descrição
*/
void printarProcessos(CELULA *celulas[], int n){
    for(int i = n-1; i >= 0; i--){
        printf("%02d %02d:%02d:%02d %s\n", celulas[i]->prior, celulas[i]->chegada.hh, celulas[i]->chegada.mm, celulas[i]->chegada.ss, celulas[i]->descricao);
    }
}

int main(void){
    char keyword[10];

    PROCESSOS terminal;
    
    terminal.tam = 0;
    terminal.tempoNaoOrdenados = 0;
    terminal.priorNaoOrdenados = 0; 

    terminal.bestPrior = NULL;
    terminal.bestTempo = NULL;
    
    do{
        scanf("%s", keyword);

        // Comando "add" O(1)
        if(strcmp(keyword, "add") == 0){
            CELULA *c1 = criarCelula(&terminal);
            inserir(c1, &terminal);
        }

        // Comando "exec"
        else if(strcmp(keyword, "exec") == 0){
            removeCelula(&terminal);
        }

        // Comando "next" O(1)
        else if(strcmp(keyword, "next") == 0){
            char key;
            scanf(" -%c", &key);
            if(key == 'p') {
                if(terminal.bestPrior != NULL) {
                    printf("%02d %02d:%02d:%02d %s\n", terminal.bestPrior->prior, terminal.bestPrior->chegada.hh, terminal.bestPrior->chegada.mm, terminal.bestPrior->chegada.ss, terminal.bestPrior->descricao);
                }
            } else if(key == 't') {
                if(terminal.bestTempo != NULL) {
                    printf("%02d %02d:%02d:%02d %s\n", terminal.bestTempo->prior, terminal.bestTempo->chegada.hh, terminal.bestTempo->chegada.mm, terminal.bestTempo->chegada.ss, terminal.bestTempo->descricao);
                }
            }
        }
        
        // Comando "change"
        else if(strcmp(keyword, "change") == 0){
            mudarCelula(&terminal);
        }
        
        // Comando "print"
        else if(strcmp(keyword, "print") == 0){
            char key;
            scanf(" -%c", &key);
            if(key == 'p') {
                ordenar(&terminal, true);
                printarProcessos(terminal.celulaPrior, terminal.tam);
            } else if(key == 't') {
                ordenar(&terminal, false);
                printarProcessos(terminal.celulaTempo, terminal.tam);
            }
        }

    } while(strcmp(keyword, "quit") != 0);

    for(int i = 0; i < terminal.tam; i++){
        free(terminal.celulaPrior[i]);
        terminal.celulaPrior[i] = NULL;
    }

    return 0;
}

/*
add 9 11:05:41 firefox
add 14 06:15:02 openoffice
add 5 05:26:18 xterm
add 7 10:44:34 emacs
add 16 05:43:21 gdb
add 11 22:47:56 garbage-collector
add 8 08:06:09 xfig
add 12 06:21:59 bash
add 13 04:11:20 nautilus
add 6 04:37:34 gnome
add 4 16:19:47 gcalc
add 17 22:40:32 x-session
add 1 07:33:25 printer-deamon
add 3 03:53:17 gimp
next -p
next -t
exec -p
exec -p
change -t 06:21:59|02:22:19
change -p 6|10
exec -t
print -p
print -t
quit
*/