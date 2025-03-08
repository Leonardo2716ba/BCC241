#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gerar_arquivo() {
    // Nome do arquivo
    char nome_arquivo[] = "mochila.txt";
    
    // Peso da mochila
    int peso_mochila = 100;
    
    // Abre o arquivo para escrita
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    // Escreve o peso da mochila na primeira linha
    fprintf(arquivo, "%d\n", peso_mochila);
    
    // Gera 409600 itens
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    for (int i = 0; i < 409600; i++) {
        int peso = rand() % 30 + 1; // Gera um peso entre 1 e 30
        int valor = rand() % 100 + 1; // Gera um valor entre 1 e 100
        fprintf(arquivo, "%d\t%d\n", peso, valor);
    }
    
    // Fecha o arquivo
    fclose(arquivo);
}

int main() {
    gerar_arquivo();
    printf("Arquivo gerado com sucesso!\n");
    return 0;
}