#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  int dx, dy;
  int grad;
  float theta;
  int degree;

  if(argv[1] == nullptr)
  {
    cerr << "Usage ";
    cerr << " ./file.exe image.format.\n" << endl;
    return -1;
  }

  Mat img = imread(argv[1], IMREAD_GRAYSCALE);
  Mat apg = Mat::zeros(img.size(), img.type());

  if(!img.data)
  {
    cerr << "Impossibile aprire l'immagine." << endl;
    return -1;
  }


  int rows = img.rows;
  int cols = img.cols;
  int thetaMatrix[rows][cols] = {};
  pair<int, int > harris;

  for(int i = 0; i < img.rows; i++)
  {
    for(int j = 0; j < img.cols; j++)
    {
      int ixx, iyy, ixy = 0;
      /*Calcolo del gradiente su X e su Y attraverso l'operatore di Sobel*/
      dx = abs((img.at<uchar>(i,j)+ 2*img.at<uchar>(i+1,j)+img.at<uchar>(i+2, j)) -
      (img.at<uchar>(i, j+2) + 2*img.at<uchar>(i+1, j+2) + img.at<uchar>(i+2, j+2)));
      dy = abs((img.at<uchar>(i,j)+ 2*img.at<uchar>(i,j+1)+img.at<uchar>(i, j+2)) -
      (img.at<uchar>(i+2, j) + 2*img.at<uchar>(i+2, j+1) + img.at<uchar>(i+2, j+2)));

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

      /*
      Calcolo delle derivative del grandiente secondo l'operatore di Sobel
      verso x (Dx) e verso y (Dy)
      */
      ixx = pow(dx, 2);
      iyy = pow(dy, 2);

      harris = make_pair(ixx, iyy);

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
  for(int i = 1; i < rows-1; i++)
  {
    for(int j = 1; j < cols-1; j++)
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
  int t1 = 220;
  int t2 = 135;

  /*Matrice finale di tutta l'operazione algoritmica*/
  Mat cannyImage = Mat::zeros(img.size(), img.type());

  for(int i = 1; i < rows-1; i++)
  {
    for(int j = 1; j < cols-1; j++)
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


  for(int i = 0; i < rows; i++)
  {
    int sumX = 0;
    int sumY = 0;
    int sumXY = 0;

    for(int j = 0; j < cols; j++)
    {
      int ixy = 0;
      ixy = harris.first * harris.second;
      sumX = sumX + harris.first;
      sumY = sumY + harris.second;
      sumXY = sumXY + ixy;

      int det = (sumX * sumY) - (pow(sumXY,2));
      int trace = sumX + sumY;
      int R = det - 0.04*(pow(trace,2));

      if(R > 40)
      {
        circle(img, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
      }
    }
  }



  imshow("Image", img);
  imshow("Canny Image", cannyImage);
  waitKey();
}
