#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct NodeABP {
    char * EstaChave ;
    void * EsteRegistro  ;
    struct NodeABP * Esquerda ;
    struct NodeABP * Direita  ;
} NodeABP ;

struct NodeABP* ArvBinPesqInsereInterno(NodeABP * InicioCast, char * NovaChave, void *  NovoRegistro  ){ //insere dados na arvore binaria de busca, criando novos nodes nas posicoes adequadas
    if (InicioCast == NULL) {
            NodeABP * NovoNo=malloc(sizeof(NodeABP));
            NovoNo->EstaChave    =  NovaChave    ;
            NovoNo->EsteRegistro =  NovoRegistro ;
            NovoNo->Esquerda     =  NULL         ;
            NovoNo->Direita      =  NULL         ;
            return NovoNo ;
    }
    int Comparacao=strcasecmp(NovaChave, InicioCast-> EstaChave) ;
    if ( Comparacao < 0 )
        InicioCast->Esquerda  = ArvBinPesqInsereInterno(InicioCast->Esquerda, NovaChave , NovoRegistro );
    else if (Comparacao > 0 )
        InicioCast->Direita = ArvBinPesqInsereInterno(InicioCast->Direita, NovaChave , NovoRegistro );
    return InicioCast;
}

int     ArvBinPesqInsere ( void * PPInicio   , char * NovaChave      , void *  NovoRegistro  ){ //Função de interface da insercao de dados na arvore binaria para ativacao por programas externos. Faz a conversao de ponteiros void para ponteiros para a estrutura de dados. Retorna o status da operação
    NodeABP * * PPReturn  =(NodeABP * *)PPInicio;
    NodeABP *   InicioCast=*((NodeABP * * )PPInicio);
    *PPReturn=ArvBinPesqInsereInterno(InicioCast, NovaChave , NovoRegistro );
    return 1;
}

NodeABP * ArvBinPesqBuscaInterno( NodeABP* InicioCast,  char * Chave ){ //Função que verifica a existencia de uma chave na arvore binario, caso encontrado retorna o registro associado a chave
    if (InicioCast == NULL ) return NULL;
    int Comparacao=strcasecmp(Chave, InicioCast-> EstaChave) ;
    if      ( Comparacao < 0 ) return ArvBinPesqBuscaInterno(InicioCast->Esquerda, Chave );
    else if ( Comparacao > 0 ) return ArvBinPesqBuscaInterno(InicioCast->Direita, Chave );
    return  InicioCast->EsteRegistro;
}

void *  ArvBinPesqBusca  ( void * PInicio    ,  char * Chave ){ //Função de interface do metodo de busca para ativacao por componentes externos. Faz a conversao de ponteiros genericos para ponteiros da estrutura de dados da arvore correspondente
    NodeABP *   InicioCast=((NodeABP * )PInicio);
    return ArvBinPesqBuscaInterno(  InicioCast,     Chave );
}

NodeABP * ArvBinPesqRemoveEmOrdemNext=NULL; //Armazena temporariamente nodes que tenham sido removidos da arvore

NodeABP * ArvBinPesqRemoveEmOrdemInterno( NodeABP* InicioCast ){ //Função que remove o primeiro node em in-order da arvore binaria de busca e retorna um ponteiro para o node removido
    if ( NULL == InicioCast             )   return NULL;
    if ( NULL != InicioCast->Esquerda   )   {
            InicioCast->Esquerda=ArvBinPesqRemoveEmOrdemInterno( InicioCast->Esquerda ) ;
            return InicioCast;
    }
    ArvBinPesqRemoveEmOrdemNext=InicioCast;
    return InicioCast->Direita ;
}

void *  ArvBinPesqRemoveEmOrdem ( void * PPInicio ){ //Função de interface da retirada em in-order, retorna um ponteiro para o node removido.
    NodeABP * * PPReturn  =(NodeABP * *)PPInicio;
    NodeABP *   InicioCast=*((NodeABP * * )PPInicio);
    ArvBinPesqRemoveEmOrdemNext=NULL;
    *PPReturn=ArvBinPesqRemoveEmOrdemInterno(InicioCast);
    if ( NULL != ArvBinPesqRemoveEmOrdemNext ){
        void * ReturningItem=ArvBinPesqRemoveEmOrdemNext->EsteRegistro;
        free( ArvBinPesqRemoveEmOrdemNext ) ;
        return  ReturningItem ;
    }
    return NULL;
}

int (*EscolhePrimeiroArvore )(void*,void*); //Interface da função a ser passada pelo componente usuário que seleciona entre os registros de dois nodes o que vem primeiro na ordem desejada pelo usuário

struct NodeABP* ArvBinPesqInsereNewOrder(NodeABP * InicioCast, NodeABP *  NovoNo  ){ //Insere um node na arvore binaria de busca de acordo com o criterio de ordenacao passado pelo usuário, returna um pinteiro para a struct node
    if (InicioCast == NULL) {
            return NovoNo ;
    }
    if ( 2 == EscolhePrimeiroArvore( InicioCast-> EsteRegistro , NovoNo->EsteRegistro ) )
        InicioCast->Esquerda= ArvBinPesqInsereNewOrder(InicioCast->Esquerda , NovoNo );
    else
        InicioCast->Direita = ArvBinPesqInsereNewOrder(InicioCast->Direita  , NovoNo );
    return InicioCast;
}

void    ArvBinPesqOrdena ( void * PPInicio   , int (*EscolhePrimeiro)(void*,void*)           ){ //Função que reorganiza a arvore binaria de acordo com o criterio de ordenacao passado na função EscolhePrimeiroArvore
    NodeABP * * PPReturn  =(NodeABP * *)PPInicio;
    EscolhePrimeiroArvore=EscolhePrimeiro;
    NodeABP * NovaArvore=NULL;
    while ( 1 ){
        ArvBinPesqRemoveEmOrdemNext=NULL;
        *PPReturn=ArvBinPesqRemoveEmOrdemInterno(*PPReturn);
        if ( ! ArvBinPesqRemoveEmOrdemNext ) break;
        ArvBinPesqRemoveEmOrdemNext->Direita    =NULL;
        ArvBinPesqRemoveEmOrdemNext->Esquerda   =NULL;
        NovaArvore=ArvBinPesqInsereNewOrder( NovaArvore, ArvBinPesqRemoveEmOrdemNext );
    }
    *PPReturn=NovaArvore;
};

