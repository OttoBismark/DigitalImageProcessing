/*
Filtro derivativo: Cross Operator (Roberts)

[-1  0  [0  -1
0   1]   1   0]

Filtro derivativo incrociato basato su derivata del primo ordine.
Il filtro di Roberts usa due maschere di dimensione 2x2
i cui valori sono compresi tra -1 e 1 (-1<= w(i) <= 1).
I valori assoluti delle risposte delle due maschere vengono sommati
per determinare il gradiente f nella posizione f(x,y).

*/

#include <iostream>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

/*
Prototipo della funzione per utilizzare l'operatore di Roberts
*/
void robertsOperator(Mat image);

int main(int argc, char **argv)
{
  /*
  Controllo per verificare l'argomento da passare al main
  */
  if(argv[1] == nullptr)
  {
    cerr <<"Inserire un argomento da passare al main" << endl;
    exit(-1);
  }
  else
  {
    cout << "Argomento inserito correttamente" << endl;
  }

  Mat img;

  /*Caricamento dell'immagine*/
  img = imread(argv[1], IMREAD_GRAYSCALE);

  imshow("Originale", img);
  /*Chiamata alla funzione per utilizzare l'operatore di Roberts*/
  robertsOperator(img);

  waitKey();

  return 0;
}

/*
Funzione per utilizzare l'operatore di Roberts
*/
void robertsOperator(Mat image)
{
  int i, j;
  Mat r;
  int gradx, grady;
  int magnitude = 0; /*Lunghezza del vettore*/

  /*
  Inizializzazione dell'immagine risultante da Roberts
  creando una matrice di zeri della stessa dimensione e tipo
  dell'immagine originale.
  */
  r = Mat::zeros(image.size(), image.type());

  for(i = 0 ; i < image.rows; i++)
  {
    for(j = 0; j < image.cols; j++)
    {
      gradx = abs(image.at<uchar>(i+1,j+1)-image.at<uchar>(i,j));
      grady = abs(image.at<uchar>(i+1,j)-image.at<uchar>(i,j+1));

      /*Somma dei due gradienti*/
      magnitude = gradx+grady;

      /*Se il gradiente sfora 255, equalizzare a 255*/
      if(magnitude>255)
      {
        magnitude = 255;
      }
      /*Se il gradiente è negativo, equalizzare a 0*/
      if(magnitude < 0)
      {
        magnitude = 0;
      }

      /*Aggiormento del gradiente nella nuova immagine*/
      r.at<uchar>(i,j) = magnitude;
    }
  }

  imshow("Roberts", r);
}
