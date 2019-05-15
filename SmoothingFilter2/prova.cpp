/*
Filtro di media ponderata

*/

#include <iostream>
#include <math.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

float ker[3][3] =
{
  {0.062, 0.125, 0.062},
  {0.125, 0.25, 0.125},
  {0.062, 0.125, 0.062},
};

int main(int argc, char **argv)
{
  Mat img, img2;
  int i, j;
  float aux;

  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main" << endl;
    exit(-1);
  }
  else
  {
    cout << "Immagine caricata correttamente " << endl;
  }

  for(i = 0; i < 3; i++)
  {
    for(j = 0; j < 3; j++)
    {
      cout << ker[i][j] << " ";
    }
    cout << endl;
  }

  img = imread(argv[1], IMREAD_GRAYSCALE);

  imshow("originale", img);

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

  imshow("Media ponderata", img2);
  waitKey();
  return 0;
}
