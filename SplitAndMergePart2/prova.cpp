/*
split and merge colori
*/

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

Vec3b average(Mat, int, int, int, int);
double variance(Mat, int, int, int, int, Vec3b);
Mat splitAndMerge(Mat);
void split(Mat, Mat, int, int, int, int);

int main(int argc, char const *argv[])
{
  if(argv[1] == nullptr)
  {
    cerr << "Inserire argomento" << endl;
    exit(-1);
  }

  Mat image = imread(argv[1], IMREAD_COLOR);

  imshow("Sorgente", image);

  // Split the image into different channels
  vector<Mat> rgbChannels(3);
  split(image, rgbChannels);

  // Show individual channels
  Mat g, fin_img;
  g = Mat::zeros(Size(image.cols, image.rows), CV_8UC1);

  // Showing Red Channel
  // G and B channels are kept as zero matrix for visual perception
  {
  vector<Mat> channels;
  channels.push_back(g);
  channels.push_back(g);
  channels.push_back(rgbChannels[2]);

  /// Merge the three channels
  merge(channels, fin_img);
  namedWindow("Red",1);
  imshow("Red", fin_img);
  }

  // Showing Green Channel
  {
  vector<Mat> channels;
  channels.push_back(g);
  channels.push_back(rgbChannels[1]);
  channels.push_back(g);
  merge(channels, fin_img);
  namedWindow("Green",1);
  imshow("Green", fin_img);
  }

  // Showing Blue Channel
  {
  vector<Mat> channels;
  channels.push_back(rgbChannels[0]);
  channels.push_back(g);
  channels.push_back(g);
  merge(channels, fin_img);
  namedWindow("Blue",1);
  imshow("Blue", fin_img);
  }

  waitKey(0);
}
