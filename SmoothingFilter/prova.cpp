/*
Filtro media mobile

[1 1 1
1 1 1   X 1/9
1 1 1]


Per applicare questo filtro all'immagine originale, scorriamo riga e colonna
della cornice, cioè l'immagine, poi, nel contorno del filtro scorriamo
per ogni pixel dell'immagine (in questo caso un contorno 3x3).
Ecco spiegati i 4 for: i primi due per la cornice dell'immagine, gli altri
due for più innestati per scorrere il filtro in posizione :
-1 <= k <= 1
-1 <= w <= 1.
Con una variabile ausiliaria effettuiamo la somma dei prodotti tra
l'immagine e il filtro sulla riga e poi si procede a scorrere sulla
successiva riga, colonna per colonna.
*/

#include <iostream>
#include <math.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

float ker[3][3] = {{1/9.0,1/9.0,1/9.0},
                  {1/9.0,1/9.0,1/9.0},
                  {1/9.0,1/9.0,1/9.0},};

int main(int argc, char **argv)
{
  Mat img, img2, img3;
  int i, j;
  float aux;

  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main" << endl;
    exit(-1);
  }
  else
  {
    cout << "Immagine caricata corretatmente" << endl;
  }

  img = imread(argv[1], IMREAD_GRAYSCALE);

  imshow("Originale", img);

  img2 = Mat::zeros(img.size(), img.type());


  for(i = 1; i < img.rows-1; i++)
  {
    for(j = 1; j < img.cols-1; j++)
    {
      aux = 0.0;
      for(int k = -1; k <= 1; k++)
      {
        for(int w = -1; w <= 1; w++)
        {
          aux = aux+(ker[k+1][w+1]*img.at<uchar>(i+k, j+w));

        }
      }
      img2.at<uchar>(i,j) = aux;
    }
  }

  imshow("Modify", img2);

  waitKey();
  return 0;
}
