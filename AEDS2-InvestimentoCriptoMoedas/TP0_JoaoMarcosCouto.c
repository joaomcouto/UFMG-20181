#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Premissa: \r ou \n nao tem significado como conteudo do arquivo
char * crossfgets( char *Buffer , int Limit , FILE *Reading ){
    long int CurrPos=ftell(Reading);
    char * result=fgets(Buffer,Limit,Reading);
    int BufferSize=strlen(Buffer);
    long int NewPos=ftell(Reading);
    if ( result != NULL ){
        char * pos1=strchr(Buffer,'\r');
        char * pos2=strchr(Buffer,'\n');
        if ( ( pos1 != NULL ) || ( pos2 != NULL )) {
                // let's make p1 the first line delimiter and p2 the last
            if ( pos1 == NULL ) {
                pos1=pos2;
            } else if ( pos2 == NULL ) {
                pos2=pos1;
            } else {
                if ( pos1 < pos2 ) {
                    if ( ( pos1 + 1 ) != pos2 ){
                        pos2=pos1;
                    }
                } else {
                    if ( ( pos2 + 1 ) != pos1 ){
                        pos1=pos2;
                    } else {
                        pos1=pos2;
                        pos2++;
                    }
                }
            }
            if ( ( pos1 - Buffer + 1 ) <= Limit ) {
                pos1[0]='\n';
                if ( ( pos1 - Buffer + 2 ) <= Limit ) {
                    pos1[1]='\0';
                } else {
                    Buffer[Limit-1]='\0';
                }
            } else {
                Buffer[Limit-1]='\0';
            }
            if ( pos2-Buffer+1 < BufferSize ) {
                fseek(Reading,CurrPos+pos2-Buffer+1,SEEK_SET );
            }
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    int i, j , k;
    time_t t;
    typedef struct {
        FILE *m ;
        int tamanho ;
        char dcorr[9], dant[9] ;
        float pcorr , pant ;
        float rcorr ;
        char dmaior[9], dmenor[9];
        float somaret ;
        double p1 ;
        double pf ;
        double rettotal ;
        float mediaret ;
        float somades2 ;
        float retorno[15000] ;
        double desviopadrao ;
        float sharpe ;

        float rmaior, rmenor ;
    } TipoMoeda ;

    int Dia1 = 100000;

    typedef struct {
        float proporcao[argc-1];
        float retorno[15000];
        float dmaior[9];
        float dmenor[9];
        float menorret ;
        float maiorret ;
        int indicemaior ;
        int indicemenor ;
        float somaret ;
        float mediaret ;
        float somades2;
        float desviopadrao;
        float sharpe ;
        float rettotal ;

    }TipoPortifolio ;
        char dataretorno[15000][9] ;

    TipoPortifolio port[4] ;

    for (i=0; i<argc-1; i++){
        port[0].proporcao[i] = (1/argc-1) ;
        for (j=1; j<=3; j++){
            srand((unsigned) time(&t));
            port[j].proporcao[i] = rand() ;
        }
    }
    int total = 0;
    for (j=1; j<=3; j++){
        for (i=0; i<argc-1; i++){
            total += port[j].proporcao[i] ;
        }
        for (i=0; i<argc-1; i++){
            port[j].proporcao[i] /= total ;
        }
    }


    TipoMoeda moeda[argc-1];


    char FileName[MAX_PATH];
    for (i=1 ; i<argc ; i++){
        strncpy(FileName, argv[i],sizeof(FileName)) ;
        strncat(FileName, ".csv", sizeof(FileName)-strlen(FileName)) ;

        moeda[i-1].m = fopen(FileName, "r") ;
        if(moeda[i-1].m == NULL) {
            printf("Error opening file");
            return(0);
        }
        fscanf(moeda[i-1].m, "%d ", &moeda[i-1].tamanho ) ;
        if (moeda[i-1].tamanho < Dia1) Dia1 = moeda[i-1].tamanho ;
        //if (moeda[i].retorno = (float *) malloc(sizeof(float)*moeda[i-1].tamanho) == NULL) printf ("Deu ruim") ;
    }
    char linha[255] ;
    char Preco[10] ;
    for (i = 0; i < argc-1 ; i++){ //inicializacoes
        crossfgets(linha, 255, moeda[i].m ) ;
        strncpy(moeda[i].dant , strtok(linha, ",\n"), sizeof(moeda[i].dant) ) ;
        strncpy(Preco, strtok(NULL,",\n"), sizeof(Preco) );
        moeda[i].pant = atof(Preco);
        moeda[i].p1 = moeda[i].pant ;
        moeda[i].rmaior = -10000 ;
        moeda[i].rmenor = 10000 ;
        moeda[i].somaret = 0;
        moeda[i].mediaret = 0;
        moeda[i].rettotal = 0 ;
    }
    for(i = 0; i < 4; i++){ //inicializacao dos portifolios
        port[i].somaret = 0 ;
        port[i].menorret = -10000 ;
        port[i].maiorret = 10000 ;
        port[i].rettotal = 1 ;
        for (k=0 ; k < Dia1 ; k++){
           port[i].retorno[k] = 0 ;
        }
    }
    for(i = 0 ; i < argc-1 ; i++){
        for (j=1 ; j <=moeda[i].tamanho ; j++){
            strncpy(moeda[i].dcorr,moeda[i].dant, sizeof(moeda[i].dcorr)) ;
            moeda[i].pcorr = moeda[i].pant ;
            if (j==moeda[i].tamanho) moeda[i].pf = moeda[i].pcorr ;
            if (crossfgets(linha, 255, moeda[i].m ) == NULL) break ;
            strncpy(moeda[i].dant, strtok(linha, ",\n"), sizeof(moeda[i].dant)) ;
            moeda[i].pant = atof(strtok(NULL,",\n")) ;
            moeda[i].rcorr = (moeda[i].pcorr - moeda[i].pant)/moeda[i].pant ;
            if (j<Dia1){
                strncpy(dataretorno[j-1], moeda[i].dcorr, sizeof(dataretorno[1])) ;
                for (k=0 ; k < 4; k++){
                    port[k].retorno[j-1] += ((port[k].proporcao[i])*(moeda[i].rcorr)) ;

                }
            }
            // moeda[i].retorno[j-1] = moeda[i].rcorr ;
            moeda[i].retorno[j-1] = moeda[i].rcorr ; //
            if (moeda[i].rmenor > moeda[i].rcorr){
                moeda[i].rmenor = moeda[i].rcorr ;
                strncpy(moeda[i].dmenor, moeda[i].dcorr, sizeof(moeda[i].dmenor)) ;
            }
            if (moeda[i].rmaior < moeda[i].rcorr){
                moeda[i].rmaior = moeda[i].rcorr ;
                strncpy(moeda[i].dmaior, moeda[i].dcorr, sizeof(moeda[i].dmaior) ) ;
            }
            moeda[i].somaret += moeda[i].rcorr ;
        }
        moeda[i].mediaret = moeda[i].somaret/(moeda[i].tamanho - 1) ;
    }
     for (i = 0; i < argc-1 ; i++) { //Calculo de desvio padrao, sharpe e retorno total de cada moeda
        moeda[i].rettotal = (moeda[i].p1 - moeda[i].pf)/moeda[i].pf ;
        for(j=0 ; j < moeda[i].tamanho-1 ; j++ ){
            //moeda[i].somades2 += (moeda[i].retorno[j] - moeda[i].mediaret)*(moeda[i].retorno[j] - moeda[i].mediaret) ;
            moeda[i].somades2 += (moeda[i].retorno[j] - moeda[i].mediaret)*(moeda[i].retorno[j] - moeda[i].mediaret) ; //
        }
       moeda[i].desviopadrao = sqrt(moeda[i].somades2 / moeda[i].tamanho) ;
       moeda[i].sharpe = (moeda[i].mediaret - 0.00038)/moeda[i].desviopadrao ;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (i=0; i < 4; i++){
        for (j=0; j<=Dia1-2; j++){
            if (port[i].retorno[j] < port[i].menorret){
                port[i].indicemenor = j ;
                port[i].menorret = port[i].retorno[j] ;

            }
            if (port[i].retorno[j] > port[i].maiorret){
                port[i].indicemaior = j ;
                port[i].maiorret = port[i].retorno[j] ;
            }
        }
    }
    for (i=0 ; i < 4 ; i++){
        for (j=0 ; j<=Dia1-2 ; j++){
            port[i].somaret += port[i].retorno[j] ;
        }
        port[i].mediaret = port[i].somaret/(Dia1-1) ;
    }

    for (i = 0; i < 4 ; i++) { //
        for(j=0 ; j <= Dia1-2 ; j++ ){
            port[i].rettotal *= (1+port[i].retorno[j]) ;
            port[i].somades2 += (port[i].retorno[j] - port[i].mediaret)*(port[i].retorno[j] - port[i].mediaret) ; //
        }
       port[i].desviopadrao = sqrt(port[i].somades2 / Dia1) ;
       port[i].sharpe = (port[i].mediaret - 0.00038)/port[i].desviopadrao ;
    }



    printf("%15s %15s %10s %10s %10s %10s %10s %10s\n", "DataMenor", "DataMaior","Menor","Maior","Media","Desvio","RTotal", "Sharpe") ;
    char auxmenor[9] ;
    char auxmaior[9] ;

    for (i= 0 ; i < argc-1 ; i++){
        printf ("%-5s ",argv[i+1] );
        printf (  "%02s"    ,strtok(moeda[i].dmenor  , "/"  ) );
        printf (  "/%02s"   ,strtok(NULL             , "/"  ) );
        printf ( "/20%2s  "   ,strtok(NULL             , "\0" ) );
        printf ( "    " ) ;
        printf (  "%02s"    ,strtok(moeda[i].dmaior  , "/"  ) );
        printf (  "/%02s"   ,strtok(NULL             , "/"  ) );
        printf ( "/20%2s  "   ,strtok(NULL             , "\0" ) );
        printf ("%8.2f%%", moeda[i].rmenor*100) ;
        printf ("%10.2f%%", moeda[i].rmaior*100) ;
        printf ("%9.2f%%", moeda[i].mediaret*100) ;
        printf ("%10.2f%%", moeda[i].desviopadrao*100) ;
        printf ("%11.2lf%%", moeda[i].rettotal*100) ;
        printf ("%11.2lf%%", moeda[i].sharpe*100) ;
        printf ("\n") ;
    }
    for (i= 0 ; i < 4 ; i++){
        printf ("%s%-15d ","PORTIFOLIO", i+1 );
        printf (  "%02s"    ,strtok(dataretorno[port[i].indicemenor]  , "/"  ) );
        printf (  "/%02s"   ,strtok(NULL             , "/"  ) );
        printf ( "/20%2s  "   ,strtok(NULL             , "\0" ) );
        printf ( "    " ) ;
        printf (  "%02s"    ,strtok(dataretorno[port[i].indicemaior]  , "/"  ) );
        printf (  "/%02s"   ,strtok(NULL             , "/"  ) );
        printf ( "/20%2s  "   ,strtok(NULL             , "\0" ) );
        printf ("%8.2f%%", port[i].menorret*100) ;
        printf ("%10.2f%%", port[i].maiorret*100) ;
        printf ("%9.2f%%", port[i].mediaret*100) ;
        printf ("%10.2f%%", port[i].desviopadrao*100) ;
        printf ("%11.2lf%%", port[i].rettotal*100) ;
        printf ("%11.2lf%%", port[i].sharpe*100) ;
        printf ("\n") ;
    }
    return 0 ;
}
