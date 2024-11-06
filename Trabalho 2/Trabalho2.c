#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> 
#define MAX_PALAVRA 100
#define MAX_LETRAS 7

typedef struct string_ {
    char *palavra;
    bool foiEncontrada;
} STRING;

typedef struct soletra_ {
    STRING *palavras[20]; 
    int palavrasTotais[20]; 
    int palavrasEncontradas[20];
} SOLETRA;

void configurarLetrasPermitidas(bool *letrasPermitidas, char *letraObrigatoria);
bool carregarPalavrasValidas(SOLETRA *soletrando, bool *letrasPermitidas, char letraObrigatoria);
bool palavraValida(const char *palavra, bool *letrasPermitidas, char letraObrigatoria);
void inicializarSoletra(SOLETRA *soletrando);
void adicionarPalavra(SOLETRA *soletrando, const char *palavra);
char *copiarString(const char *original);
int buscarPalavraBinaria(STRING *palavras, int tamanho, const char *procurada);
void liberarSoletra(SOLETRA *soletrando);

/// @brief Função para configurar as letras permitidas
/// @param letrasPermitidas 
/// @param letraObrigatoria 
void configurarLetrasPermitidas(bool *letrasPermitidas, char *letraObrigatoria) {
    scanf(" %c", letraObrigatoria);
    letrasPermitidas[*letraObrigatoria - 'A'] = true;

    for (int i = 0; i < 6; i++) {
        char a;
        scanf(" %c", &a);
        letrasPermitidas[a - 'A'] = true;
    }
}

/// @brief Função para carregar palavras válidas do arquivo
/// @param soletrando 
/// @param letrasPermitidas 
/// @param letraObrigatoria 
/// @return bool (True/False = indicando se foi possível carregá-las)
bool carregarPalavrasValidas(SOLETRA *soletrando, bool *letrasPermitidas, char letraObrigatoria) {
    FILE *arquivo = fopen("valid_words.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return false;
    }

    char palavra[MAX_PALAVRA];

    while (fgets(palavra, sizeof(palavra), arquivo)) {
        // Remove a quebra de linha
        palavra[strcspn(palavra, "\n")] = '\0'; 

        if (palavraValida(palavra, letrasPermitidas, letraObrigatoria)) {
            adicionarPalavra(soletrando, palavra);
        }
    }

    fclose(arquivo);
    return true;
}


/// @brief Função que verifica se a palavra é valida de acordo com as letras propostas pelo usuário
/// @param palavra 
/// @param letrasPermitidas 
/// @param letraObrigatoria 
/// @return bool (True/False = indicando se a palavra segue as regras)
bool palavraValida(const char *palavra, bool *letrasPermitidas, char letraObrigatoria) {
    bool encontrouObrigatoria = false;

    for (int i = 0; palavra[i] != '\0'; i++) {
        if(palavra[i] == letraObrigatoria)
            encontrouObrigatoria = true;

        if(letrasPermitidas[palavra[i] - 'A'] == false)
            return false;
    }

    return encontrouObrigatoria;
}

/// @brief Inicializa os campos da estrutura SOLETRA
/// @param soletrando 
void inicializarSoletra(SOLETRA *soletrando) {
    for (int i = 0; i < 20; i++) {
        soletrando->palavras[i] = NULL;
        soletrando->palavrasTotais[i] = 0;
        soletrando->palavrasEncontradas[i] = 0;
    }
}


/// @brief Função auxiliar da função de adicionar palavras, usada para copiar a string manualmente
/// @param original 
/// @return CHAR * (Retorna uma cópia da palavra alocada dinamicamente)
char *copiarString(const char *original) {
    char *copia = malloc(strlen(original) + 1);
    if (copia != NULL) {
        strcpy(copia, original);
    }
    return copia;
}


/// @brief Função que adiciona uma palavra nova à estrutura SOLETRA
/// @param soletrando 
/// @param nova 
void adicionarPalavra(SOLETRA *soletrando, const char *nova) {
    int tamanho = strlen(nova);
    // Verifica se o tamanho está no intervalo de 1 a 19
    if (tamanho > 0 && tamanho < 20) { 
        int indice = tamanho - 1;

        // Aloca memória para palavras no índice, se necessário
        if (soletrando->palavras[indice] == NULL) {
            soletrando->palavras[indice] = malloc(MAX_PALAVRA * sizeof(STRING));
            if (soletrando->palavras[indice] == NULL) {
                printf("ERRO NA ALOCAÇÃO");
                exit(1);
            }
        }
        // Verifica a posição adequada de inserção
        int posicao = soletrando->palavrasTotais[indice];
        // Insere uma palavra na struct na posicao, de acordo com o tamanho da palavra
        soletrando->palavras[indice][posicao].palavra = copiarString(nova);
        // Inicializando o booleano da palavra como falso para posterior busca e modificação
        soletrando->palavras[indice][posicao].foiEncontrada = false;
        // Incrementando a quantidade de palavras daquele tamanho
        soletrando->palavrasTotais[indice]++;
    }
}


