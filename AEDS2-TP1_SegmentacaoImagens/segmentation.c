#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "segmentation.h"
#include "pilha.h"

void PilhasVazias(int NumSementes, TipoPilha Pilha[]){
        int i ;
        for (i = 0 ; i < NumSementes ; i++) { FPVazia(&Pilha[i]) ; }
}

void LerAuxiliar(FILE* aux, int* NumSementes, int* T){
        fscanf(aux, "%d %d", NumSementes, T) ;
}

void Init2D( int DimY , int DimX , short int vetor[DimY][DimX]){
    int i,j ;
    for (i =0 ; i < DimY ; i++){
        for (j= 0; j < DimX ; j++){
            vetor[i][j] = 0 ;
        }
    }
    return ;
}

void LerPGM(FILE* pgm,int TamX, int TamY, short int image[TamY][TamX]){
    int i , j ;
    for (i =0 ; i < TamY ; i++){
        for (j= 0; j < TamX ; j++){
            fscanf(pgm, "%d", &image[i][j]) ;
        }
    }

}

FILE * AbreArquivo( char Nome[] , char Ext[] , char Modo [] ){
    char FileName[MAX_PATH] ;
    strncpy(FileName, Nome, MAX_PATH ) ;
    strncat(FileName, Ext , MAX_PATH-strlen(FileName)) ;
    return fopen(FileName, Modo) ;
}

void EmpilhaSemente(TipoCor CorSemente[], int NumSementes, TipoPilha Pilha[], FILE* aux){
        char dummy , dummy2;
        int i ;
        TipoItem Semente;
        for (i=0 ; i < NumSementes ; i++){
            fscanf(aux, "%d, %d ", &Semente.PosX, &Semente.PosY) ;
            fscanf(aux, "%c%d, %d, %d%c\n" , &dummy, &CorSemente[i].Vermelho , &CorSemente[i].Verde, &CorSemente[i].Azul, &dummy2 ) ;
            Empilha(Semente, &Pilha[i]) ;
        }
        return ;
}

void GerarPPM(int DimY , int DimX , short int seg[DimY][DimX],short int image[DimY][DimX], TipoCor CorSemente[], FILE* ppm, int NumSementes, int PixelDepth){
    int temp ;
    int i, j ;
    fprintf(ppm, "P3\n%hi %hi\n%hi\n", DimX, DimY, PixelDepth) ;
    for (i =0 ; i < DimY ; i++){
        for (j= 0; j < DimX ; j++){
            temp = seg[i][j] ;
                if (seg[i][j] == 0) {
                    fprintf(ppm, "%hi %hi %hi  ", image[i][j] , image[i][j], image[i][j]) ;
                } else{
                    if (seg[i][j] <= NumSementes){
                        fprintf(ppm, "%hi %hi %hi  ", CorSemente[temp-1].Vermelho , CorSemente[temp-1].Verde, CorSemente[temp-1].Azul) ;
                    } else {
                        fprintf ( stderr ,"Numero de grupo %d: invalido  ", seg[i][j]) ;
                    }
                }
        }
    }
}

void Segmenta(int NumSementes,TipoPilha Pilha[],int TamY, int TamX, short int seg[TamY][TamX], short int image[TamY][TamX], int T ){
        int X, Y , i, j;
        TipoItem ItemAux, V[4] ;
        for (i=0; i < NumSementes ; i++){
            while(!Vazia(&Pilha[i])){ ;
                Desempilha(&Pilha[i], &ItemAux) ;
                X = ItemAux.PosX ;
                Y = ItemAux.PosY ;
                seg[Y][X] = i+1 ;
                V[0].PosX = X ;
                V[0].PosY = Y-1 ;
                V[1].PosX = X + 1 ;
                V[1].PosY = Y ;
                V[2].PosX = X ;
                V[2].PosY = Y + 1 ;
                V[3].PosX = X - 1 ;
                V[3].PosY = Y ;
                for(j = 0 ; j < 4 ; j++){
                    if((V[j].PosX>= 0 ) && (V[j].PosX < TamX) && (V[j].PosY >=0) &&(V[j].PosY < TamY)){
                       if((seg[V[j].PosY][V[j].PosX] == 0) && (abs(image[Y][X]-image[V[j].PosY][V[j].PosX])<= T ))
                            Empilha(V[j], &Pilha[i]) ;
                    }
                }
            }
        }

}
