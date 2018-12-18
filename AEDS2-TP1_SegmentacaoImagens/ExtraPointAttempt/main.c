#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "segmentation.h"
#include "pilha.h"

// A funcao abaixo foi minha tentativa de obter o ponto extra, impedida pelo limite de utilizacao de memoria do codeblocks. Favor ler a secao conclusao da documentacao


 void RecSegmenta(int PosY, int PosX, int * Visitados,  TipoCor CorRand, int T, int TamX, int TamY , short int Recimage[TamY][TamX], short int Recseg[TamY][TamX], TipoCor RecRGB[TamY][TamX] ){
        Recseg[PosY][PosX] = 1 ;
        Visitados -= 1 ;
        if ((PosY < 0) || (PosX<0)  || (PosY>TamY) || (PosX > TamX)) return ;
        if (!(PosY+1<0 || PosY+1 >=TamY) && (abs(Recimage[PosY][PosX] - Recimage[PosY+1][PosX])<= T) && (Recseg[PosY+1][PosX] == 0) ){
            Recseg[PosY+1][PosX] = 1 ;
            RecRGB[PosY+1][PosX].Vermelho = CorRand.Vermelho ;
            RecRGB[PosY+1][PosX].Verde = CorRand.Verde ;
            RecRGB[PosY+1][PosX].Azul = CorRand.Azul ;
            RecSegmenta(PosY+1 , PosX, &Visitados, CorRand, T, TamX, TamY, Recimage, Recseg, RecRGB) ;
        }
        if (!(PosY-1<0 || PosY-1 >=TamY) && (abs(Recimage[PosY][PosX] - Recimage[PosY-1][PosX])<= T) && (Recseg[PosY-1][PosX] == 0) ){
            Recseg[PosY-1][PosX] = 1 ;
            RecRGB[PosY-1][PosX].Vermelho = CorRand.Vermelho ;
            RecRGB[PosY-1][PosX].Verde = CorRand.Verde ;
            RecRGB[PosY-1][PosX].Azul = CorRand.Azul ;
            RecSegmenta(PosY-1 , PosX, &Visitados, CorRand, T, TamX, TamY, Recimage, Recseg, RecRGB) ;
        }
        if (!(PosX+1<0 || PosX+1 >=TamX) && (abs(Recimage[PosY][PosX] - Recimage[PosY][PosX + 1])<= T) && (Recseg[PosY][PosX+1] == 0) ){
            Recseg[PosY][PosX + 1] = 1 ;
            RecRGB[PosY][PosX + 1].Vermelho = CorRand.Vermelho ;
            RecRGB[PosY][PosX + 1].Verde = CorRand.Verde ;
            RecRGB[PosY][PosX + 1].Azul = CorRand.Azul ;
            RecSegmenta(PosY , PosX + 1 , &Visitados, CorRand, T, TamX, TamY, Recimage, Recseg, RecRGB) ;
        }
        if (!(PosX-1<0 || PosX-1 >=TamX) && (abs(Recimage[PosY][PosX] - Recimage[PosY][PosX -1 ])<= T) && (Recseg[PosY][PosX-1] == 0) ){
            Recseg[PosY][PosX -1] = 1 ;
            RecRGB[PosY][PosX -1].Vermelho = CorRand.Vermelho ;
            RecRGB[PosY][PosX -1].Verde = CorRand.Verde ;
            RecRGB[PosY][PosX -1].Azul = CorRand.Azul ;
            RecSegmenta(PosY , PosX -1 , &Visitados, CorRand, T, TamX, TamY, Recimage, Recseg, RecRGB) ;
        }
    }


int TamX , TamY ;

int main(int argc, char* argv[]){
    int PixelDepth ; //armazena a quantidade de tons de cinza presentes no PGM
    int NumSementes, T;
    FILE* pgm ; //ponteiro para handle do arquivo PGM
    FILE* aux ; //ponteiro para handle do arquivo auxiliar
    aux = AbreArquivo(argv[1], ".txt", "r") ; //realiza a abertura do arquivo auxiliar
    pgm = AbreArquivo(argv[1], ".pgm", "r");//realiza a abertura do arquivo pgm
    char magicnumber[2] ; //armazena a codificacao utilizada no PGM
    fgets(magicnumber, MAX_PATH, pgm) ; //Le a codificao utilizada no PGM averto
    fscanf(pgm, "%d %d", &TamX, &TamY); //Le a dimensao da imagem a ser segmentada
    fscanf(pgm, "%d", &PixelDepth); //Le a quantidade de tons de cinza presentes no PGM
    LerAuxiliar(aux, &NumSementes, &T) ; //Le, no .txt auxiliar, o numero de sementes e a constante T
    short int image[TamY][TamX] ; //Matriz que armazenara os tons de cinza de cada pixel no PGM
    short int seg[TamY][TamX] ; //Matriz que armazenara o numero do grupo a qual pertence cada pixel



    Init2D(TamY,TamX,seg) ;

    LerPGM(pgm, TamX, TamY, image) ;

    TipoPilha Pilha[NumSementes] ;
    TipoCor CorSemente[NumSementes] ;
    PilhasVazias(NumSementes,  Pilha) ; //Cria uma pilha vazia para cada uma das sementes
    EmpilhaSemente(CorSemente, NumSementes,  Pilha, aux) ; //Empilha as sementes em suas respectivas pilhas
    Segmenta(NumSementes,Pilha,TamY, TamX,seg, image, T) ; //Algoritmo principal de expansao de regiões
    FILE* ppm = AbreArquivo(argv[1], ".ppm", "w");
    GerarPPM(TamY, TamX, seg,image, CorSemente, ppm, NumSementes, PixelDepth) ; //Algortimo principal de colorimento dos pixels e consequente geracao do PPM de saida final




    Init2D(TamY,TamX,seg) ;

    FILE * rec ;
    TipoItem SementeRand ;
    TipoCor RecRGB[TamY][TamX] ;
    int i, j ;

    for (i=0 ; i < TamY; i++ ){
        for (j = 0 ; j < TamX ; j++){
            RecRGB[i][j].Vermelho = 0 ;
            RecRGB[i][j].Verde = 0 ;
            RecRGB[i][j].Azul = 0 ;
        }
    }
    srand(time(NULL));
    int Visitados = TamX * TamY ;
    while(Visitados > 200 ){
        SementeRand.PosX = rand() % (TamX + 1 ) ;
        SementeRand.PosY = rand() % (TamY + 1 ) ;
        TipoCor CorRand ;
        CorRand.Azul = rand() % (PixelDepth +1) ;
        CorRand.Verde = rand() % (PixelDepth +1) ;
        CorRand.Vermelho = rand() % (PixelDepth +1) ;
        RecSegmenta(SementeRand.PosY, SementeRand.PosX, &Visitados, CorRand, T, TamX, TamY, image, seg, RecRGB) ;

    }
    rec = AbreArquivo(argv[1], "Rec.ppm", "w");
    fprintf(rec, "P3\n%hi %hi\n%hi\n", TamX, TamY, PixelDepth) ;

    for (i = 0 ; i < TamY ; i++){
        for (j = 0 ; j < TamX; j++){
            fprintf(rec, "%hi %hi %hi  ", RecRGB[i][j].Vermelho , RecRGB[i][j].Verde, RecRGB[i][j].Azul) ;
        }
    }



return 0 ;
}