/// @brief Função que realiza a busca binária, por meio do strcmp, para encontrar a palavra procurada
/// @param palavras 
/// @param tamanho 
/// @param procurada 
/// @return int (O inteiro do índice em que a palavra se encontra)
int buscarPalavraBinaria(STRING *palavras, int tamanho, const char *procurada) {
    int esquerda = 0, direita = tamanho - 1;
    while (esquerda < direita) {
        int meio = (esquerda + direita) / 2;
        int cmp = strcmp(palavras[meio].palavra, procurada);

        if (cmp == 0) {
            // Palavra encontrada
            return meio; 
        } else if (cmp < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio;
        }
    }
     // Palavra não encontrada
    return -1;
}

/// @brief Função que desaloca toda a memória alocada na struct SOLETRA
/// @param soletrando 
void liberarSoletra(SOLETRA *soletrando) {
    for (int i = 0; i < 20; i++) {
        if (soletrando->palavras[i] != NULL) {
            for (int j = 0; j < soletrando->palavrasTotais[i]; j++) {
                // Libera cada string
                free(soletrando->palavras[i][j].palavra);  
            }
            // Libera o array de `STRING`
            free(soletrando->palavras[i]);  
        }
    }
}

int main() {
    bool letrasPermitidas[26] = {};
    char letraObrigatoria;
    SOLETRA soletrando;
    inicializarSoletra(&soletrando);

    while(1) {
        char keyword[15];
        scanf("%s", keyword); 

        if(strcmp(keyword, "inicio") == 0) {
            configurarLetrasPermitidas(letrasPermitidas, &letraObrigatoria);

            if (!carregarPalavrasValidas(&soletrando, letrasPermitidas, letraObrigatoria)) {
                return EXIT_FAILURE;
            }
        }
        else if(strcmp(keyword, "palavra") == 0){
            char tentativa[MAX_PALAVRA];
            scanf("%s", tentativa);
            
            // Calcula o tamanho da palavra tentativa e define o índice apropriado
            int indiceArray = strlen(tentativa) - 1;

            // Busca pela palavra no array específico do tamanho
            int indice = buscarPalavraBinaria(soletrando.palavras[indiceArray], soletrando.palavrasTotais[indiceArray], tentativa);

            if (indice == -1) {
                printf("palavra invalida\n");
            } else {
                if(soletrando.palavras[indiceArray][indice].foiEncontrada == false) {
                    printf("sucesso +1\n");
                    // Atualiza o status de 'foiEncontrada' da palavra encontrada
                    soletrando.palavras[indiceArray][indice].foiEncontrada = true;
                    // Incrementa o contador de palavras encontradas no índice apropriado, referente ao tamanho
                    soletrando.palavrasEncontradas[indiceArray]++;
                    
                    bool acabou = true;
                    for(int i = 0; i < 20; i++) {
                        if(soletrando.palavrasEncontradas[i] != soletrando.palavrasTotais[i]) {
                            acabou = false;
                            break;
                        }
                    }
                    if(acabou) {
                        printf("parabens! voce encontrou todas as palavras\n");
                        break;
                    }
                } else {
                    printf("palavra já encontrada\n");
                }
            }
        }
        else if(strcmp(keyword, "progresso") == 0){
            printf("progresso atual:\n");
            for (int i = 0; i < 20; i++) {
                // A partir dos dados guardados na struct, vai apontando a quantidade de palavras (encontradas e totais), desde que haja alguma palavra daquele tamanho
                if(soletrando.palavrasTotais[i] != 0) {
                    printf("(%d letras) %d palavra(s) encontrada(s) / %d palavra(s) faltando\n", i+1, soletrando.palavrasEncontradas[i], soletrando.palavrasTotais[i]-soletrando.palavrasEncontradas[i]);
                }
            }
        }

        else if(strcmp(keyword, "solucao") == 0){
            printf("para encerrar o jogo estavam faltando as palavras:\n");
            for (int i = 0; i < 20; i++) {
                if(soletrando.palavrasTotais[i] != 0) {
                    bool primeiro = true;
                    if(soletrando.palavrasEncontradas[i] == soletrando.palavrasTotais[i]) {
                        continue;
                    }
                    printf("(%d letras) ", i+1);
                    for (int j = 0; j < soletrando.palavrasTotais[i]; j++) {
                        if(!soletrando.palavras[i][j].foiEncontrada) {
                            if (!primeiro) {
                                printf(", ");
                            }
                            printf("%s", soletrando.palavras[i][j].palavra);
                            primeiro = false;
                        }
                    }
                    printf("\n");
                }
            }
            break;
        }
    }
    liberarSoletra(&soletrando);

    return EXIT_SUCCESS;
}
