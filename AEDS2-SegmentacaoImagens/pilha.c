#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "segmentation.h"

int Vazia(const TipoPilha *Pilha){
        return (Pilha->Fundo == Pilha->Topo);
    }
void FPVazia(TipoPilha *Pilha) {
    Pilha->Topo = (Apontador) malloc(sizeof(Celula));
    Pilha->Fundo = Pilha->Topo;
    Pilha->Topo->Prox = NULL;
}
void Empilha(TipoItem item, TipoPilha *Pilha) {
    Apontador aux;
    aux = (Apontador) malloc(sizeof(Celula));
    Pilha->Topo->Item = item;
    aux->Prox = Pilha->Topo;
    Pilha->Topo = aux;
}
int Desempilha(TipoPilha *Pilha, TipoItem *item) {
    Apontador q;
    if(Vazia(Pilha)){
        return 0;
    }
    q = Pilha->Topo;
    Pilha->Topo = q->Prox;
    free(q);
    *item = Pilha->Topo->Item; return 1;
}
