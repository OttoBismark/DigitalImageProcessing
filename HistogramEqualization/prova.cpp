#include <iostream>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void calcolaIstogrammi(Mat src, int []);

void calcolaCdf(int [], float []);

void cdfScalato(float[], int [], int );

void elaboraImmagine(int [], Mat );

int main(int argc, char **argv)
{
  int barre[256] = {0};
  float cdf[256] = {0.0};
  int cdfS[256] = {0};

  Mat immagine_sorgente(imread(argv[1], IMREAD_GRAYSCALE));
  Mat immagine_elaborata(immagine_sorgente.clone());
  // Mat immagine_OPCV(immagine_sorgente.clone());

  imshow("Immagine in input", immagine_sorgente);

  /*
  Chiamata alla funzione calcolaIstogramma per il calcolo
  dell'istogramma dell'immagine originale
  */
  calcolaIstogrammi(immagine_sorgente,barre);
  /*Chiamata della funzione cdf per calcolare l'istogramma cumulativo*/
  calcolaCdf(barre,cdf);
  /*Chiamata alla funzione cdfScalato per calcolare la funzione di
  distribuzione cumulativa*/
  cdfScalato(cdf, cdfS, immagine_sorgente.rows * immagine_sorgente.cols);
  /*Chiamata alla funzione elaboraImmagine per elaborare l'immagine finale
  dopo aver calcolato l'istogramma*/
  elaboraImmagine(cdfS, immagine_elaborata);
  
  imshow("Immagine Elaborata", immagine_elaborata);
  // equalizeHist( immagine_sorgente, immagine_OPCV );

  // imshow("Immagine Elaborata con OPCV", immagine_OPCV);
  waitKey();
  return 0;
}

/*

Funzione per calcolare l'istogramma dell'immagine sorgente

*/
void calcolaIstogrammi(Mat src, int b[])
{
  for (int i=0; i<src.rows; i++)
  {
    for (int j=0; j<src.cols; j++)
    {
      /*
      Calcolo delle occorrenze dell'immagine nel vettore b[]
      All'interno di questo vettore avremo il numero di pixel che
      hanno stesso livello di intensità
      */
      b[(int)src.at<uchar>(i,j)]++;
    }
  }
}

/*
Funzione che, a partire dall'array occorrenze b[]
ci calcoliamo l'istogramma cumulativo attraverso la
funzione di distribuzione dell'istogramma
*/
void calcolaCdf(int b[], float cdf[])
{
  cdf[0] = b[0];
  for (int i=1; i<256; i++)
  {
    /*
    Si parte dall'i+1 per poi accumulare tutti gli altri
    i-1 elementi dell'istogramma, in modo da avere un'istogramma
    spaziato e ben equalizzato
    */
    cdf[i] = (float)b[i] + cdf[i-1];
  }
}

/*
Funzione che ci permette di effettuare l'ultimo passaggio per
l'equalizzazione dell'istogramma.
Questa funzione prende gli "i" elementi del vettore dell'istogramma
cumulativo sottraendolo al primo -> cdf[0]: ogni i-simo elemento
va diviso con la dimensione massima dell'immagine. Una volta
fatto ciò, và calcolata per l'intensità massima, cioè 255.
*/
void cdfScalato(float cdf[], int cdfS[], int dim)
{
  for (int i=0; i<256; i++)
  {
    cdfS[i] = ((cdf[i] - cdf[0]) / (dim - 1)) * 255;
  }
}

/*
Funzione che, una volta scalati i singoli pixel, li inseriamo
nell'immagine finale, che è l'immagine stessa clonata.
Come indice del vettore cdfS[], ossia il vettore di elementi
della funzione cumulativa di distribuzione scalata, usiamo l'indice
dell'immagine stessa, quella clonata all'inizio.
*/
void elaboraImmagine(int c[], Mat des)
{
  for (int i=0; i<des.rows; i++)
  {
    for (int j=0; j<des.cols; j++)
    {
      //come indice di c si usa l'immagine originale o in clone
      des.at<uchar>(i,j) = c[(int)des.at<uchar>(i,j)];
    }
  }
}
