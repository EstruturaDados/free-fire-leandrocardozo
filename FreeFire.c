/*
  Mochila - comparacao Vetor x Lista Encadeada
  - Nível: iniciante / aventureiro (simples e direto)
  - Implementa:
    * Vetor (capacidade MAX_ITENS) com inserir/remover/listar/busca sequencial
    * Ordenacao do vetor (selection sort) e busca binaria (após ordenar)
    * Lista encadeada com inserir/remover/listar/busca sequencial
    * Contadores de comparacoes para buscas (sequencial e binaria)
  - Observação: usa scanf("%29s") para strings (sem espaços), para manter o código simples
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

/* ---------------------- Structs ---------------------- */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

/* Nó para lista encadeada */
typedef struct No {
    Item dados;
    struct No *proximo;
} No;

/* ---------------------- Globais (contadores) ---------------------- */
int comps_seq_vetor = 0;   /* comparações na busca sequencial do vetor */
int comps_bin_vetor = 0;   /* comparações na busca binária do vetor */
int comps_seq_lista = 0;   /* comparações na busca sequencial da lista */

/* ---------------------- Funções do VETOR ---------------------- */

/* Inserir no vetor (no final) */
void inserirVetor(Item vetor[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("Vetor cheio! Nao e possivel inserir mais itens.\n");
        return;
    }
    printf("Nome: ");
    scanf("%29s", vetor[*total].nome);
    printf("Tipo: ");
    scanf("%19s", vetor[*total].tipo);
    printf("Quantidade: ");
    scanf("%d", &vetor[*total].quantidade);
    (*total)++;
    printf("Item inserido no vetor.\n");
}

