/*
Trasformazione logaritmica di un'immagine
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
  Mat img, img2;

  if(argv[1] == nullptr)
  {
    cerr << "Errore, inserire un argomento al main" << endl;
    exit(-1);
  }
  else
  {
    cout << "Immagine caricata correttamente" << endl;
  }

  img = imread(argv[1], IMREAD_GRAYSCALE);

  imshow("No Logarithm", img);
  /*
  Creazione della matrice di zeri della stessa grandezza dell'originale
  e dello stesso tipo
  */
  img2 = Mat::zeros(img.size(), img.type());

  /*
  2 for innestati per scorrere l'immagine punto punto e applicare il
  logaritmo
  */
  for(int i = 0; i < img.rows; i++)
  {
    for(int j = 0; j < img.cols; j++)
    {
      img2.at<uchar>(i, j) = (255/log(256))*log(1+img.at<uchar>(i,j));
    }
  }

  imshow("Logarithm", img2);

  waitKey();
  return 0;
}
