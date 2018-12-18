#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PrepCopaMundo.h"
#include "PrepCopaMundoLista.h"
#include "PrepCopaMundoArvoreBinariaPesquisa.h"
#include "PrepCopaMundoHash.h"

#define  LinhaMax           300
#define  TamanhoNomePais    45
#define  TamanhoData        15

typedef char NomePais   [TamanhoNomePais    ];
typedef char ParPaises  [2*TamanhoNomePais+1];
typedef char Data     [15];
typedef unsigned short int       Gols    ;

typedef struct{
    NomePais    PaisRankeado ;
    short int   PontosGanhos ;
    short int   QuantidadeJogos ;
    short int   Vitorias ;
    short int   Empates ;
    short int   Derrotas ;
    short int   GolsMarcados ;
    short int   GolsSofridos ;
    short int   SaldoDeGols ;
    float       Aproveitamento ;
}   ItemRanking ;

typedef struct{
    ParPaises   PaisesJogando ;
    short int   Quantidade  ;
}   ItemConfronto ;


typedef char Linha [LinhaMax];

void    *   ColecaoPaises=NULL ;

void    *   ColecaoConfronto=NULL ;


int PosicionaProximaLinha( FILE * Entrada ){
    Linha       TrechoIgnorado   ;
    if ( NULL == fgets( TrechoIgnorado , LinhaMax -1, Entrada ) ) {
        fprintf( stderr , "Falha encontrando final da linha.\n" );
        return 0 ;
    };
    return 1 ;
}

int     (*ColecaoInsere ) ( void * PPInicio, char * NovaChave, void *  NovoRegistro    );
void *  (*ColecaoBusca  ) ( void * PInicio, const  char * Chave/*, void * PPRegistro   */ );
void *  (*ColecaoRemove ) ( void * PPInicio );
void    (*ColecaoOrdena ) ( void * PPInicio, int (*EscolhePrimeiro)(void*,void*) );


int EscolhePrimeiroPais ( void * AV , void * BV ){ //Compara registros de dois paises e retorna um ou dois conforme o primeiro ou o segundo pais tem que aparecer primeiro no ranking
    ItemRanking * A = (ItemRanking * ) AV;
    ItemRanking * B = (ItemRanking * ) BV;
    if ( A->PontosGanhos > B->PontosGanhos ) return 1;
    if ( A->PontosGanhos < B->PontosGanhos ) return 2;
    if ( A->Aproveitamento > B->Aproveitamento ) return 1;
    if ( A->Aproveitamento < B->Aproveitamento ) return 2;
    if ( A->SaldoDeGols > B->SaldoDeGols ) return 1;
    if ( A->SaldoDeGols < B->SaldoDeGols ) return 2;
    if ( A->GolsMarcados > B->GolsMarcados ) return 1;
    if ( A->GolsMarcados < B->GolsMarcados ) return 2;
    if ( strcasecmp( A->PaisRankeado , B->PaisRankeado ) > 0 ) return 2;
    return 1;
}

int EscolhePrimeiroConfronto ( void * AV , void * BV ){ //Compara pares de paises retornando 1 ou 2 conforme o primeiro ou o segundo tem a maior quantidade de jogos e portante tenha que aprecer primeiro na lista
    ItemConfronto * A = (ItemConfronto * ) AV;
    ItemConfronto * B = (ItemConfronto * ) BV;
    if ( A->Quantidade > B->Quantidade ) return 1;
    if ( A->Quantidade < B->Quantidade ) return 2;
    if ( strcasecmp( A->PaisesJogando , B->PaisesJogando ) > 0 ) return 2;
    return 1;
}

