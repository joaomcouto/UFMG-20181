#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

char * crossfgets( char *Buffer , int Limite , FILE *Lendo ){ //Faz, operacionalmente, a mesma coisa que fgets, porem funciona tanto com /r quanto /n
    long int PosAtual=ftell(Lendo);
    char * resultado=fgets(Buffer,Limite,Lendo);
    int TamBuffer=strlen(Buffer);
    long int NovaPos=ftell(Lendo);
    if ( resultado != NULL ){
        char * pos1=strchr(Buffer,'\r');
        char * pos2=strchr(Buffer,'\n');
        if ( ( pos1 != NULL ) || ( pos2 != NULL )) {
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
            if ( ( pos1 - Buffer + 1 ) <= Limite ) {
                pos1[0]='\n';
                if ( ( pos1 - Buffer + 2 ) <= Limite ) {
                    pos1[1]='\0';
                } else {
                    Buffer[Limite-1]='\0';
                }
            } else {
                Buffer[Limite-1]='\0';
            }
            if ( pos2-Buffer+1 < TamBuffer ) {
                fseek(Lendo,PosAtual+pos2-Buffer+1,SEEK_SET );
            }
        }
    }
    return resultado;
}

int main(int argc, char* argv[])
{
    int i, j , k;
    time_t t;
    typedef struct { //armazena dados relevantes para portifolios
        float proporcao[10];
        float *retorno; //armazena os valores de retorno para cada portifolio
        float dmaior[9];
        float dmenor[9];
        float menorret, maiorret ;
        int indicemaior , indicemenor;
        float somaret, mediaret  ;
        float somades2; //Somatorio necessario para o calculo do desvio padrao
        double desviopadrao;
        double sharpe ;
        float rettotal ;

    } TipoPortifolio;
    TipoPortifolio portif[4] ;

    typedef struct { //armazena dados relevantes para moedas
        FILE *m ;
        int tamanho ; //numero de registros no arquivo da moeda
        char dcorr[9], dant[9] ; //dia corrente e dia anteior
        float pcorr , pant ; //preco corrente e preco do dia anterior
        float rcorr ; //retorno corrente (do dia em navegacao em relacao ao anterior a ele)
        char dmaior[9], dmenor[9];
        float somaret ;
        double p1 ; //valor da moeda no dia 1 da moeda
        double pf ; //valor da moeda no dia final da moeda
        double rettotal ;
        float mediaret, somades2 ;
        float *retorno;
        double desviopadrao , sharpe;
        float rmaior, rmenor ;
    } TipoMoeda ;

    int Dia1 = 100000;
    srand(time(NULL));
    float aux = 1 ;
    TipoMoeda moeda[argc-1];
    for (i=0; i<argc-1; i++){ //Gera valores randomicos para as proporcoes de investimento dos portifolios
        portif[0].proporcao[i] = (aux/(argc-1)) ;
        for (j=1; j<=3; j++){
           while ((portif[j].proporcao[i] = rand())== portif[j].proporcao[i-1]);
        }
    }
    for (j=1; j<=3; j++){ //assegura que a soma das proporcoes seja igual a 1 para todos os portifolios
        float total = 0;
        for (i=0; i<argc-1; i++){
            total += portif[j].proporcao[i] ;
        }
        for (i=0; i<argc-1; i++){
            portif[j].proporcao[i] /= total ;
        }
    }
    char FileName[MAX_PATH];
    for (i=1 ; i<argc ; i++){ //Faz a abertura dos arquivos cahamados por parametro de linha de comando
        strncpy(FileName, argv[i],sizeof(FileName)) ;
        strncat(FileName, ".csv", sizeof(FileName)-strlen(FileName)) ;

        moeda[i-1].m = fopen(FileName, "r") ;
        if(moeda[i-1].m == NULL) {
            printf("Error opening file");
            return(0);
        }
        fscanf(moeda[i-1].m, "%d ", &moeda[i-1].tamanho ) ;
        if (moeda[i-1].tamanho < Dia1) Dia1 = moeda[i-1].tamanho ;
        if ((moeda[i-1].retorno = (float *) malloc(sizeof(float)*moeda[i-1].tamanho) )== NULL) fprintf (stderr,"Falha alocando area para retornos por moeda.\n") ;
    }
    // Tentativa falha de alocar dinamicamente a matriz de chars dataretorno
    //char **dataretorno;
    //dataretorno = (char**)calloc(Dia1, sizeof(char*)) ;
    //for (i=0 ; i<Dia1 ; i++){
    //    dataretorno[i] = calloc(9,sizeof(char)) ;
    //    //dataretorno[i][0] = '\0' ;
    //}
    char dataretorno[15000][9];

    char linha[255] ; //Array auxiliar de leitura linha a linha dos arquivos
    char Preco[10] ; //Recebe os valores das moedas como string. Posteriormente utilizado como parametro para atof
    for (i = 0; i < argc-1 ; i++){ //inicializacoes das variaveis acumuladoras de moedas
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
    for(i = 0; i < 4; i++){ //inicializacoes das variaveis acumuladoras de portifolios
        portif[i].somaret = 0 ;
        portif[i].rettotal = 1 ;
        if ((portif[i].retorno = (float *) malloc(sizeof(float)*Dia1)) == NULL) fprintf (stderr,"Falha alocando area para retornos por portifolio.\n") ;
        for (k=0 ; k < Dia1 ; k++){
           portif[i].retorno[k] = 0 ;
        }
    }
    for(i = 0 ; i < argc-1 ; i++){//Obtem os valores das moedas dia a dia, calculando o retorno em cada transicao
        for (j=1 ; j <=moeda[i].tamanho ; j++){
            strncpy(moeda[i].dcorr,moeda[i].dant, sizeof(moeda[i].dcorr)) ;
            moeda[i].pcorr = moeda[i].pant ;
            if (j==moeda[i].tamanho) moeda[i].pf = moeda[i].pcorr ; //Obtem o valor de cada moeda em seu ultimo dia
            if (crossfgets(linha, 255, moeda[i].m ) == NULL) break ; //Quebra o loop de leitura ao atingir o fim de cada arquivo
            strncpy(moeda[i].dant, strtok(linha, ",\n"), sizeof(moeda[i].dant)) ;
            moeda[i].pant = atof(strtok(NULL,",\n")) ;
            moeda[i].rcorr = (moeda[i].pcorr - moeda[i].pant)/moeda[i].pant ;
            if (j<Dia1){ //Calcula o retorno de cada portifolio proporcionalmente a suas proporcoes de investimento
                strncpy(dataretorno[j-1], moeda[i].dcorr, sizeof(dataretorno[1])) ;
                for (k=0 ; k < 4; k++){
                    portif[k].retorno[j-1] += ((portif[k].proporcao[i])*(moeda[i].rcorr)) ;

                }
            }
            moeda[i].retorno[j-1] = moeda[i].rcorr ; //
            if (moeda[i].rmenor > moeda[i].rcorr){ //Averigua qual o dia de menor retorno da cada moeda
                moeda[i].rmenor = moeda[i].rcorr ;
                strncpy(moeda[i].dmenor, moeda[i].dcorr, sizeof(moeda[i].dmenor)) ;
            }
            if (moeda[i].rmaior < moeda[i].rcorr){ //Obtem o dia de maior retorno de cada moeda
                moeda[i].rmaior = moeda[i].rcorr ;
                strncpy(moeda[i].dmaior, moeda[i].dcorr, sizeof(moeda[i].dmaior) ) ;
            }
            moeda[i].somaret += moeda[i].rcorr ; //Somatario de todos os retorno a fim de use no calculo de media
        }
        moeda[i].mediaret = moeda[i].somaret/(moeda[i].tamanho - 1) ;
    }
     for (i = 0; i < argc-1 ; i++) { //Calculo de desvio padrao, sharpe e retorno total de cada moeda
        moeda[i].rettotal = (moeda[i].p1 - moeda[i].pf)/moeda[i].pf ;
        moeda[i].somades2 = 0 ;
        for(j=0 ; j < moeda[i].tamanho-1 ; j++ ){
            moeda[i].somades2 += ((moeda[i].retorno[j] - moeda[i].mediaret)*(moeda[i].retorno[j] - moeda[i].mediaret) ); //
        }
       moeda[i].desviopadrao = sqrt(moeda[i].somades2 / (moeda[i].tamanho-1)) ;
       moeda[i].sharpe = (moeda[i].mediaret - 0.00038)/moeda[i].desviopadrao ;
    }
    for (i=0; i < 4; i++){ //Calculo do menor e maior retorno de cada portfilio
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
    for (i=0 ; i < 4 ; i++){ //Calcula a soma dos retornos dos portilios e utliza a soma obtida para calcular a media de retorno dele
        for (j=0 ; j<=Dia1-2 ; j++){
            portif[i].somaret += portif[i].retorno[j] ;
        }
        portif[i].mediaret = portif[i].somaret/(Dia1-1) ;
    }

    for (i = 0; i < 4 ; i++) { //Calculo do retorno total, desviopadrao e sharpe de cada portifolio
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

    for (i= 0 ; i < argc-1 ; i++){ //Imprime os dados requisitados das moedas
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
    for (i= 0 ; i < 4 ; i++){ //Imprime os dados requisitados dos portifolios 1 a 4
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
        printf ("PROPORCOES:\n") ;
        printf ("\n") ;
        for (i= 0 ; i < 4 ; i++){ //Imprime as proporcoes de investimento utilizadas em cada portifolio
            printf ("%s%-7d ","PORTIFOLIO", i+1 );
            for (j=0 ; j < argc-1 ; j++){
                printf("%.2f", portif[i].proporcao[j]) ;
                if (j == argc-2) break; else printf(", ") ;
            }
            printf("\n") ;
        }
    return 0 ;
}
