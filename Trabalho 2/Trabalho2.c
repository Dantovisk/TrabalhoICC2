#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PALAVRA 100
#define MAX_LETRAS 7

typedef struct string_{
    char *palavra;
    bool foiEncontrada;
} STRING;

typedef struct soletra_{
    // cada indice dos arrays a seguir representa o numero de letras a que
    // se referem
    STRING *palavras[20]; //matriz de strings
    int palavrasTotais[20]; //tamanhos dos arrays de strings
    int palavrasEncotradas[20];
} SOLETRA;

// Retorna true se a palavra seguir as regras
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

int main() {
    bool letrasPermitidas[26] = {};
    char letraObrigatoria = 'P';

    while(1){
        char keyword[15];
        scanf("%s", keyword); 

        if(strcmp(keyword, "inicio") == 0){
            scanf(" %c", &letraObrigatoria);
            letrasPermitidas[letraObrigatoria - 'A'] = true;

            for(int i = 0; i<6; i++){
                char a;
                scanf(" %c", &a);
                letrasPermitidas[a - 'A'] = true;
            }

            FILE *arquivo = fopen("valid_words.txt", "r");
            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo.\n");
                return 1;
            }

            char palavra[MAX_PALAVRA];
            printf("Palavras que seguem a regra:\n");
            
            // Lê cada linha do arquivo para verificar se a palavra será valida
            while (fgets(palavra, sizeof(palavra), arquivo)) {
                // remove a quebra de linha
                palavra[strcspn(palavra, "\n")] = '\0';

                if (palavraValida(palavra, letrasPermitidas, letraObrigatoria)) {
                    printf("%s\n", palavra);
                }
            }

            fclose(arquivo);
        }
        else if(strcmp(keyword, "palavra") == 0){

        }
        else if(strcmp(keyword, "progresso") == 0){
        }

        else if(strcmp(keyword, "solucao") == 0){
        }
    }
    

    

    return 0;
}
