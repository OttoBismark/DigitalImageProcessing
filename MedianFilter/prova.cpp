/*
Filtro mediano
*/

#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main" << endl;
    exit(-1);
  }

  Mat image = imread(argv[1], IMREAD_GRAYSCALE);
  Mat outputImage = image.clone();
  if(!image.data)
  {
      cout << "Impossibile leggere l'immagine!\n" << endl;
      exit(0);
  }

  vector<int> pixelRegion;
  int rows = image.rows;
  int cols = image.cols;

  for(int i=1; i<image.rows-1; i++)
  {
    for(int j=1; j<image.cols-1; j++)
    {
      pixelRegion.clear();

      for(int k=-1; k<=1; k++)
      {
        for(int z=-1; z<=1; z++)
        {
          pixelRegion.push_back(image.at<uchar>(i-z,j-k));
        }
      }

      sort(pixelRegion.begin(),pixelRegion.end());
      outputImage.at<uchar>(i,j) = pixelRegion[4];
    }
  }

  namedWindow("Original",0);
  imshow("Original",image);

  namedWindow("MedianFilter",0);
  imshow("MedianFilter",outputImage);

  waitKey();
}
