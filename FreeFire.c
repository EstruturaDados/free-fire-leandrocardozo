#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// Definição da struct Item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Vetor de itens e contador
Item mochila[MAX_ITENS];
int totalItens = 0;

// Função para cadastrar um item
void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("Mochila cheia!\n");
        return;
    }
    printf("Digite o nome do item: ");
    scanf("%s", mochila[totalItens].nome);
    printf("Digite o tipo do item (arma, munição, cura): ");
    scanf("%s", mochila[totalItens].tipo);
    printf("Digite a quantidade: ");
    scanf("%d", &mochila[totalItens].quantidade);
    totalItens++;
    printf("Item cadastrado com sucesso!\n");
}

// Função para remover um item pelo nome
void removerItem() {
    if (totalItens == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    scanf("%s", nome);

    int encontrado = 0;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            // Desloca os itens restantes
            for (int j = i; j < totalItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            totalItens--;
            printf("Item removido com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Item não encontrado!\n");
    }
}

// Função para listar todos os itens
void listarItens() {
    if (totalItens == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    printf("Itens na mochila:\n");
    for (int i = 0; i < totalItens; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função de busca sequencial pelo nome
void buscarItem() {
    if (totalItens == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a buscar: ");
    scanf("%s", nome);

    int encontrado = 0;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("Item encontrado! Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Item não encontrado!\n");
    }
}

// Menu principal
int main() {
    int opcao;

    do {
        printf("\n--- Sistema de Mochila ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: buscarItem(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
