#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id;
    int prazo;
    int lucro;
} Tarefa;

typedef struct {
    int inicio;
    int fim;
} Intervalo;


/* EXPLICAÇÃO DA LÓGICA GULOSA PARA O PROBLEMA DOS POSTOS ==================
* A cada parada necssária, o algoritmo escolhe o posto mais 
* distante que ainda está dentro do alcance atual do carro, 
* nunca um posto mais próximo, mesmo que este também estivesse ao alcance.

* A ideia central é: parar mais longe nunca deixa você em pior situação do que 
* parar mais perto, só pode melhorar (ou empatar) suas opções futuras.
* Se de um posto "X" você consegue alcançar tudo que seria alcançável de um posto "Y" 
* mais próximo, então qualquer estratégia que pararia em Y também poderia ter parado em X sem perder 
* nenhuma opção
*/

int minimoParadas(int distanciaTotal, int alcanceMax, int postos[], int n, int paradasEscolhidas[], int *qtdParadas) {
    int posicaoAtual = 0;
    int i = 0;
    *qtdParadas = 0;

    while (posicaoAtual + alcanceMax < distanciaTotal) {
        int melhorPosto = -1;

        while (i < n && postos[i] <= posicaoAtual + alcanceMax) {
            melhorPosto = postos[i];
            i++;
        }

        if (melhorPosto == -1) {
            return -1;
        }

        posicaoAtual = melhorPosto;
        paradasEscolhidas[*qtdParadas] = melhorPosto;
        (*qtdParadas)++;
    }

    return *qtdParadas;
}

void testarCenarioPostos(char *nome, int distanciaTotal, int alcanceMax, int postos[], int n) {
    int paradasEscolhidas[50];
    int qtdParadas = 0;

    printf("=== %s ===\n", nome);
    printf("Distancia Total = %d, Alcanec Max = %d, Postos=[", distanciaTotal, alcanceMax);
    for (int i = 0; i < n; i++) printf(" %d ", postos[i]);
    printf("]\n");

    int resultado = minimoParadas(distanciaTotal, alcanceMax, postos, n, paradasEscolhidas, &qtdParadas);

    if (resultado == -1) {
        printf("SEM SOLUCAO: existe um trecho maior que o alcance maximo do carro.\n\n");
        return;
    }

    printf("Numero minimo de paradas: %d\n", resultado);
    printf("Postos utilizados: ");
    for (int i = 0; i < qtdParadas; i++) {
        printf("%d%s", paradasEscolhidas[i], (i < qtdParadas - 1) ? ", " : "\n");
    }
    printf("\n");
}

int compararIntervalos(const void *a, const void *b) {
    Intervalo *ia = (Intervalo *) a;
    Intervalo *ib = (Intervalo *) b;
    if (ia->inicio != ib->inicio) {
        return ia->inicio - ib->inicio;
    }
    return ib->fim - ia->fim;
}
 

int cobrirIntervalo(Intervalo intervalos[], int n, int L, Intervalo escolhidos[], int *qtdEscolhidos) {
    qsort(intervalos, n, sizeof(Intervalo), compararIntervalos);
 
    int coberto = 0;
    int i = 0;
    *qtdEscolhidos = 0;
 
    while (coberto < L) {
        int melhorFim = coberto; 
        int melhorIdx = -1;
 
        while (i < n && intervalos[i].inicio <= coberto) {
            if (intervalos[i].fim > melhorFim) {
                melhorFim = intervalos[i].fim;
                melhorIdx = i;
            }
            i++;
        }
 
        if (melhorIdx == -1) {
            // nenhum intervalo avancou a cobertura: ha uma lacuna
            return -1;
        }
 
        escolhidos[*qtdEscolhidos] = intervalos[melhorIdx];
        (*qtdEscolhidos)++;
        coberto = melhorFim;
    }
 
    return *qtdEscolhidos;
}

int compararPorLucro(const void *a, const void *b) {
    Tarefa *ta = (Tarefa *) a;
    Tarefa *tb = (Tarefa *) b;
    return tb->lucro - ta->lucro;
}
 
