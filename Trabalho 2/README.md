Optou-se pela utilização das seguintes estruturas baseadas em listas sequenciais / vetores:
- STRING que armazena uma palavra(vetor de chars) e um booleano que indica se a palavra foi encontrada durante o jogo.
- SOLETRA, armazena uma matriz de STRINGs baseado no número de letras, junto com a quantidade total, encontrada e
capacidade de alocação de memória. É utilizada tal matriz para que facilite a busca sabendo o tamanho das palavras.

A inicialização do código é baseado na verificação das palavras que são válidas para o jogo, a partir das letras permitidas,
de dentro do dicionário e armazená-las dentro da estrutura SOLETRA. É utilizado mallocs e reallocs que podem ser ajustados
pelos define MEM_INICIAL e MEM_REALLOC para uso eficiente de memória. 

Durante o jogo, a busca binária é usada para achar as palavras no vetor de STRING do tamanho correspondente e verifica se
a palavra existe na estrutura SOLETRA. A lógica para saber se o jogo acabou consiste em comparar as palavras totais e as encontradas.
O progresso e a solução verifica se as palavras com um determinado número de letras existe no SOLETRA e printa.