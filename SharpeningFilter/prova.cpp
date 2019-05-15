/*
Filtro di sharpening (laplaciano)
*/

#include <iostream>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat sharpeningLaplace(Mat img)
{
  Mat toModify = img.clone();
  int rows = img.rows;
  int cols = img.cols;
  int grad;
  for(int i=1; i<rows-1; i++)
  {
    for(int j=1; j<cols-1; j++)
    {
      grad = -4*img.at<uchar>(i,j)+img.at<uchar>(i+1,j)+img.at<uchar>(i-1,j)+img.at<uchar>(i,j+1)+img.at<uchar>(i,j-1);

      if(grad > 256)
      {
        grad = 255;
      }

      if(grad < 0)
      {
        grad = 0;
      }
      toModify.at<uchar>(i,j) = grad;
    }
  }

  for(int i=0; i<rows; i++)
  {
    for(int j=0; j<cols; j++)
    {
      toModify.at<uchar>(i,j) = abs(img.at<uchar>(i,j)-toModify.at<uchar>(i,j));
    }
  }
  return toModify;
}


int main(int argc, char **argv)
{
  Mat input, output;

  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main" << endl;
    exit(-1);
  }
  else
  {
    cout << "Immagine caricata correttamente" << endl;
  }
  input = imread(argv[1], IMREAD_GRAYSCALE);

  imshow("Original",input);

  output = sharpeningLaplace(input);
  imshow("Laplace",output);

  waitKey();
}
