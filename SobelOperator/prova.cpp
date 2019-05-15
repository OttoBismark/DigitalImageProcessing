/*
Filtro derivativo: operatore di operatoreSobel

[1    2    1     [1   0   -1]
0     0    0      2   0   -2
-1   -2   -1]     1   0   -1]

*/

#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void operatoreSobel(Mat image);

int main(int argc, char **argv)
{
  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main"<< endl;
    exit(-1);
  }
  else
  {
    cout << "Argomento inserito correttamente "<< endl;
  }

  Mat img;

  img = imread(argv[1], IMREAD_GRAYSCALE);
  imshow("Originale", img);

  operatoreSobel(img);
  waitKey();
  return 0;
}

void operatoreSobel(Mat image)
{
  int i, j;
  int magnitude;
  Mat r;
  int Gx, Gy;

  r = Mat::zeros(image.size(), image.type());

  for(i = 0; i < image.rows; i++)
  {
    for(j = 0; j < image.cols; j++)
    {
      Gx = abs((image.at<uchar>(i,j)+ 2*image.at<uchar>(i+1,j)+image.at<uchar>(i+2, j)) -
      (image.at<uchar>(i, j+2) + 2*image.at<uchar>(i+1, j+2) + image.at<uchar>(i+2, j+2)));
      Gy = abs((image.at<uchar>(i,j)+ 2*image.at<uchar>(i,j+1)+image.at<uchar>(i, j+2)) -
      (image.at<uchar>(i+2, j) + 2*image.at<uchar>(i+2, j+1) + image.at<uchar>(i+2, j+2)));
      magnitude = Gx +Gy;

      if(magnitude > 255)
      {
        magnitude = 255;
      }

      if(magnitude < 0)
      {
        magnitude = 0;
      }

      r.at<uchar>(i, j) = magnitude;
    }
  }

  imshow("Operatore di Sobel", r);
}
