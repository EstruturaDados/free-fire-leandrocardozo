#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMP 20
#define MAX_NOME 30
#define MAX_TIPO 20

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade;
} Componente;

/* --- utilitários --- */
void trim_newline(char *s) {
    size_t L = strlen(s);
    if (L == 0) return;
    if (s[L-1] == '\n') s[L-1] = '\0';
}

/* copia array de componentes */
void copiarComponentes(Componente dst[], const Componente src[], int n) {
    for (int i = 0; i < n; ++i) dst[i] = src[i];
}

/* mostra componentes formatados */
void mostrarComponentes(const Componente arr[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("\nLista de componentes (%d):\n", n);
    printf("%-3s  %-28s  %-15s  %s\n", "ID", "NOME", "TIPO", "PRIORIDADE");
    for (int i = 0; i < n; ++i) {
        printf("%-3d  %-28s  %-15s  %d\n", i+1, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("\n");
}

/* --- Algoritmos de ordenação (cada um preenche o contador de comparações) --- */

/* Bubble sort por nome (string). Comparações incrementam via *comparacoes */
void bubbleSortPorNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    for (int pass = 0; pass < n - 1; ++pass) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - pass; ++j) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; /* otimização: para se já estiver ordenado */
    }
}

/* Insertion sort por tipo (string). */
void insertionSortPorTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; ++i) {
        Componente chave = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = chave;
    }
}

/* Selection sort por prioridade (int). Ordena crescente: prioridade menor antes. */
void selectionSortPorPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; ++i) {
        int minidx = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[minidx].prioridade) minidx = j;
        }
        if (minidx != i) {
            Componente tmp = arr[i];
            arr[i] = arr[minidx];
            arr[minidx] = tmp;
        }
    }
}

/* --- Busca binária por nome (assume arr ordenado por nome). Conta comparações. */
int buscaBinariaPorNome(const Componente arr[], int n, const char *chave, long *comparacoes) {
    int lo = 0, hi = n - 1;
    *comparacoes = 0;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, arr[mid].nome);
        if (cmp == 0) return mid;
        if (cmp < 0) hi = mid - 1;
        else lo = mid + 1;
    }
    return -1;
}

/* Função "medir tempo" para chamar um algoritmo de ordenação e medir tempo + comparações.
   Recebe ponteiro para função de ordenação com assinatura (Componente[], int, long*).
   Retorna tempo em segundos; comparacoes é preenchido pela função de ordenação. */
double medirTempo(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparacoes) {
    clock_t start = clock();
    algoritmo(arr, n, comparacoes);
    clock_t end = clock();
    return (double)(end - start) / (double)CLOCKS_PER_SEC;
}

/* --- Input / menu --- */

void cadastrarComponente(Componente lista[], int *n) {
    if (*n >= MAX_COMP) {
        printf("Limite de componentes atingido (%d).\n", MAX_COMP);
        return;
    }
    char buffer[128];
    printf("Digite o nome do componente: ");
    fgets(buffer, sizeof(buffer), stdin);
    trim_newline(buffer);
    strncpy(lista[*n].nome, buffer, MAX_NOME-1);
    lista[*n].nome[MAX_NOME-1] = '\0';

    printf("Digite o tipo do componente: ");
    fgets(buffer, sizeof(buffer), stdin);
    trim_newline(buffer);
    strncpy(lista[*n].tipo, buffer, MAX_TIPO-1);
    lista[*n].tipo[MAX_TIPO-1] = '\0';

    printf("Digite a prioridade (1-10): ");
    fgets(buffer, sizeof(buffer), stdin);
    int p = atoi(buffer);
    if (p < 1) p = 1;
    if (p > 10) p = 10;
    lista[*n].prioridade = p;

    (*n)++;
    printf("Componente cadastrado.\n");
}

