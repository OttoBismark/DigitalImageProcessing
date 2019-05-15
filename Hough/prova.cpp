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
  int nrho, dist, ntheta;
  if(argv[1] == nullptr && argv[2] == nullptr)
  {
    cerr << "Error! Usage ";
    cerr << "Usage = ./executable image.ext [threshold] "<< endl;
    return -1;
  }

  vector<float> vsin;
  vector<float> vcosin;
  vector<int> vecRho;
  vector<int> vecTheta;

  int threshold = atoi(argv[2]);
  Mat img = imread(argv[1], IMREAD_COLOR);
  Mat canny;
  Canny(img, canny, 75, 80, 3);

  int rows = canny.rows;
  int cols = canny.cols;

  ntheta = 180;

  dist = sqrt(pow(rows, 2) + pow(cols, 2));

  nrho = 2*ceil(dist)-1;

  // cout << "nrho -> "<< nrho << endl;

  for(int t = -90; t < 90; t++)
  {
    vsin.push_back((t*M_PI)/180.0);
    vcosin.push_back((t*M_PI)/180.0);
  }

  for(int rho = -nrho/2; rho < nrho/2; rho++)
  {
    vecRho.push_back(rho);
  }

  for(int tempTheta = -ntheta/2; tempTheta < ntheta/2; tempTheta++)
  {
    vecTheta.push_back(tempTheta);
  }

  int a[nrho][ntheta];

  for(int i = 0; i < nrho; i++)
  {
    for(int j = 0; j < ntheta; j++)
    {
      a[i][j] = 0;
    }
  }


  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      if(canny.at<uchar>(i,j) > 250)
      {
        for(int t = 0; t < 180; t++)
        {
          int r = abs((round((double)i*cos(vcosin[t]) + (double)j*sin(vsin[t]))));
          // cout << "r -> " << r << endl;

          a[r][t]++;
        }
      }
    }
  }

  vector< pair< int, int> > peak;

  int max = 0;

  for(int i = 0; i < nrho; i++)
  {
    for(int j = 0 ; j < ntheta; j++)
    {
      if(a[i][j] > threshold)
      {
        max = a[i][j];
        peak.push_back(pair<int, int>(i,j));
      }
    }
  }

  reverse(peak.begin(), peak.end());
  for(int w = 0; w < peak.size(); w++)
  {
    double a = cos(peak[w].second*M_PI/180);
    double b = sin(peak[w].second*M_PI/180);
    double x0 = a*peak[w].first;
    double y0 = b*peak[w].first;
    double x1 = round(x0 +1000 *-b);
    double y1 = round(y0 +1000 * a);
    double x2 = round(x0 -1000 *-b);
    double y2 = round(y0 -1000 * a);
    line(img, Point(x1,y1),Point(x2,y2), Scalar(0,0,255), 1, 8);
  }


  // for(int k = 0; k < peak.size(); k++)
  // {
  //   cout << "X ->" << peak[k].first << " Y -> " << peak[k].second;
  //   line(img, Point(peak[k].first),Point(peak[k].second), Scalar(0,0,255), 2, 8);
  //   cout << "\n";
  // }

  imshow("Original Image.", img);
  imshow("Canny Opencv.", canny);
  waitKey();
}
