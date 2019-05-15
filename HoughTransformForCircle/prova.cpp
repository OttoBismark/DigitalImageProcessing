#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;


vector<Vec3f>hough(Mat img, int min, int max, int T)
{
  int radius=max-min+1;
  int rows,cols;

  rows=img.rows;
  cols=img.cols;

  int size[3]={radius,rows,cols};

  Mat acc=Mat::zeros(3,size,CV_64F);

  for(int x=0;x<img.rows;x++)
  {
      for(int y=0;y<img.cols;y++)
      {
          if(img.at<uchar>(x,y)>250)
          {
              for(int r=min;r<=max;r++)
              {
                  for(int t=0;t<=360;t++)
                  {
                      double a=abs(x-r*cos(t*CV_PI/180));
                      double b=abs(y-r*sin(t*CV_PI/180));

                      if(a<rows && b<cols)
                      {
                        acc.at<double>(r-min,a,b)++;
                      }
                  }
              }
          }
      }
  }

  vector<Vec3f>circles;

  for(int i=0;i<size[0];i++)
  {
    for(int j=0;j<size[1];j++)
    {
      for(int k=0;k<size[2];k++)
      {
          if(acc.at<double>(i,j,k)>T)
          {
              Vec3f c(i+min,j,k);
              circles.push_back(c);
          }
      }
    }
  }

  return circles;
}



int main(int argc, char **argv)
{
  if(argv[1] == nullptr)
  {
    cout << "usage ";
    cout << "./prova.exe minRadius maxRadius threshold";
    cout << endl;
    exit(-1);
  }

  int rmin, rmax, threshold;


  Mat img=imread(argv[1],IMREAD_COLOR);
  Mat output=Mat::zeros(img.size(),img.type());
  Mat canny=Mat::zeros(img.size(),img.type());

  Canny(img,canny,160,200,3);


  rmin = atoi(argv[2]);
  rmax = atoi(argv[3]);
  threshold = atoi(argv[4]);
  vector<Vec3f>circles=hough(canny,rmin,rmax,threshold);

  for(unsigned int i=0;i<circles.size();i++)
  {
    Point center(cvRound(circles[i][2]),cvRound(circles[i][1]));
    int radius=cvRound(circles[i][0]);
    circle(img,center,3,Scalar(0,255,0),-1,8);
    circle(img,center,radius,Scalar(0,0,255),2,8,0);
  }

  namedWindow("img",0);
  imshow("img",img);

  namedWindow("canny",0);
  imshow("canny",canny);

  waitKey(0);
  return 0;
}