int ProcessaConfrontoPorPais( NomePais EstePais , Gols GolsDeste, Gols GolsDoOutro ){ //De acordo com o resultado de um determinado jogo em analise, atualiza as estatisticas de uma dos paises envolvidos
    ItemRanking * ItemCorrente ;
    ItemCorrente=ColecaoBusca( ColecaoPaises , EstePais );
    if ( NULL == ItemCorrente ) {
        ItemCorrente=malloc(sizeof(ItemRanking)) ;
        strcpy(ItemCorrente->PaisRankeado,EstePais);
        ItemCorrente->QuantidadeJogos=0;
        ItemCorrente->PontosGanhos=0;
        ItemCorrente->Vitorias=0;
        ItemCorrente->Empates=0;
        ItemCorrente->Derrotas=0;
        ItemCorrente->GolsMarcados=0;
        ItemCorrente->GolsSofridos=0;
        ItemCorrente->SaldoDeGols=0;
        ItemCorrente->Aproveitamento=0;
        ColecaoInsere( &ColecaoPaises, ItemCorrente->PaisRankeado, ItemCorrente  );
    }
    if      ( GolsDeste >  GolsDoOutro ) {
            ItemCorrente->PontosGanhos+=3;
            ItemCorrente->Vitorias+=1;
    } else if ( GolsDeste == GolsDoOutro ) {
            ItemCorrente->PontosGanhos+=1;
            ItemCorrente->Empates+=1;
    } else {
            ItemCorrente->Derrotas+=1;
    }
    ItemCorrente->QuantidadeJogos+=1;
    ItemCorrente->GolsMarcados+=GolsDeste;
    ItemCorrente->GolsSofridos+=GolsDoOutro;
    ItemCorrente->SaldoDeGols=ItemCorrente->GolsMarcados-ItemCorrente->GolsSofridos;
    ItemCorrente->Aproveitamento=(float)(100 * ItemCorrente->PontosGanhos)/(float)(3 * ItemCorrente->QuantidadeJogos);
    return 1;
}

int ProcessaConfrontoPorDupla( NomePais PaisA, NomePais PaisB ){ //Processa a ocorrencia de um jogo com o objetivo de atualizar as estatisticas de confrontos
    ParPaises   EstesPaises;
    int Comparacao=strcasecmp( PaisA , PaisB );
    if      ( Comparacao < 0 ) strcat ( strcat ( strcpy( EstesPaises , PaisA ), "," ), PaisB ) ;
    else if ( 0 < Comparacao ) strcat ( strcat ( strcpy( EstesPaises , PaisB ), "," ), PaisA ) ;
    else {
        fprintf ( stderr , "Erro: Confronto de %s com o mesmo %s.\n" , PaisA , PaisB ) ;
        return 0;
    }
    ItemConfronto * ConfrontoToUpate ;
    ConfrontoToUpate=ColecaoBusca( ColecaoConfronto , EstesPaises);
    if ( NULL == ConfrontoToUpate ) {
        ConfrontoToUpate=malloc(sizeof(ItemConfronto));
        strcpy(ConfrontoToUpate->PaisesJogando,EstesPaises);
        ConfrontoToUpate->Quantidade=0;
        ColecaoInsere(&ColecaoConfronto,ConfrontoToUpate->PaisesJogando,ConfrontoToUpate);
    }
    ConfrontoToUpate->Quantidade+=1;
    return 1;
}

int ResultadosCarrega( char * ArquivoResultados, unsigned short int AnoInicial, unsigned short int AnoFinal ){ //Le o arquivo e identifica os dados de interesse a chama as funcoes de processamentos
    FILE * Entrada=fopen( ArquivoResultados , "r" );
    if ( ( Entrada != NULL )) {
        unsigned short  AnoLido         ;
        Data            DataConfronto   ;
        NomePais        PaisA           ;
        NomePais        PaisB           ;
        Gols            GolsA           ;
        Gols            GolsB           ;
        char            Torneio [ 50 ]  ;
                /* No inicio da primeira iteração, saltacabeçalho.  */
        while ( PosicionaProximaLinha( Entrada ) &&
                0 <= fscanf( Entrada ,
                             "%4hu-"
                             "%11[^,],"
                             "%100[^,],"
                             "%100[^,],"
                             "%hu,"
                             "%hu,"
                             "%100[^,]," ,
                             &AnoLido    ,
                             DataConfronto,
                             PaisA,
                             PaisB,
                             &GolsA,
                             &GolsB,
                             Torneio
                            )
               ){
            if  (   ( 0   == AnoInicial ) ||
                    ( (   0   == AnoFinal           ) && ( AnoLido == AnoInicial    ) ) ||
                    ( (   AnoInicial <= AnoLido    ) && ( AnoLido <= AnoFinal      ) )
                ) {
                if ( 0 == strcasecmp( "FIFA World Cup" , Torneio ) ) {
                    ProcessaConfrontoPorPais( PaisA, GolsA , GolsB   );

                    ProcessaConfrontoPorPais( PaisB, GolsB , GolsA   );
                }
                ProcessaConfrontoPorDupla( PaisA, PaisB           );

            }
        }
        fclose( Entrada );
        return 1 ;
    }
    fprintf( stderr , "Falha abrindo o arquivo %s: %s\n" , ArquivoResultados , (char *)strerror(errno));
    return 0 ;
}

