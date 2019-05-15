/*
Correzione di Gamma
*/

#include <iostream>
#include <math.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  if(argv[1] == nullptr)
  {
    cerr << "Errore! Inserire un argomento al main" << endl;
    exit(-1);
  }
  else
  {
    cout << "Immagine caricata correttamente" << endl;
  }

  Mat src, dest;

  src = imread(argv[1], IMREAD_GRAYSCALE);
  /*Conversione di valori dall'immagine di partenza in 32 floating point*/
  src.convertTo(dest, CV_32F);

  imshow("Normale", src);

  /*Divisione del negativo per 255 in modo di ottenere valori compresi tra
  0 e 1
  */
  dest = dest/255;

  pow(dest, 2, dest);

  /*Operazione inversa per normalizzare i valori dei bit*/
  dest = dest*255;

  /*Applicazione del valore assoluto all'intera matrice*/
  convertScaleAbs(dest, dest);
  /*Normalizzazione di tutti i valori compresi tra 0 e 255*/
  normalize(dest, dest, 0, 255, NORM_MINMAX);

  imshow("Gamma", dest);
  waitKey();
  return 0;
}
