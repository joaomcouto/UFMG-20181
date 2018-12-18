#ifndef PILHA_H_INCLUDED
#define PILHA_H_INCLUDED

#include "segmentation.h"

typedef struct {
    int PosX ;
    int PosY ;
} TipoItem;

typedef struct Celula_str *Apontador;

typedef struct Celula_str {
    TipoItem Item;
    Apontador Prox;
} Celula;
typedef struct {
    Apontador Topo, Fundo;
} TipoPilha;

int Vazia(const TipoPilha *) ;

void FPVazia(TipoPilha *) ;

void Empilha(TipoItem , TipoPilha *) ;

int Desempilha(TipoPilha *, TipoItem *) ;

#endif
