#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DESCR 100
#define MAXN 1000

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
priorNaoOrdenados: número que acumula a quantidade de elementos adicionados em celulaPrior, sem ter dado sort
tempoNaoOrdenados: número que acumula a quantidade de elementos adicionados em celulaTempo, sem ter dado sort
celulaPrior: conjunto de células com olhar para a prioridade
celulaTempo: conjunto de células com olhar para o tempo 
bestPrior: ponteiro para a maior prioridade
bestTempo: ponteiro para o menor tempo
*/
typedef struct processos_ {
    int tam;
    int priorNaoOrdenados; 
    int tempoNaoOrdenados; 
    CELULA *celulaPrior [MAXN];
    CELULA *celulaTempo [MAXN];
    CELULA *bestPrior;
    CELULA *bestTempo;
} PROCESSOS;

/* A função faz a alocação dinâmica da memória utilizada para a struct CELULA, já recebendo do usuário os valores que serão
inicializados nos campos da struct, sejam eles: a prioridade, o horário, a descrição. Além disso, faz-se a conversão do valor.
Essa conversão faz que as horas fiquem isoladas, numericamente, dos minutos e estes, dos segundos.
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

/* A função faz um Insertionsort iterativamente, de tal modo que torna o vetor ordenado em O(n^2) */
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

/* Sabendo que o pior caso do Quicksort é aquele para o qual o vetor está ordenado, busca-se utilizá-lo somente para quando 
o número de mudanças, sem ter ocorrido um sort, é mais significativo. Para casos com poucas alterações para a ordenação, opta-se
por realizar um InsertionSort, que mesmo sendo O(n^2), para poucas mudanças, deve se sair melhor que o QuickSort. Assim, a partir
das variáveis 'priorNaoOrdenados' e 'tempoNaoOrdenados' define-se qual dos dois usar. 
*/
void ordenar(PROCESSOS *terminal, bool isPrior){
    int x;
    // x recebe um valor indicando se o vetor está muito ou pouco desordenado
    if (isPrior) {
        x = terminal->priorNaoOrdenados;
        terminal->priorNaoOrdenados = 0;
    } else {
        x = terminal->tempoNaoOrdenados;
        terminal->tempoNaoOrdenados = 0;
    } 

    // Quando o número de elementos que não estão necessariamente ordenados for pequeno, realiza-se insertion sort
    if (x <= 5){
        insertionSort((isPrior ? terminal->celulaPrior :terminal->celulaTempo), terminal->tam, isPrior); 
    } else {
        quickSort((isPrior ? terminal->celulaPrior :terminal->celulaTempo), 0, terminal->tam-1, isPrior);
    }

}

/* A função faz a inserção da célula, sem se importar com ordenação, apenas se insere no próximo espaço vago de ambas, Prior e Tempo
A função tem complexidade O(1), pois não é uma preocupação ordenar o vetor.
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
/* A célula com maior prioridade/menor tempo, a depender do input do usuário, é removida da lista de processos. Para tanto,
ordena-se o vetor da respectiva categoria e, no outro, faz-se deslocamentos. Como a ordenação do vetor de células é a operação
mais significativa operacionalmente, a complexidade fica de acordo com o sort escolhido, em O(n^2) ou O(n log n).
*/
void removeCelula(PROCESSOS *terminal){
    char key;
    scanf(" -%c", &key);

    if (terminal->tam == 0) return;

    if (key == 'p') {
        // ORDENA O VETOR DE PRIORIDADE, RECEBENDO O ÚLTIMO ELEMENTO, QUE É O DE MAIOR PRIORIDADE
        ordenar(terminal, true);
        CELULA *aux = terminal->celulaPrior[terminal->tam-1];
        // COMO SOMENTE O VETOR DE PRIORIDADE FICOU ORDENADO, NO VETOR DE CÉLULAS DE TEMPO, REALIZA-SE O SHIFT(DESLOCAMENTO)
        for(int i=0; i < terminal->tam; i++) {
            if(terminal->celulaTempo[i]->prior == aux->prior) {
                for(int j=i; j < terminal->tam-1; j++) {
                    terminal->celulaTempo[j] = terminal->celulaTempo[j+1];
                }
                break;
            }
        }
        // A MELHOR PRIORIDADE PASSA A SER A SEGUNDA MAIOR PRIODADE
        terminal->bestPrior = terminal->celulaPrior[terminal->tam-2];
        // É EXECUTADO O PROCESSO DE MAIOR PRIORIDADE E A MEMÓRIA PARA ELE ALOCADA É LIBERADA
        free(aux);
        aux = NULL;
    } else if (key == 't') {
        // ORDENA O VETOR DE TEMPO, RECEBENDO O ÚLTIMO ELEMENTO, QUE É O DE MENOR TEMPO
        ordenar(terminal, false);
        CELULA *aux = terminal->celulaTempo[terminal->tam-1];
        // COMO SOMENTE O VETOR DE TEMPO FICOU ORDENADO, NO VETOR DE CÉLULAS DE PRIORIDADE, REALIZA-SE O SHIFT(DESLOCAMENTO)
        for(int i=0; i < terminal->tam; i++) {
            if(terminal->celulaPrior[i]->valor == aux->valor) {
                for(int j = i; j < terminal->tam-1; j++) {
                    terminal->celulaPrior[j] = terminal->celulaPrior[j+1];
                }
            }
        }
         // A MENOR TEMPO PASSA A SER O SEGUNDO MENOR TEMPO
        terminal->bestTempo = terminal->celulaTempo[terminal->tam-2];
         // É EXECUTADO O PROCESSO DE MENOR TEMPO E A MEMÓRIA PARA ELE ALOCADA É LIBERADA
        free(aux);
        aux = NULL;
    }
    // É DECREMENTADO O TAMANHO DA LISTA DE PROCESSOS E, CASO CHEGUE A ZERO, O 'BESTPRIOR' E O 'BESTTEMPO' NÃO EXISTEM MAIS
    terminal->tam--;
    if(terminal->tam == 0){
        terminal->bestTempo = NULL;
        terminal->bestPrior = NULL;
    }
    return;
}



