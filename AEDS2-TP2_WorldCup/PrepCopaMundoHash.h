#ifndef PREPCOPAMUNDOHASH_H_INCLUDED
#define PREPCOPAMUNDOHASH_H_INCLUDED

int     HashInsere ( void * PPInicio   , char * NovaChave      , void *  NovoRegistro  );
void *  HashBusca  ( void * PInicio    , const char * Chave                           );
void *  HashRemove ( void * PPInicio                                                   );
void    HashOrdena ( void * PPInicio   , int (*EscolhePrimeiro)(void*,void*)           );

#endif // PREPCOPAMUNDOHASH_H_INCLUDED
