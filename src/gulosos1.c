#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int duracao;
    int deadline;
} Tarefa;

void Merge(Tarefa tarefas[], int inicio, int meio, int fim) {
    int i, j, k;
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
 
    Tarefa *esq = (Tarefa*) malloc(sizeof(Tarefa) * n1);
    Tarefa *dir = (Tarefa*) malloc(sizeof(Tarefa) * n2);
 
    for (i = 0; i < n1; i++)
        esq[i] = tarefas[inicio + i];
    for (j = 0; j < n2; j++)
        dir[j] = tarefas[meio + 1 + j];
 
    i = 0, j = 0, k = inicio;
 
    while (i < n1 && j < n2) {
        if (esq[i].deadline <= dir[j].deadline) {
            tarefas[k++] = esq[i++];
        } else {
            tarefas[k++] = dir[j++];
        }
    }
 
    while (i < n1)
        tarefas[k++] = esq[i++];
    while (j < n2)
        tarefas[k++] = dir[j++];
 
    free(esq);
    free(dir);
}

void MergeSort(Tarefa tarefas[], int inicio, int fim) {
    if(inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        MergeSort(tarefas, inicio, meio);
        MergeSort(tarefas, meio + 1, fim);
        Merge(tarefas, inicio, meio, fim);
    }
}

int minMoedas(int num, int vet[]) {

    int i = 0;
    int count = 0;
    
    while(num != 0) {
        if(num - vet[i] >= 0) {
            num -= vet[i];
            count++;
        } else {
            i++;
        }
    }

    return count;
}

int main() {

    //int vetorMoedas[] = {100, 50, 25, 10, 5, 1};
    //printf("Menor numero de moedas: %d", minMoedas(200, vetorMoedas));

    int duracoes[] = {3, 5, 2, 7, 4};
    int deadlines[] = {6, 9, 4, 10, 7};
    int n = 5;

    Tarefa tarefas[n];
    for (int i = 0; i < n; i++) {
        tarefas[i].id = i + 1;
        tarefas[i].duracao = duracoes[i];
        tarefas[i].deadline = deadlines[i];
    }

    MergeSort(tarefas, 0, n - 1);

    printf("=== Ordem apos ordenar por deadline crescente ===\n");
    for (int i = 0; i < n; i++) {
        printf("Tarefa %d (duracao=%d, deadline=%d)\n",
               tarefas[i].id, tarefas[i].duracao, tarefas[i].deadline);
    }

    int tempoAcumulado = 0;
    int totalConcluidas = 0;

    printf("\n=== Simulando execucao (mais cedo primeiro) ===\n");
    for (int i = 0; i < n; i++) {
        int candidato = tempoAcumulado + tarefas[i].duracao;
        if (candidato <= tarefas[i].deadline) {
            tempoAcumulado = candidato;
            totalConcluidas++;
            printf("Tarefa %d: EXECUTADA  (tempo acumulado = %d <= deadline %d)\n",
                   tarefas[i].id, tempoAcumulado, tarefas[i].deadline);
        } else {
            printf("Tarefa %d: DESCARTADA (tempo acumulado seria %d > deadline %d)\n",
                   tarefas[i].id, candidato, tarefas[i].deadline);
        }
    }
 
    printf("\nTotal de tarefas concluidas no prazo: %d de %d\n", totalConcluidas, n);
    

    return 0;
}
