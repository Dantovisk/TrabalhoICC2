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
typedef struct {
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
typedef struct {
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
typedef struct {
    int tam;
    CELULA *celulaPrior [MAXN];
    CELULA *celulaTempo [MAXN];
} PROCESSOS;

/* A função realiza a cópia das informações de uma célula para outra */
void copiaCelula(CELULA *origem, CELULA *destino) {
    if(origem != NULL && destino!=NULL) {
        strcpy(destino->descricao, origem->descricao);
        destino->prior = origem->prior;
        destino->valor = origem->valor;
        destino->chegada.hh = origem->chegada.hh;
        destino->chegada.mm = origem->chegada.mm;
        destino->chegada.ss = origem->chegada.ss;
    }
}
/* A função inicializa as células com valores absurdos para ser possível*/
void inicializaBestCelula(CELULA *celula) {
    celula->prior=-1;
    celula->valor=INF;
    memset(celula->descricao, 0, sizeof(celula->descricao));
    celula->chegada = (HORARIO) {INF, INF, INF};
    return;
}

/* A função faz a alocação dinâmica da memória utilizada para a struct CELULA, já recebendo do usuário os valores que serão
inicializados nos campos da struct, sejam eles, a prioridade, o horário e fazendo a conversão do valor.
*/
CELULA *criarCelula(CELULA *bestPrior, CELULA *bestTempo){
    CELULA *c = (CELULA *) malloc(sizeof(CELULA));
    if(c != NULL){
        scanf("%d %d:%d:%d %s", &c->prior, &c->chegada.hh, &c->chegada.mm, &c->chegada.ss, c->descricao);
        c->valor = (c->chegada.hh << 20) + (c->chegada.mm << 10) + c->chegada.ss;
        if(c->prior > bestPrior->prior) {
            copiaCelula(c, bestPrior);
        }
        if(c->valor < bestTempo->valor) {
            copiaCelula(c, bestTempo);
        }
    }
    return c;
}

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

/* A função faz a inserção da célula, sem se importar com ordenação, apenas se insere no próximo espaço vago
*/
void inserir(CELULA *c1, PROCESSOS *terminal){
    if(terminal->tam < MAXN){
        int i;
        terminal->celulaTempo[terminal->tam] = c1;
        terminal->celulaPrior[terminal->tam] = c1;
        terminal->tam++;
    }

    return; 
}

void removeCelula(PROCESSOS *terminal, CELULA *bestPrior, CELULA *bestTempo){
    char key;
    scanf(" -%c", &key);
    if (terminal->tam == 0) return;


    if (key == 'p') {
        // ENCONTRAR A CÉLULA COM A MAIOR PRIORIDADE
        int segundomelhor=-1;
        int indice = 0;
        for (int i = 0; i < terminal->tam; i++) {
            if (terminal->celulaPrior[i]->prior == bestPrior->prior) {
                free(terminal->celulaPrior[i]);
                terminal->celulaPrior[i] = NULL;
                for (int j = i; j < terminal->tam-1; j++) {
                    terminal->celulaPrior[j] = terminal->celulaPrior[j + 1];
                }
            }
            if (terminal->celulaTempo[i]->prior == bestPrior->prior) {
                free(terminal->celulaTempo[i]);
                terminal->celulaTempo[i] = NULL;
                for (int j = i; j < terminal->tam-1; j++) {
                    terminal->celulaTempo[j] = terminal->celulaTempo[j + 1];
                }
            } else if (segundomelhor < terminal->celulaTempo[i]->prior) {
                segundomelhor = terminal->celulaTempo[i]->prior;
                indice = i;
            }
        }
        if (terminal->tam > 0) {
            copiaCelula(terminal->celulaTempo[indice], bestPrior);
        } else {
            inicializaBestCelula(bestPrior);
        }

    } else if (key == 't') {
        // ENCONTRAR A CÉLULA COM O MENOR TEMPO
        int segundomelhor=INF;
        int indice = 0;
        for (int i = 0; i < terminal->tam; i++) {
            if (terminal->celulaTempo[i]->valor == bestTempo->valor) {
                free(terminal->celulaTempo[i]);
                terminal->celulaTempo[i] = NULL;
                for (int j = i; j < terminal->tam-1; j++) {
                    terminal->celulaTempo[j] = terminal->celulaTempo[j + 1];
                }
            }
            if (terminal->celulaPrior[i]->valor == bestTempo->valor) {
                free(terminal->celulaPrior[i]);
                terminal->celulaPrior[i] = NULL;
                for (int j = i; j < terminal->tam-1; j++) {
                    terminal->celulaPrior[j] = terminal->celulaPrior[j + 1];
                }
            } else if (segundomelhor > terminal->celulaPrior[i]->valor) {
                segundomelhor = terminal->celulaPrior[i]->valor;
                indice = i;
            }
        }
        if (terminal->tam > 0) {
            copiaCelula(terminal->celulaPrior[indice], bestTempo);
        } else {
            inicializaBestCelula(bestTempo);
        }
    }
    terminal->tam--;
}



/* A função faz a mudança das informações da célula e, caso ela seja melhor que as outras em tempo e/ou prioridade, troca a vigente
*/
void mudarCelula(PROCESSOS *terminal, CELULA *bestPrior, CELULA *bestTempo){
    char key;
    scanf(" -%c", &key);

    bool melhor = false;
    int i;

    if(key == 'p'){

        int prior_old, prior_new;
        scanf("%d|%d", &prior_old, &prior_new);

        // VERIFICA SE A MUDANÇA DEVE CAUSAR ALTERAÇÃO DO MELHOR
        if(prior_new > bestPrior->prior) {
            melhor = true;
        }

        for(i = 0; i < terminal->tam; i++){
            // TROCAR EM PRIOR
            if(terminal->celulaPrior[i]->prior == prior_old){
                terminal->celulaPrior[i]->prior = prior_new;
                // TROCAR A MELHOR VIGENTE EM PRIOR PELA NOVA MELHOR
                if(melhor == true) {
                    copiaCelula(terminal->celulaPrior[i], bestPrior);
                }
            }
            // TROCAR EM TEMPO
            if(terminal->celulaTempo[i]->prior == prior_old){
                terminal->celulaTempo[i]->prior = prior_new;
            }
        }
    } else if(key == 't'){

        int hh_old, mm_old, ss_old, valor_new, valor_old;
        HORARIO new;
        scanf("%d:%d:%d|%d:%d:%d", &hh_old, &mm_old, &ss_old, &new.hh, &new.mm, &new.ss);
        valor_old = (hh_old << 20) + (mm_old << 10) + ss_old;
        valor_new = (new.hh << 20) + (new.mm << 10) + new.ss;

        // VERIFICA SE A MUDANÇA DEVE CAUSAR ALTERAÇÃO DO MELHOR
        if(valor_new < valor_old) {
            melhor = true;
        } 

        for(i = 0; i < terminal->tam; i++) {
            // TROCAR EM TEMPO
            if(terminal->celulaTempo[i]->valor == valor_old){
                terminal->celulaTempo[i]->chegada = new;
                terminal->celulaTempo[i]->valor = valor_new;
                // TROCAR A MELHOR VIGENTE EM TEMPO PELA NOVA MELHOR
                if(melhor == true) {
                    copiaCelula(terminal->celulaTempo[i], bestTempo);
                }
            }
            // TROCAR EM PRIOR
            if(terminal->celulaPrior[i]->valor == valor_old){
                terminal->celulaPrior[i]->chegada = new;
                terminal->celulaPrior[i]->valor = valor_new;
            }
        }
    }
    return;
}

/* A função printa todos os processos, com a prioridade, o horário e a descrição
*/
void printarProcessos(CELULA *celulas[], int n){
    for(int i = n-1; i >= 0; i--){
        printf("%02d %02d:%02d:%02d %s\n", celulas[i]->prior, celulas[i]->chegada.hh, celulas[i]->chegada.mm, celulas[i]->chegada.ss, celulas[i]->descricao);
    }
    printf("\n");
    
}

int main(){
    char keyword[10];

    PROCESSOS terminal;
    terminal.tam = 0;

    CELULA bestPrior;
    CELULA bestTempo;
    inicializaBestCelula(&bestPrior);
    inicializaBestCelula(&bestTempo);
    
    do{
        scanf("%s", keyword);

        // Comando "add".
        if(strcmp(keyword, "add") == 0){
            CELULA *c1 = criarCelula(&bestPrior, &bestTempo);
            inserir(c1, &terminal);
        }

        // Comando "exec"
        else if(strcmp(keyword, "exec") == 0){
            removeCelula(&terminal, &bestPrior, &bestTempo);
        }

        // Comando "next"
        else if(strcmp(keyword, "next") == 0){
            char key;
            scanf(" -%c", &key);
            if(key == 'p') {
                if(bestPrior.prior != -1) {
                    printf("%02d %02d:%02d:%02d %s\n", bestPrior.prior, bestPrior.chegada.hh, bestPrior.chegada.mm, bestPrior.chegada.ss, bestPrior.descricao);
                }
            } else if(key == 't') {
                if(bestTempo.valor != INF) {
                    printf("%02d %02d:%02d:%02d %s\n", bestTempo.prior, bestTempo.chegada.hh, bestTempo.chegada.mm, bestTempo.chegada.ss, bestTempo.descricao);
                }
            }
        }
        
        // Comando "change"
        else if(strcmp(keyword, "change") == 0){
            mudarCelula(&terminal, &bestPrior, &bestTempo);
        }
        
        // Comando "print"
        else if(strcmp(keyword, "print") == 0){
            char key;
            scanf(" -%c", &key);
            if(key == 'p') {
                quickSort(terminal.celulaPrior, 0, terminal.tam-1, true);
                printarProcessos(terminal.celulaPrior, terminal.tam);
            } else if(key == 't') {
                quickSort(terminal.celulaTempo, 0, terminal.tam-1, false);
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