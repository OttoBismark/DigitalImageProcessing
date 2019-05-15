/*
Sottrazione dello sfondo
*/
#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

void removeBackground(Mat image1, Mat image2, Mat image3, int n);

int main(int argc, char **argv)
{
  if(argv[1] == nullptr &&
    argv[2] == nullptr &&
    argv[3] == nullptr)
  {
    cerr <<"Inserire 3 immagini come tre argomenti" << endl;
    exit(-1);
  }

  Mat img1, img2, img3;

  img1 = imread(argv[1], IMREAD_GRAYSCALE);
  img2 = imread(argv[2], IMREAD_GRAYSCALE);
  img3 = imread(argv[3], IMREAD_GRAYSCALE);

  removeBackground(img1, img2, img3, argc);

  waitKey();
  return 0;
}

void removeBackground(Mat image1, Mat image2, Mat image3, int n)
{
  Mat bg = (image1+image2+image3)/n;
  Mat mask = Mat::zeros(image1.size(), image1.type());

  Mat out;

  int row = image1.rows;
  int col = image1.cols;

  for(int i = 0; i < row; i++)
  {
    for(int j = 0; j < col; j++)
    {
      if(abs(image2.at<uchar>(i,j)-bg.at<uchar>(i,j)) > 45)
      {
        mask.at<uchar>(i,j) = 255;
      }
    }
  }

  Mat bg1 = (bg+image3)/n-1;
  Mat mask1 = Mat::zeros(image2.size(), image2.type());

  for(int i = 0; i < row; i++)
  {
    for(int j = 0; j < col; j++)
    {
      if(abs(image3.at<uchar>(i,j)-bg1.at<uchar>(i,j)) > 22)
      {
        mask1.at<uchar>(i,j) = 255;
      }
    }
  }

  imshow("Sfondo 0", bg);
  imshow("Sfondo 1", bg1);
  imshow("Maschera 0", mask);
  imshow("Maschera 1", mask1);
}
