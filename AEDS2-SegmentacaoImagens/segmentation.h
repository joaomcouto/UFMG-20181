
#ifndef SEGMENTATION_H_INCLUDED
#define SEGMENTATION_H_INCLUDED

#include "pilha.h"

typedef struct {
    short int Vermelho;
    short int Verde ;
    short int Azul ;
} TipoCor ;

void PilhasVazias(int NumSementes, TipoPilha Pilha[]) ;

void Init2D( int DimY , int DimX , short int vetor[DimY][DimX]) ;

FILE * AbreArquivo( char Nome[] , char Ext[] , char Modo [] ) ;

void EmpilhaSemente(TipoCor CorSemente[], int NumSementes, TipoPilha Pilha[], FILE* aux) ;

void GerarPPM(int DimY , int DimX , short int seg[DimY][DimX],short int image[DimY][DimX], TipoCor CorSemente[], FILE* ppm, int NumSementes, int PixelDepth) ;

void LerAuxiliar(FILE* aux, int* NumSementes, int* T) ;

void Segmenta(int NumSementes,TipoPilha Pilha[],int TamY, int TamX, short int seg[TamY][TamX], short int image[TamY][TamX], int T ) ;

void LerPGM(FILE* pgm,int TamX, int TamY, short int image[TamY][TamX]) ;

#endif
