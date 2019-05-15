/*
Fitro derivativo: Operatore di Prewitt

[1  1  1    [1   0  -1]
 0  0  0     1   0  -1
-1 -1 -1]    1   0  1]

L'operatore di Prewitt utilizza una maschera di dimensione 3x3, a differenza
di Roberts che utilizza una 2x2 con operatore a incrocio (Cross Operation).
Questo operatore ci permette di accentuare i bordi in quanto è meno
sensibile alla variazione della luce. Il funzionamento prevede di sottrarre
ai valori nella maschera in basso con quelli in alto (ricordando che, nelle
immagini l'origine è in alto a sinistra; lungo l'asse orizzontale troviamo le
Y e in verticale le X).
Anche questo operatore è basato sul gradiente, in grado di effettuare
simultaneamente la differenziazione lungo le due direzioni e una media
spaziale lungo la direzione ortogonale, che riduce la sensibilità al rumore.*/



#include <iostream>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

/*prototipo della funzione "operatorePrewitt"
per utlizzare l'operatore di Prewitt*/
void operatorePrewitt(Mat image);

int main(int argc, char **argv)
{
  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da dare al main"<< endl;
    exit(-1);
  }
  else
  {
    cout << "Argomento caricato correttamente"<< endl;
  }

  Mat img;

  img = imread(argv[1], IMREAD_GRAYSCALE);

  imshow("Originale", img);

  /*Chiamata alla funzione operatorePrewitt*/
  operatorePrewitt(img);
  waitKey();
  return 0;
}

/*
Funzione "operatorePrewitt" per ottenere i contorni dell'immagine
*/
void operatorePrewitt(Mat image)
{
  int i, j;
  int Gx, Gy;
  int magnitude;
  Mat r;

  r = Mat::zeros(image.size(), image.type());

  for(i = 0; i < image.rows; i++)
  {
    for(j = 0; j < image.cols; j++)
    {
      Gx = abs((image.at<uchar>(i,j)+ image.at<uchar>(i+1,j)+image.at<uchar>(i+2, j)) -
      (image.at<uchar>(i, j+2) + image.at<uchar>(i+1, j+2) + image.at<uchar>(i+2, j+2)));
      Gy = abs((image.at<uchar>(i,j)+ image.at<uchar>(i,j+1)+image.at<uchar>(i, j+2)) -
      (image.at<uchar>(i+2, j) + image.at<uchar>(i+2, j+1) + image.at<uchar>(i+2, j+2)));
      magnitude = Gx +Gy;

      if(magnitude > 255)
      {
        magnitude = 255;
      }

      if(magnitude < 0)
      {
        magnitude = 0;
      }

      r.at<uchar>(i,j) = magnitude;
    }
  }
  imshow("Prewitt", r);
}