int montarAgenda(Tarefa tarefas[], int n, char agenda[], int maxPrazo, Tarefa descartadas[], int *qtdDescartadas) {
    qsort(tarefas, n, sizeof(Tarefa), compararPorLucro);
 
    for (int d = 0; d <= maxPrazo; d++) agenda[d] = '\0';
 
    int lucroTotal = 0;
    *qtdDescartadas = 0;
 
    for (int i = 0; i < n; i++) {
        int prazoTarefa = tarefas[i].prazo;
        if (prazoTarefa > maxPrazo) prazoTarefa = maxPrazo;
 
        int alocada = 0;
       
        for (int d = prazoTarefa; d >= 1; d--) {  // tenta o dia mais tardio possivel, andando pra tras ate o dia 1
            if (agenda[d] == '\0') {
                agenda[d] = tarefas[i].id;
                lucroTotal += tarefas[i].lucro;
                alocada = 1;
                break;
            }
        }
 
        if (!alocada) {
            descartadas[*qtdDescartadas] = tarefas[i];
            (*qtdDescartadas)++;
        }
    }
 
    return lucroTotal;
}
 
void testarCenarioIntervalos(char *nome, int L, Intervalo intervalos[], int n) {
    Intervalo copia[50];
    for (int i = 0; i < n; i++) copia[i] = intervalos[i];
 
    Intervalo escolhidos[50];
    int qtdEscolhidos = 0;
 
    printf("=== %s ===\n", nome);
    printf("L = %d, Intervalos = [", L);
    for (int i = 0; i < n; i++) {
        printf("(%d,%d)", intervalos[i].inicio, intervalos[i].fim);
    }
    printf("]\n");
 
    int resultado = cobrirIntervalo(copia, n, L, escolhidos, &qtdEscolhidos);
 
    if (resultado == -1) {
        printf("SEM SOLUCAO: existe uma lacuna que nenhum intervalo cobre.\n\n");
        return;
    }
 
    printf("Numero minimo de intervalos: %d\n", resultado);
    printf("Intervalos escolhidos: ");
    for (int i = 0; i < qtdEscolhidos; i++) {
        printf("(%d,%d)", escolhidos[i].inicio, escolhidos[i].fim);
    }
    printf("\n");
}

int main() {
    
    int postos1[] = {5, 10, 15, 20};
    testarCenarioPostos("Cenario dos postos", 25, 10, postos1, 4);

    Intervalo intervalos[] = {{0, 3}, {2, 5}, {4, 7}, {6, 10}, {8, 10}};
    testarCenarioIntervalos("Cenario dos intervalos", 10, intervalos, 5);

    Tarefa tarefas[] = {{'A', 2, 100}, {'B', 1, 19}, {'C', 2, 27}, {'D', 1, 25}, {'E', 3, 15}};
    int n = 5;
 
    int maxPrazo = 0;
    for (int i = 0; i < n; i++) {
        if (tarefas[i].prazo > maxPrazo) maxPrazo = tarefas[i].prazo;
    }

    char agenda[50];
    Tarefa descartadas[50];
    int qtdDescartadas = 0;
    int lucroTotal = montarAgenda(tarefas, n, agenda, maxPrazo, descartadas, &qtdDescartadas);
 
    printf("\n\n=== Agenda de trabalho ===\n");
    for (int d = 1; d <= maxPrazo; d++) {
        if (agenda[d] != '\0') {
            printf("Dia %d: Tarefa %c\n", d, agenda[d]);
        } else {
            printf("Dia %d: (livre, nenhuma tarefa cabe aqui)\n", d);
        }
    }
 
    printf("\n=== Tarefas descartadas (prazo estourado) ===\n");
    if (qtdDescartadas == 0) {
        printf("Nenhuma\n");
    } else {
        for (int i = 0; i < qtdDescartadas; i++) {
            printf("Tarefa %c (prazo=%d, lucro=%d)\n",
                   descartadas[i].id, descartadas[i].prazo, descartadas[i].lucro);
        }
    }
 
    printf("\nLucro total maximizado: R$ %d\n", lucroTotal);

    return 0;
}
