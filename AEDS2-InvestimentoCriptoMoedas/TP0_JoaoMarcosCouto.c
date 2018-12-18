#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


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
        float proporcao[10];
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
        double desviopadrao;
        double sharpe ;
        float rettotal ;

    } TipoPortifolio;
    TipoPortifolio portif[4] ;

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
        double sharpe ;

        float rmaior, rmenor ;
    } TipoMoeda ;

    int Dia1 = 100000;



        char dataretorno[15000][9] ;

    srand(time(NULL));
    float aux = 1 ;
    TipoMoeda moeda[argc-1];
    for (i=0; i<argc-1; i++){
        portif[0].proporcao[i] = (aux/(argc-1)) ;
        for (j=1; j<=3; j++){
           while ((portif[j].proporcao[i] = rand())== portif[j].proporcao[i-1]);
        }
    }

    for (j=1; j<=3; j++){
        float total = 0;
        for (i=0; i<argc-1; i++){
            total += portif[j].proporcao[i] ;
        }
        for (i=0; i<argc-1; i++){
            portif[j].proporcao[i] /= total ;
        }
    }





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
        portif[i].somaret = 0 ;
        portif[i].rettotal = 1 ;
        for (k=0 ; k < Dia1 ; k++){
           portif[i].retorno[k] = 0 ;
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
                    portif[k].retorno[j-1] += ((portif[k].proporcao[i])*(moeda[i].rcorr)) ;

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
        moeda[i].somades2 = 0 ;
        for(j=0 ; j < moeda[i].tamanho-1 ; j++ ){
            //moeda[i].somades2 += (moeda[i].retorno[j] - moeda[i].mediaret)*(moeda[i].retorno[j] - moeda[i].mediaret) ;
            moeda[i].somades2 += ((moeda[i].retorno[j] - moeda[i].mediaret)*(moeda[i].retorno[j] - moeda[i].mediaret) ); //
        }
       moeda[i].desviopadrao = sqrt(moeda[i].somades2 / (moeda[i].tamanho-1)) ;
       moeda[i].sharpe = (moeda[i].mediaret - 0.00038)/moeda[i].desviopadrao ;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (i=0; i < 4; i++){
        portif[i].menorret = portif[i].retorno[0] ;
        portif[i].indicemenor = 0 ;
        portif[i].maiorret = portif[i].retorno[0] ;
        portif[i].indicemaior = 0 ;
        for (j=1; j<=Dia1-1; j++){
            if (portif[i].retorno[j] < portif[i].menorret){
                portif[i].indicemenor = j ;
                portif[i].menorret = portif[i].retorno[j] ;

            }
            if (portif[i].retorno[j] > portif[i].maiorret){
                portif[i].indicemaior = j ;
                portif[i].maiorret = portif[i].retorno[j] ;
            }
        }
    }
    for (i=0 ; i < 4 ; i++){
        for (j=0 ; j<=Dia1-2 ; j++){
            portif[i].somaret += portif[i].retorno[j] ;
        }
        portif[i].mediaret = portif[i].somaret/(Dia1-1) ;
    }

    for (i = 0; i < 4 ; i++) {
        portif[i].somades2 = 0 ;
        for(j=0 ; j <= Dia1-2 ; j++ ){
            portif[i].rettotal *= (1+portif[i].retorno[j]) ;
            portif[i].somades2 += (portif[i].retorno[j] - portif[i].mediaret)*(portif[i].retorno[j] - portif[i].mediaret) ; //
        }
       portif[i].desviopadrao = sqrt(portif[i].somades2 / (Dia1-1)) ;
       portif[i].sharpe = (portif[i].mediaret - 0.00038)/portif[i].desviopadrao ;
    }



    printf("%22s %15s %10s %10s %10s %10s %10s %10s\n", "DataMenor", "DataMaior","Menor","Maior","Media","Desvio","RTotal", "Sharpe") ;
    char auxmenor[9] ;
    char auxmaior[9] ;

    for (i= 0 ; i < argc-1 ; i++){
        printf ("%-12s ",argv[i+1] );
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
        printf ("%10.2lf%%", moeda[i].desviopadrao*100) ;
        printf ("%11.2f%%", moeda[i].rettotal*100) ;
        printf ("%11.2lf%%", moeda[i].sharpe*100) ;
        printf ("\n") ;
    }
    char aux1[9] ;
    for (i= 0 ; i < 4 ; i++){
        if (argc<=2) break;
        printf ("%s%-2d ","PORTIFOLIO", i+1 );
        strncpy(aux1,dataretorno[portif[i].indicemenor], sizeof(aux1)) ;
        printf (  "%02s"    ,strtok( aux1, "/"  ) );
        printf (  "/%02s"   ,strtok(NULL             , "/"  ) );
        printf ( "/20%2s  "   ,strtok(NULL             , "\0" ) );
        printf ( "    " ) ;
        strncpy(aux1,dataretorno[portif[i].indicemaior], sizeof(aux1)) ;
        printf (  "%02s"    ,strtok(aux1 , "/"  ) );
        printf (  "/%02s"   ,strtok(NULL             , "/"  ) );
        printf ( "/20%2s  "   ,strtok(NULL             , "\0" ) );
        printf ("%8.2f%%", portif[i].menorret*100) ;
        printf ("%10.2f%%", portif[i].maiorret*100) ;
        printf ("%9.2f%%", portif[i].mediaret*100) ;
        printf ("%10.2lf%%", portif[i].desviopadrao*100) ;
        printf ("%11.2f%%", portif[i].rettotal*100) ;
        printf ("%11.2lf%%", portif[i].sharpe*100) ;
        printf ("\n") ;
    }
        printf ("\n") ;
        printf ("PORPORCOES:\n") ;
        printf ("\n") ;
        for (i= 0 ; i < 4 ; i++){
            printf ("%s%-7d ","PORTIFOLIO", i+1 );
            for (j=0 ; j < argc-1 ; j++){
                printf("%.2f", portif[i].proporcao[j]) ;
                if (j == argc-2) break; else printf(", ") ;
            }
            printf("\n") ;
        }
    return 0 ;
}
