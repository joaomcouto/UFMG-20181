#ifndef PREPCOPAMUNDOARVOREBINARIAPESQUISA_H_INCLUDED
#define PREPCOPAMUNDOARVOREBINARIAPESQUISA_H_INCLUDED

int     ArvBinPesqInsere ( void * PPInicio   , char * NovaChave      , void *  NovoRegistro  );
void *  ArvBinPesqBusca  ( void * PInicio    , const  char * Chave                           );
void *  ArvBinPesqRemoveEmOrdem ( void * PPInicio                                                   );
void    ArvBinPesqOrdena ( void * PPInicio   , int (*EscolhePrimeiro)(void*,void*)           );

#endif // PREPCOPAMUNDOARVOREBINARIAPESQUISA_H_INCLUDED
