#include <stdlib.h>
#include <string.h>
#include "PrepCopaMundoLista.h"

typedef struct NodeLista {
    char * EstaChave ;
    void * EsteRegistro  ;
    struct NodeLista * Anterior ;
    struct NodeLista * Proximo  ;
} NodeLista ;

int ListaInsere ( void * PPInicio, char * NovaChave, void *  NovoRegistro    ){ //Dada um chave, a insere na proxima posicao disponivel da lista
    NodeLista * * InicioCast=(NodeLista * *)PPInicio;
    NodeLista * NovoNodeLista=malloc(sizeof(NodeLista));
    NovoNodeLista->EstaChave    =  NovaChave      ;
    NovoNodeLista->EsteRegistro =  NovoRegistro   ;
    NovoNodeLista->Anterior    =   NULL      ;
    if ( NULL == *InicioCast){
        NovoNodeLista->Proximo     =   NULL      ;
    } else {
        (*InicioCast)->Anterior=NovoNodeLista;
        NovoNodeLista->Proximo=(*InicioCast);
    }
    *InicioCast=NovoNodeLista;
    return 1;
}

void * ListaRemove ( void * PPInicio ){//Remove o primeiro elemento da lista
    NodeLista * * PPReturn  =(NodeLista * *)PPInicio;
    NodeLista *   InicioCast=*((NodeLista * * )PPInicio);
    if ( NULL == InicioCast){
        return NULL ;
    }
    void * RegistroEncontrado=InicioCast->EsteRegistro;
    if ( ( NULL == InicioCast->Proximo ) && ( NULL == InicioCast->Anterior)){
        (*PPReturn)=NULL;
    } else {
        if ( NULL != InicioCast->Proximo ) {
                (InicioCast->Proximo )->Anterior=(InicioCast)->Anterior;
                (*PPReturn)=InicioCast->Proximo;
        }
        if ( NULL != InicioCast->Anterior) {
                (InicioCast->Anterior)->Proximo= (InicioCast)->Proximo ;
                (*PPReturn)=InicioCast->Anterior;
        }
    }
    free(InicioCast);
    return RegistroEncontrado;
}

void * ListaBusca  ( void * PInicio, const  char * Chave ){//Dada uma chave, verifica sua existencia na lista
    NodeLista * ProximoBusca=(NodeLista*)PInicio;
    while (  NULL != ProximoBusca ){
        if ( 0 == strcasecmp(ProximoBusca->EstaChave , Chave ) ){
            return ProximoBusca->EsteRegistro;
        }
        ProximoBusca=ProximoBusca->Proximo;
    }
    return NULL;
}

int (*EscolhePrimeiroLista)(void*,void*); //Variavel para funcao que identifica entre dois registros qual deve vir primeiro na ordem designada pelo usuario

NodeLista *Divide(NodeLista *InicioCast) //Metodo da divisao de regioes do mergesort
{
    NodeLista *fast = InicioCast,*slow = InicioCast;
    while (fast->Proximo && fast->Proximo->Proximo)
    {
        fast = fast->Proximo->Proximo;
        slow = slow->Proximo;
    }
    NodeLista *temp = slow->Proximo;
    slow->Proximo = NULL;
    return temp;
}

NodeLista *Merge(NodeLista *Bloco1, NodeLista *Bloco2) //Metodo que mescla o conteudo de duas regioes definidas pelo metodo merge a fim de aplicar o algoritmo de mergesort
{ if (!Bloco1)
        return Bloco2;
    if (!Bloco2)
        return Bloco1;
    if ( 1 == EscolhePrimeiroLista( Bloco1-> EsteRegistro , Bloco2->EsteRegistro ) )
    {
        Bloco1->Proximo = Merge(Bloco1->Proximo,Bloco2);
        Bloco1->Proximo->Anterior = Bloco1;
        Bloco1->Anterior = NULL;
        return Bloco1;
    }
    else
    {
        Bloco2->Proximo = Merge(Bloco1,Bloco2->Proximo);
        Bloco2->Proximo->Anterior = Bloco2;
        Bloco2->Anterior = NULL;
        return Bloco2;
    }
}

NodeLista * MergeSort( NodeLista * InicioCast ){ //Programa principal do metodo mergesort. Selecionado para ordenacao de uma lista duplamente encadeada
    if (!InicioCast || !InicioCast->Proximo)
        return InicioCast;
    NodeLista  *Bloco2 = Divide(InicioCast);
    InicioCast = MergeSort(InicioCast);
    Bloco2 = MergeSort(Bloco2);
    return Merge(InicioCast,Bloco2);
}

void ListaOrdena ( void * PPInicio, int (*EscolhePrimeiro)(void*,void*) ){ //Realiza ativa
    NodeLista * * PPReturn  =(NodeLista * *)PPInicio;
    NodeLista *   InicioCast=*((NodeLista * * )PPInicio);
    EscolhePrimeiroLista=EscolhePrimeiro;
    (*PPReturn)=MergeSort( InicioCast );
}