/* A função faz a mudança da informação de uma célula e, feito isso, é realizada uma ordenação do vetor.
A maior prioridade/o menor tempo é atualizado sempre para o valor que constar no último elemento do vetor,
depois que ele já foi ordenado. A complexidade é norteada principalmente pelo tipo de sort optado (QuickSort
(n log n) ou InsertionSort(n^2)), já que para além disso, o shift dos elementos do vetor é realizado em O(n).
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
        // ORDENA E DEFINE O MELHOR
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
        // ORDENA E DEFINE O MELHOR
        ordenar(terminal, false);
        terminal->bestTempo = terminal->celulaTempo[terminal->tam-1];
    }
    return;
}

/* A função printa todos os processos, com a prioridade, o horário e a descrição, o vetor é percorrido do último índice ao primeiro
*/
void printarProcessos(CELULA *celulas[], int n){
    for(int i = n-1; i >= 0; i--){
        printf("%02d %02d:%02d:%02d %s\n", celulas[i]->prior, celulas[i]->chegada.hh, celulas[i]->chegada.mm, celulas[i]->chegada.ss, celulas[i]->descricao);
    }
    if(n != 0) {    
        printf("\n");
    }
}

int main(void){
    char keyword[10];

    PROCESSOS terminal;
    
    // INICIALIZAÇÃO DE CAMPOS DA STRUCT TERMINAL
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

        // Comando "exec" (COMPLEXIDADE VARIÁVEL, SEGUNDO O TIPO DE SORT)
        else if(strcmp(keyword, "exec") == 0){
            removeCelula(&terminal);
        }

        // Comando "next" O(1) 
        else if(strcmp(keyword, "next") == 0){
            char key;
            scanf(" -%c", &key);
            // PRINTA A RESPECTIVA VARIÁVEL 'BEST____', DE ACORDO COM O INPUT
            if(key == 'p') {
                if(terminal.bestPrior != NULL) {
                    printf("%02d %02d:%02d:%02d %s\n", terminal.bestPrior->prior, terminal.bestPrior->chegada.hh, terminal.bestPrior->chegada.mm, terminal.bestPrior->chegada.ss, terminal.bestPrior->descricao);
                    printf("\n");
                }
            } else if(key == 't') {
                if(terminal.bestTempo != NULL) {
                    printf("%02d %02d:%02d:%02d %s\n", terminal.bestTempo->prior, terminal.bestTempo->chegada.hh, terminal.bestTempo->chegada.mm, terminal.bestTempo->chegada.ss, terminal.bestTempo->descricao);
                    printf("\n");
                }
            }
        }
        
        // Comando "change" (COMPLEXIDADE VARIÁVEL, SEGUNDO O TIPO DE SORT)
        else if(strcmp(keyword, "change") == 0){
            mudarCelula(&terminal);
        }
        
        // Comando "print" (COMPLEXIDADE VARIÁVEL, SEGUNDO O TIPO DE SORT)
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

    // LIBERA TODA A MEMÓRIA ALOCADA DAS CÉLULAS
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