/* Remover do vetor por nome (desloca elementos) */
void removerVetor(Item vetor[], int *total) {
    if (*total == 0) {
        printf("Vetor vazio!\n");
        return;
    }
    char nome[MAX_NOME];
    printf("Nome do item para remover: ");
    scanf("%29s", nome);

    int idx = -1;
    for (int i = 0; i < *total; i++) {
        if (strcmp(vetor[i].nome, nome) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Item nao encontrado no vetor.\n");
        return;
    }
    for (int j = idx; j < *total - 1; j++) {
        vetor[j] = vetor[j+1];
    }
    (*total)--;
    printf("Item removido do vetor.\n");
}

/* Listar itens do vetor */
void listarVetor(const Item vetor[], int total) {
    if (total == 0) {
        printf("Vetor vazio.\n");
        return;
    }
    printf("Itens no vetor:\n");
    for (int i = 0; i < total; i++) {
        printf("%d) %s | %s | %d\n", i+1, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
}

/* Busca sequencial no vetor (retorna índice ou -1) */
int buscarSequencialVetor(const Item vetor[], int total, const char *nome) {
    comps_seq_vetor = 0;
    for (int i = 0; i < total; i++) {
        comps_seq_vetor++;
        if (strcmp(vetor[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Ordenação simples (Selection Sort) por nome */
void ordenarVetor(Item vetor[], int total) {
    for (int i = 0; i < total - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < total; j++) {
            if (strcmp(vetor[j].nome, vetor[menor].nome) < 0) {
                menor = j;
            }
        }
        if (menor != i) {
            Item tmp = vetor[i];
            vetor[i] = vetor[menor];
            vetor[menor] = tmp;
        }
    }
    printf("Vetor ordenado por nome.\n");
}

/* Busca binária no vetor (assume vetor ordenado). Retorna índice ou -1 */
int buscarBinariaVetor(const Item vetor[], int total, const char *nome) {
    comps_bin_vetor = 0;
    int lo = 0, hi = total - 1;
    while (lo <= hi) {
        comps_bin_vetor++;
        int mid = (lo + hi) / 2;
        int cmp = strcmp(nome, vetor[mid].nome);
        if (cmp == 0) return mid;
        if (cmp < 0) hi = mid - 1;
        else lo = mid + 1;
    }
    return -1;
}

/* ---------------------- Funções da LISTA ENC ADEADA ---------------------- */

/* Inserir no final da lista */
void inserirLista(No **cabeca) {
    No *novo = (No*) malloc(sizeof(No));
    if (!novo) {
        printf("Erro de alocacao.\n");
        return;
    }
    printf("Nome: ");
    scanf("%29s", novo->dados.nome);
    printf("Tipo: ");
    scanf("%19s", novo->dados.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    novo->proximo = NULL;

    if (*cabeca == NULL) {
        *cabeca = novo;
    } else {
        No *p = *cabeca;
        while (p->proximo) p = p->proximo;
        p->proximo = novo;
    }
    printf("Item inserido na lista encadeada.\n");
}

/* Remover por nome na lista (retorna 1 se removido) */
void removerLista(No **cabeca) {
    if (*cabeca == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    char nome[MAX_NOME];
    printf("Nome do item para remover: ");
    scanf("%29s", nome);

    No *p = *cabeca, *ant = NULL;
    while (p) {
        if (strcmp(p->dados.nome, nome) == 0) break;
        ant = p;
        p = p->proximo;
    }
    if (!p) {
        printf("Item nao encontrado na lista.\n");
        return;
    }
    if (ant == NULL) {
        *cabeca = p->proximo;
    } else {
        ant->proximo = p->proximo;
    }
    free(p);
    printf("Item removido da lista.\n");
}

/* Listar elementos da lista */
void listarLista(const No *cabeca) {
    if (!cabeca) {
        printf("Lista vazia.\n");
        return;
    }
    printf("Itens na lista encadeada:\n");
    int i = 1;
    for (const No *p = cabeca; p; p = p->proximo) {
        printf("%d) %s | %s | %d\n", i++, p->dados.nome, p->dados.tipo, p->dados.quantidade);
    }
}

/* Busca sequencial na lista (retorna ponteiro para nó ou NULL) */
No* buscarSequencialLista(No *cabeca, const char *nome) {
    comps_seq_lista = 0;
    for (No *p = cabeca; p; p = p->proximo) {
        comps_seq_lista++;
        if (strcmp(p->dados.nome, nome) == 0) return p;
    }
    return NULL;
}

/* Liberar toda a lista */
void liberarLista(No **cabeca) {
    No *p = *cabeca;
    while (p) {
        No *tmp = p;
        p = p->proximo;
        free(tmp);
    }
    *cabeca = NULL;
}

/* ---------------------- Menu e integração ---------------------- */

void menuVetor(Item vetor[], int *total) {
    int opc;
    char nome[MAX_NOME];
    do {
        printf("\n--- Menu: Vetor ---\n");
        printf("1 Inserir  2 Remover  3 Listar  4 Buscar sequencial  5 Ordenar  6 Buscar binaria  0 Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: inserirVetor(vetor, total); break;
            case 2: removerVetor(vetor, total); break;
            case 3: listarVetor(vetor, *total); break;
            case 4:
                printf("Nome para busca sequencial: ");
                scanf("%29s", nome);
                {
                    int idx = buscarSequencialVetor(vetor, *total, nome);
                    if (idx >= 0) printf("Encontrado no indice %d (%s) | comparacoes=%d\n", idx, vetor[idx].nome, comps_seq_vetor);
                    else printf("Nao encontrado | comparacoes=%d\n", comps_seq_vetor);
                }
                break;
            case 5:
                ordenarVetor(vetor, *total);
                break;
            case 6:
                printf("Obs: execute ordenacao antes para garantir busca binaria.\n");
                printf("Nome para busca binaria: ");
                scanf("%29s", nome);
                {
                    int idx = buscarBinariaVetor(vetor, *total, nome);
                    if (idx >= 0) printf("Encontrado no indice %d (%s) | comparacoes(binaria)=%d\n", idx, vetor[idx].nome, comps_bin_vetor);
                    else printf("Nao encontrado | comparacoes(binaria)=%d\n", comps_bin_vetor);
                }
                break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opc != 0);
}

void menuLista(No **cabeca) {
    int opc;
    char nome[MAX_NOME];
    do {
        printf("\n--- Menu: Lista Encadeada ---\n");
        printf("1 Inserir  2 Remover  3 Listar  4 Buscar sequencial  0 Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: inserirLista(cabeca); break;
            case 2: removerLista(cabeca); break;
            case 3: listarLista(*cabeca); break;
            case 4:
                printf("Nome para busca sequencial: ");
                scanf("%29s", nome);
                {
                    No *res = buscarSequencialLista(*cabeca, nome);
                    if (res) printf("Encontrado: %s | %s | %d | comparacoes=%d\n", res->dados.nome, res->dados.tipo, res->dados.quantidade, comps_seq_lista);
                    else printf("Nao encontrado | comparacoes=%d\n", comps_seq_lista);
                }
                break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opc != 0);
}

/* Programa principal: escolhe modo (vetor x lista) e roda menus */
int main(void) {
    Item vetor[MAX_ITENS];
    int totalVetor = 0;
    No *cabeca = NULL;

    int escolha;
    do {
        printf("\n=== Mochila: Vetor x Lista Encadeada ===\n");
        printf("1 - Trabalhar com Vetor (lista sequencial)\n");
        printf("2 - Trabalhar com Lista Encadeada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1: menuVetor(vetor, &totalVetor); break;
            case 2: menuLista(&cabeca); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (escolha != 0);

    /* limpa memoria da lista (se houver) */
    liberarLista(&cabeca);

    printf("Encerrando. Ate mais.\n");
    return 0;
}