int Ordena( )  { //Chama a função de ordenação para cada uma das coleções de paises e confrontos passando as respectivas funcoes com criterios de ordenacao
        ColecaoOrdena( &ColecaoPaises       , EscolhePrimeiroPais       );
        ColecaoOrdena( &ColecaoConfronto    , EscolhePrimeiroConfronto  );
    return 1;
}


int RankingGera      (FILE* saida )  { //Gerador da saida final do ranking requisitado e ja ordenado
    fprintf( saida ,"LALA Ranking,P,J,V,E,D,GM,GS,SG,AP\n");
    ItemRanking * Item;
    while ( 1 ){
        Item=ColecaoRemove(&ColecaoPaises);
        if ( ! Item ) break;
        fprintf( saida,"%s,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hi,%.2f\n" ,
                Item->PaisRankeado,
                Item->PontosGanhos,
                Item->QuantidadeJogos,
                Item->Vitorias,
                Item->Empates,
                Item->Derrotas,
                Item->GolsMarcados,
                Item->GolsSofridos,
                Item->SaldoDeGols,
                Item->Aproveitamento
            );
        free(Item);
    }
    return 1;
}

int ConfrontosGera  ( )  { //Gerador da saida final dos confrontos
    printf( "Confrontos\n");
    ItemConfronto * Item;
    while ( 1 ){
        Item=ColecaoRemove(&ColecaoConfronto);
        if ( ! Item ) break;
        printf( "%s,%hu\n" ,
                Item->PaisesJogando,
                Item->Quantidade
            );
        free(Item);
    }
    return 1;
}

int MemoriaLibera    ( ) { //Funcao reservada para a liberacao de memoria alocada dinamicamente durante a execusao do programa, que eventualmente nao tenha sido liberada pelos seus respectivos processos
    return 1;
}

int main( int argc, char *argv[] )
{
    char        ArquivoResultados[255]; //ARmazena o nome do arquivo de entrada
    char        Metodo=0; //Armazena da entra o metodo a ser utlizado
    short int   AnoInicial=0;
    short int   AnoFinal=0;
    switch( argc ){ //Switch realiza a atribuicao aos parametros acima de acordo com o numero de argumentos recebidos
        case 5:     AnoFinal    =atoi( argv[4] );
        case 4:     AnoInicial  =atoi( argv[3] );
        case 3:     Metodo      =atoi( argv[2] );
                    strcpy(ArquivoResultados, argv[1] );
                    break;
        default:    fprintf( stderr ,   "Parametros invalidos. Informe método de busca e, opcionalmente"
                                        ", ano inicial e, também opcionalmente, ano final."
                            ) ;
                    return 1 ;
    }

    switch( Metodo ){ //De acordo com o parametro para o metodo atribui as variaveis correspondentes as funcoes que implementam os metodos de insercao busca, remocao e ordenacao dos dados implementadas pelo modulo correspondente ao metodo selecionado
        case 1: ColecaoBusca    =ListaBusca     ;
                ColecaoInsere   =ListaInsere    ;
                ColecaoRemove   =ListaRemove    ;
                ColecaoOrdena   =ListaOrdena    ;
                break;
        case 2: ColecaoBusca    =ArvBinPesqBusca     ;
                ColecaoInsere   =ArvBinPesqInsere    ;
                ColecaoRemove   =ArvBinPesqRemoveEmOrdem    ;
                ColecaoOrdena   =ArvBinPesqOrdena    ;
                break ;
        case 3: ColecaoBusca    =HashBusca     ;
                ColecaoInsere   =HashInsere    ;
                ColecaoRemove   =HashRemove    ;
                ColecaoOrdena   =HashOrdena    ;
                break;
        default: fprintf( stderr ,   "Médoto invalido: %s. Indique um valor entre 1 e 3" , argv[2] );
                return 0;
    }

    if ( ResultadosCarrega( ArquivoResultados, AnoInicial , AnoFinal   ) ) { //Ativa os diversos processos para a geracao da saida final
    if ( Ordena           (               ) ) {
    FILE* saida ;
    saida = fopen("saida.txt", "w") ;

    if ( RankingGera      (saida           ) ) {
    if ( 0 <= fprintf        ( saida,"\n")         ) {
    if ( ConfrontosGera  (    saida           ) ) {
    if ( 0 <= fprintf        ( saida,"\n")         ) {
    if ( MemoriaLibera    (               ) ) {
    fclose(saida) ;
        return 0;
    }}}}}
    fclose(saida);
    }}

    return 1;
}