int main(void) {
    Componente comps[MAX_COMP];
    int total = 0;
    int opc;

    printf("=== Torre de Resgate - Sistema de Priorizacao e Montagem ===\n");

    do {
        printf("\nMenu principal:\n");
        printf("1 - Cadastrar componente (ate %d)\n", MAX_COMP);
        printf("2 - Listar componentes\n");
        printf("3 - Ordenar por nome (Bubble Sort)\n");
        printf("4 - Ordenar por tipo (Insertion Sort)\n");
        printf("5 - Ordenar por prioridade (Selection Sort)\n");
        printf("6 - Busca binaria por nome (requer ordenacao por nome)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) { /* leitura robusta */
            while (getchar() != '\n'); continue;
        }
        while (getchar() != '\n'); /* limpa resto da linha */

        if (opc == 1) {
            cadastrarComponente(comps, &total);
        } else if (opc == 2) {
            mostrarComponentes(comps, total);
        } else if (opc == 3) {
            if (total == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            Componente copia[MAX_COMP];
            copiarComponentes(copia, comps, total);
            long comps_count = 0;
            double tempo = medirTempo(bubbleSortPorNome, copia, total, &comps_count);
            printf("Ordenacao por nome (Bubble Sort) concluida.\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n", comps_count, tempo);
            mostrarComponentes(copia, total);
            /* opcional: copiar de volta para comps se quiser sobrescrever original */
            /* copiarComponentes(comps, copia, total); */
        } else if (opc == 4) {
            if (total == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            Componente copia[MAX_COMP];
            copiarComponentes(copia, comps, total);
            long comps_count = 0;
            double tempo = medirTempo(insertionSortPorTipo, copia, total, &comps_count);
            printf("Ordenacao por tipo (Insertion Sort) concluida.\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n", comps_count, tempo);
            mostrarComponentes(copia, total);
        } else if (opc == 5) {
            if (total == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            Componente copia[MAX_COMP];
            copiarComponentes(copia, comps, total);
            long comps_count = 0;
            double tempo = medirTempo(selectionSortPorPrioridade, copia, total, &comps_count);
            printf("Ordenacao por prioridade (Selection Sort) concluida.\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n", comps_count, tempo);
            mostrarComponentes(copia, total);
        } else if (opc == 6) {
            if (total == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            /* Para usar busca binaria precisamos de um array ordenado por nome.
               Aqui pedimos ao usuário para fornecer um array ordenado ou aceitamos ordenar uma copia. */
            Componente copia[MAX_COMP];
            copiarComponentes(copia, comps, total);
            long comps_count = 0;
            double tempo_ord = medirTempo(bubbleSortPorNome, copia, total, &comps_count);
            printf("Primeiro, ordenamos por nome (Bubble Sort) para habilitar a busca binaria.\n");
            printf("Ordenacao: comparacoes=%ld | tempo=%.6f s\n", comps_count, tempo_ord);
            mostrarComponentes(copia, total);

            char chave[MAX_NOME];
            printf("Digite o nome do componente-chave a procurar: ");
            if (!fgets(chave, sizeof(chave), stdin)) continue;
            trim_newline(chave);
            long comps_bin = 0;
            clock_t tstart = clock();
            int idx = buscaBinariaPorNome(copia, total, chave, &comps_bin);
            double tbin = (double)(clock() - tstart) / (double)CLOCKS_PER_SEC;
            if (idx >= 0) {
                printf("Componente encontrado no indice %d: %s | %s | %d\n", idx+1, copia[idx].nome, copia[idx].tipo, copia[idx].prioridade);
                printf("Comparacoes (binaria): %ld | tempo busca: %.6f s\n", comps_bin, tbin);
            } else {
                printf("Componente nao encontrado. Comparacoes (binaria): %ld | tempo busca: %.6f s\n", comps_bin, tbin);
            }
        } else if (opc == 0) {
            printf("Encerrando.\n");
        } else {
            printf("Opcao invalida.\n");
        }

    } while (opc != 0);

    return 0;
}
