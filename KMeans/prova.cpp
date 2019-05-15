#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

Mat kMeans(Mat img,int K,int tentativi,int soglia)
{
  Mat toModify = img.clone();
  int rows = img.rows;
  int cols = img.cols;
  vector<Vec3b> oldPoints;
  vector<Vec3b> newPoints;
  vector<Point> clusters[K];

  for(int i=0; i<K; i++)
  {
    int x = rand()%rows;
    int y = rand()%cols;
    Vec3b V = img.at<Vec3b>(x,y);
    oldPoints.push_back(V);
  }

  int n = 0;
  bool finito = 0;
  while(!finito && n<=tentativi)
  {
    cout << "Tentativo numero: "<< n+1 << endl;

    for(int i=0; i<K; i++)
    {
      clusters[i].clear();
    }

    for(int i=0; i<rows; i++)
    {
      for(int j=0; j<cols; j++)
      {
        int indice;
        int minimo = INT_MAX;
        for(int k=0; k<K; k++)
        {
          int distanza = sqrt(pow(img.at<Vec3b>(i,j).val[0]-oldPoints[k].val[0],2) +
                          pow(img.at<Vec3b>(i,j).val[1]-oldPoints[k].val[1],2) +
                          pow(img.at<Vec3b>(i,j).val[2]-oldPoints[k].val[2],2));

          if(distanza < minimo)
          {
            minimo = distanza;
            indice = k;
          }
        }
        Point p = Point(i,j);
        clusters[indice].push_back(p);
      }
    }

    for(int i=0; i<K; i++)
    {
      Vec3b V;
      int val0 = 0;
      int val1 = 0;
      int val2 = 0;

      for(int j=0 ; j<clusters[i].size(); j++)
      {
        int x = clusters[i][j].x;
        int y = clusters[i][j].y;

        val0 += img.at<Vec3b>(x,y).val[0];
        val1 += img.at<Vec3b>(x,y).val[1];
        val2 += img.at<Vec3b>(x,y).val[2];
      }

      V.val[0] = val0/clusters[i].size();
      V.val[1] = val1/clusters[i].size();
      V.val[2] = val2/clusters[i].size();

      newPoints.push_back(V);
    }

    finito = 1;

    for(int i=0; i<K; i++)
    {
      int distance = sqrt(pow(newPoints[i].val[0]-oldPoints[i].val[0],2) + pow(newPoints[i].val[1]-oldPoints[i].val[1],2) + pow(newPoints[i].val[2]-oldPoints[i].val[2],2));

      if(distance > soglia)
      {
          oldPoints = newPoints;
          newPoints.clear();
          finito = 0;
          break;
      }
    }
    n++;

    }

    for(int i=0; i<K; i++)
    {
      for(int j=0; j<clusters[i].size(); j++)
      {
          toModify.at<Vec3b>(clusters[i][j].x,clusters[i][j].y).val[0] = newPoints[i].val[0];
          toModify.at<Vec3b>(clusters[i][j].x,clusters[i][j].y).val[1] = newPoints[i].val[1];
          toModify.at<Vec3b>(clusters[i][j].x,clusters[i][j].y).val[2] = newPoints[i].val[2];
      }
    }

  return toModify;
}

int main(int argc, char **argv)
{
  if(argv[1] == nullptr)
  {
    cerr << "Inserire un argomento da passare al main" << endl;
    exit(-1);
  }

  Mat image = imread(argv[1], 1);
  Mat outputImage = Mat::zeros(image.size(),image.type());
  if(!image.data)
  {
    cout << "Impossibile leggere l'immagine!\n" << endl;
    exit(0);
  }
  int k = atoi(argv[2]);
  int tentativi = atoi(argv[3]);
  int soglia = atoi(argv[4]);

  srand((unsigned int)time(NULL));
  outputImage = kMeans(image,k,tentativi,soglia);

  namedWindow("Original",0);
  imshow("Original",image);
  namedWindow("kMeans",0);
  imshow("kMeans",outputImage);

  waitKey();
  return 0;
}
