#include <cstdlib>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <math.h>
#include <unistd.h>

using namespace std;
using namespace cv;

Mat distanceTransform(Mat img)
{
  Mat binaryImage;
  Mat toModify = Mat::zeros(img.size(),img.type());

  int rows = img.rows;
  int cols = img.cols;
  int valDaAssegnare;

  threshold(img,binaryImage,100.0,255.0,THRESH_BINARY);

  toModify = binaryImage.clone();

  /* Scansione dall'alto in basso */
  for(int i=1; i<rows-1; i++)
  {
    for(int j=1; j<cols-1; j++)
    {
      if(toModify.at<uchar>(i,j)!=0)
      {
        int left = toModify.at<uchar>(i,j-1);
        int up = toModify.at<uchar>(i-1,j);
        valDaAssegnare = min(left,up);
        valDaAssegnare++;

        toModify.at<uchar>(i,j) = valDaAssegnare;
      }
    }
  }

  /* Scansione dal basso all'alto */

  for(int i=rows-1; i>=1; i--)
  {
    for(int j=cols-1; j>=1; j--)
    {
      if(toModify.at<uchar>(i,j)!=0)
      {
        valDaAssegnare = toModify.at<uchar>(i,j);

        if((toModify.at<uchar>(i,j+1)+1) < valDaAssegnare)
        {
          valDaAssegnare = toModify.at<uchar>(i,j+1)+1;
        }

        if((toModify.at<uchar>(i+1,j)+1) < valDaAssegnare)
        {
          valDaAssegnare = toModify.at<uchar>(i+1,j)+1;
        }

        toModify.at<uchar>(i,j) = valDaAssegnare;
      }
    }
  }

  normalize(toModify,toModify,0,255,NORM_MINMAX);
  return toModify;
}


int main(int argc, char **argv)
{
  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main" << endl;
    exit(-1);
  }

  Mat image = imread(argv[1], 0);
  Mat outputImage = Mat::zeros(image.size(),image.type());
  if(!image.data)
  {
    cout << "Impossibile leggere l'immagine!\n" << endl;
    exit(0);
  }

  outputImage = distanceTransform(image);

  namedWindow("Original",0);
  imshow("Original",image);

  namedWindow("Distance",0);
  imshow("Distance",outputImage);

  waitKey();
  return 0;
}
