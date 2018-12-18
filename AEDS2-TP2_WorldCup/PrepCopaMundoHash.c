#include <stdlib.h>
#include <string.h>
#include "PrepCopaMundoHash.h"
#include "PrepCopaMundoArvoreBinariaPesquisa.h"

#define     TamanhoHash 10000

typedef struct {
    char * EstaChave ;
    void * EsteRegistro  ;
} NodoHash ;

typedef NodoHash * HashArray[TamanhoHash];

    /*  Hash djb2: rápido e com boa distribuição http://www.cse.yorku.ca/~oz/hash.html */
int CodigoHash( const char * Chave ){ //Funcao que gera uma posicao no array hash apartir da chave informada
    unsigned long hash = 5381;
    int c;
    while ( *Chave) {
            c = *Chave++;
            hash = ((hash << 5) + hash) + c;
    }
    return hash % TamanhoHash ;
}



int     HashInsere ( void * PPInicio   , char * NovaChave      , void *  NovoRegistro  ){ //Insere a chave e o registro na tabela hash
    HashArray * * PPReturn  =(HashArray * *)PPInicio;
    HashArray *   InicioCast=*((HashArray * * )PPInicio);
    if (InicioCast == NULL) {
            InicioCast=malloc(sizeof(HashArray));
            *PPReturn=InicioCast;
            int Cursor;
            for( Cursor=0;Cursor< TamanhoHash; Cursor ++ ) {
                    (*InicioCast)[Cursor]=NULL;
            }
    }
    NodoHash * NovoNodoHash = (NodoHash*) malloc(sizeof( NodoHash));
    NovoNodoHash->EstaChave     = NovaChave     ;
    NovoNodoHash->EsteRegistro  = NovoRegistro  ;
    int IndiceHash = CodigoHash(NovaChave);
    int IndiceInicial=IndiceHash;
    while((*InicioCast)[IndiceHash] != NULL && (*InicioCast)[IndiceHash]->EstaChave != NULL ) {
          ++IndiceHash;
          IndiceHash %= TamanhoHash;
          if ( IndiceHash == IndiceInicial ) break;
    }
    (*InicioCast)[IndiceHash] = NovoNodoHash;
    return 1;
}

void *  HashBusca  ( void * PInicio    , const char * Chave ){ //Verifica a existencia de uma registro com a chave informada na tabela hash
    HashArray *   InicioCast=((HashArray * )PInicio);
    if ( NULL == InicioCast ) {
            return NULL;
    }
    int IndiceHash = CodigoHash(Chave);
    int IndiceInicial=IndiceHash;
    while((*InicioCast)[IndiceHash] != NULL ) {
        if( 0 == strcasecmp ( (*InicioCast)[IndiceHash]->EstaChave , Chave ) )
                return (*InicioCast)[IndiceHash]->EsteRegistro;
        ++IndiceHash;
        IndiceHash %= TamanhoHash;
       if ( IndiceHash == IndiceInicial ) break;
    }
   return NULL;
}

char * ChaveRemovida; //Armazenamento temporario de uma chave apos sua remocao da tabela hash
int IndiceRemocao=0;
void *  HashRemove ( void * PPInicio  ){ //Remove a proxima entrada existente na tabela hash
    HashArray *   InicioCast=*((HashArray ** )PPInicio);
    if ( NULL == InicioCast ) {
            return NULL;
    }
    while   (   ((*InicioCast)[IndiceRemocao] == NULL ) ||
                ((*InicioCast)[IndiceRemocao]->EstaChave == NULL )
            ) {
        ++IndiceRemocao;
        if ( IndiceRemocao >= TamanhoHash ) {
            IndiceRemocao=0;
            return NULL;
        }
    }
    void *  RegistroRemovido= (*InicioCast)[IndiceRemocao]->EsteRegistro;
            ChaveRemovida=(*InicioCast)[IndiceRemocao]->EstaChave;
    (*InicioCast)[IndiceRemocao]->EstaChave = NULL ;
    (*InicioCast)[IndiceRemocao]->EsteRegistro = NULL;
    IndiceRemocao++;
    return RegistroRemovido;
}

void    HashOrdena ( void * PPInicio   , int (*EscolhePrimeiro)(void*,void*)           ){ //Reorganiza os pares chave e registro na tabela hash de acordo com a funcao EscolhePrimeiro passada pelo usuario
    HashArray * * PPReturn  =(HashArray * *)PPInicio;
    HashArray *   InicioCast=*((HashArray * * )PPInicio);
    //EscolhePrimeiro=EscolhePrimeiro;
    void * NovaArvore=NULL;
    NodoHash * Removido=NULL;
    while ( 1 ){
        Removido=HashRemove(PPReturn);
        if ( ! Removido ) break;
        ArvBinPesqInsere( &NovaArvore, ChaveRemovida , Removido );
    }
    if ( NovaArvore ) {
        ArvBinPesqOrdena(&NovaArvore, EscolhePrimeiro );
        int Posicao=0;
        // NodoHash * NovoNodoHash;
        while ( 1 ){
            Removido=ArvBinPesqRemoveEmOrdem(&NovaArvore);
            if ( ! Removido ) break;
            if ( (*InicioCast)[Posicao] == NULL ) (*InicioCast)[Posicao] = /*(NodoHash*) */ malloc(sizeof(NodoHash));
            (*InicioCast)[Posicao]->EsteRegistro=Removido;
            (*InicioCast)[Posicao]->EstaChave=malloc(2);
            strcpy((*InicioCast)[Posicao]->EstaChave, "X");
            Posicao++;
        };
    }
};

