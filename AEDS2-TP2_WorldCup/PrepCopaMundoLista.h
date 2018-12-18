#ifndef PREPCOPAMUNDOLISTA_H_INCLUDED
#define PREPCOPAMUNDOLISTA_H_INCLUDED

/* O programa usuário entrega para este modulo:
    - registro, acompanhado de have string alocados pelo proprio programa usuário.
    - função que informa qual entre dois registros qual vem primeiro quando ordenados.
*/

int     ListaInsere ( void * PPInicio   , char * NovaChave      , void *  NovoRegistro  );
void *  ListaBusca  ( void * PInicio    , const  char * Chave                           );
void *  ListaRemove ( void * PPInicio                                                   );
void    ListaOrdena ( void * PPInicio   , int (*EscolhePrimeiro)(void*,void*)           );

#endif // PREPCOPAMUNDOLISTA_H_INCLUDED

