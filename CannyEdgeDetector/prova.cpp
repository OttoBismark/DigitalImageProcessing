/*
Algoritmo di Canny

1) Sottoporre l'immagine a smoothing tramite filtro gaussiano
2) Calcolare la magnitudo e l'angolo del gradiente
3) Applicare la non maxima suppression all'immagine del gradiente
4) Utilizzare la doppia soglia e la connettività per individuare
  e collegare gli edge

*/

#include <iostream>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat CannyEdgeDetection(Mat image);

/*
Filtro gaussiano

[1 2 1
2 4 2
1 2 1]
*/
float ker[3][3]=
{
  {1.0/16.0,2.0/16.0,1.0/16.0},
  {2.0/16.0,4.0/16.0,2.0/16.0},
  {1.0/16.0,2.0/16.0,1.0/16.0},
};


int main(int argc, char ** argv)
{
  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main" << endl;
    exit(-1);
  }
  else
  {
    cout << "Argomento caricato correttamente" << endl;
  }

  Mat img, c;

  img = imread(argv[1], IMREAD_GRAYSCALE);
  imshow("Originale", img);

  /*Creazione di una matrice di zeri per poter ricevere l'output
  dell'algoritmo di Canny*/
  c = Mat::zeros(img.size(), img.type());

  /*Chiamata alla funzione per effettuare l'algoritmo di Canny*/
  c = CannyEdgeDetection(img);

  imshow("Algoritmo di Canny", c);
  waitKey();
  return 0;
}

/*Funzione CannyEdgeDetection per effettuare l'algoritmo di Canny*/
Mat CannyEdgeDetection(Mat image)
{
  int i, j, u, v, row, column;
  float dx, dy, grad, theta;
  float aux, degree;
  Mat img2;

  row = image.rows;
  column = image.cols;

  /*
  Matrice delle 'theta' che conterrà gli angoli del gradiente
  compreso tra 0 <= theta <= 135
  */
  int thetaMatrix[row][column] = {};

  img2 = image.clone();

  for(i = 1; i < image.rows-1; i++)
  {
    for(j = 1; j < image.cols-1; j++)
    {
      aux = 0.0;

      for(u = -1; u <= 1; u++)
      {
        for(v = -1; v <= 1;v++)
        {
          /*Convoluzione del filtro di gauss all'immagine*/
          aux = aux+(ker[u+1][v+1]*image.at<uchar>(i+u, j+v));
        }
      }

      img2.at<uchar>(i,j) = aux;
    }
  }

  /*
  Matrice di appoggio uguale alla matrice calcolata con il filtro di
  smoothing di Gauss
  */
  Mat apg = img2.clone();

  for(i = 0; i < img2.rows; i++)
  {
    for(j = 0; j < img2.cols; j++)
    {
      /*Calcolo del gradiente su X e su Y attraverso l'operatore di Sobel*/
      dx = abs((img2.at<uchar>(i,j)+ 2*img2.at<uchar>(i+1,j)+img2.at<uchar>(i+2, j)) -
      (img2.at<uchar>(i, j+2) + 2*img2.at<uchar>(i+1, j+2) + img2.at<uchar>(i+2, j+2)));
      dy = abs((img2.at<uchar>(i,j)+ 2*img2.at<uchar>(i,j+1)+img2.at<uchar>(i, j+2)) -
      (img2.at<uchar>(i+2, j) + 2*img2.at<uchar>(i+2, j+1) + img2.at<uchar>(i+2, j+2)));

      /*Somma dei gradienti*/
      grad = dx + dy;

      if(grad > 255)
      {
        grad = 255;
      }

      if(grad < 0)
      {
        grad = 0;
      }

      /*Aggiornamento del nuovo valore di intensità*/
      apg.at<uchar>(i,j) = grad;
      /*Calcolo dell'angolo del gradiente tramite arctan(dy/dx)*/
      theta = atan2(dy,dx);
      degree = (theta*180) / M_PI;

      /* Se la direzione del gradiente e verso gli 0° */
      if((degree > -22.5 && degree <= 22.5) ||
        (degree > 157.5 && degree <= -157.5))
      {
        degree = 0;
      }
      /* Se la direzione del gradiente è verso i 45° */
      else if((degree > 22.5 && degree <= 67.5) ||
              (degree > -157.5 && degree <= -112.5))
      {
        degree = 45;
      }
      /* Se la direzione del gradiente è verso i 90° */
      else if((degree > 67.5 && degree <= 112.5) ||
              (degree > -112.5 && degree <= -67.5))
      {
        degree = 90;
      }
      /* Se la direzione del gradiente è verso i 135° */
      else if((degree > 112.5 && degree <= 157.5) ||
              (degree > -67.5 && degree <= -22.5))
      {
        degree = 135;
      }

      thetaMatrix[i][j] = degree;
    }
  }


  /*
  Soppressione dei non massimi.
  */
  for(i = 1; i < row-1; i++)
  {
    for(j = 1; j < column-1; j++)
    {
      int N = apg.at<uchar>(i-1, j);
      int S = apg.at<uchar>(i+1, j);
      int W = apg.at<uchar>(i,j-1);
      int E = apg.at<uchar>(i,j+1);
      int NW = apg.at<uchar>(i-1, j-1);
      int NE = apg.at<uchar>(i-1, j+1);
      int SE = apg.at<uchar>(i+1, j-1);
      int SW = apg.at<uchar>(i+1, j+1);
      int choice = thetaMatrix[i][j];

      switch (choice)
      {
        case 0:
          if(apg.at<uchar>(i,j) < E || apg.at<uchar>(i, j) < W)
          {
            apg.at<uchar>(i,j) = 0;
          }
        break;

        case 45:
          if(apg.at<uchar>(i,j) < NW || apg.at<uchar>(i, j) < SE)
          {
            apg.at<uchar>(i,j) = 0;
          }
        break;

        case 90:
          if(apg.at<uchar>(i,j) < N || apg.at<uchar>(i, j) < S)
          {
            apg.at<uchar>(i,j) = 0;
          }
        break;

        case 135:
          if(apg.at<uchar>(i,j) < NE || apg.at<uchar>(i, j) < SW)
          {
            apg.at<uchar>(i,j) = 0;
          }
        break;
      }
    }
  }

  /*
  Sogliatura per isteresi
  Valori di soglia (Thresholding)
  */
  int t1 = 150;
  int t2 = 70;

  /*Matrice finale di tutta l'operazione algoritmica*/
  Mat cannyImage = Mat::zeros(image.size(), image.type());

  for(i = 1; i < row-1; i++)
  {
    for(j = 1; j < column-1; j++)
    {
      if(apg.at<uchar>(i,j) < t2)
      {
        cannyImage.at<uchar>(i,j) = 0;
      }
      else if(apg.at<uchar>(i,j) > t1)
      {
        cannyImage.at<uchar>(i,j) = 255;
      }
      else if(apg.at<uchar>(i,j) >= t2 && apg.at<uchar>(i,j) <= t1)
      {
        for(int x = -1; x <= 1; x++)
        {
          for(int y = -1; y <= 1; y++)
          {
            if(apg.at<uchar>(i+x, j+y) > t1)
            {
              cannyImage.at<uchar>(i,j) = 255;
            }
          }
        }
      }
    }
  }
  return cannyImage;
}
