/*
Region Growing a colori
*/

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
#include <vector>
#include <iterator>

using namespace std;
using namespace cv;

void regionGrowingColorIterative(Mat img, vector<Point> vecPoint, Vec3b pixel, int T, Mat mask, Mat toModify)
{
    while(!vecPoint.empty())
    {
        Point P = vecPoint.back();
        int x = P.x;
        int y = P.y;

        vecPoint.pop_back();

        for(int i=-1; i<=1; i++)
        {
            for(int j=-1; j<=1; j++)
            {
                if((x+i>=0) && (x+i<img.rows))
                {
                    if((y+j>=0) && (y+j<img.cols))
                    {
                        int dist = sqrt(pow(pixel.val[0]-img.at<Vec3b>(x+i,y+j).val[0],2)+
                                        pow(pixel.val[1]-img.at<Vec3b>(x+i,y+j).val[1],2)+
                                        pow(pixel.val[2]-img.at<Vec3b>(x+i,y+j).val[2],2));

                        if(dist < T && mask.at<uchar>(x+i,y+j)==0)
                        {
                            mask.at<uchar>(x+i,y+j) = 1;
                            toModify.at<Vec3b>(x+i,y+j) = pixel;
                            vecPoint.push_back(Point(x+i,y+j));
                        }
                    }
                }
            }
        }
    }
}


void regionGrowingColor(Mat img,int x, int y, Vec3b pixel, int T, Mat mask, Mat toModify)
{
    for(int i=-1; i<=1; i++)
    {
        for(int j=-1; j<=1; j++)
        {
            if((x+i>=0) && (x+i<img.rows))
            {
                if((y+j>=0) && (y+j<img.cols))
                {
                    int dist = sqrt(pow(pixel.val[0]-img.at<Vec3b>(x+i,y+j).val[0],2)+
                                    pow(pixel.val[1]-img.at<Vec3b>(x+i,y+j).val[1],2)+
                                    pow(pixel.val[2]-img.at<Vec3b>(x+i,y+j).val[2],2));

                    if(dist < T && mask.at<uchar>(x+i,y+j)==0)
                    {
                        mask.at<uchar>(x+i,y+j) = 1;
                        toModify.at<Vec3b>(x+i,y+j) = pixel;
                        regionGrowingColor(img,x+i,y+j,pixel,T,mask,toModify);
                    }
                }
            }
        }
    }
}


int main(int argc, char **argv)
{
    if(argv[1] == nullptr && argv[2] == nullptr)
    {
      cerr << "Inserire due argomenti da passare al main" << endl;
      exit(-1);
    }

    Mat image = imread(argv[1], 1);
    Mat outputImage = Mat::zeros(image.size(),image.type());
    Mat mask = Mat::zeros(image.size(),image.type());

    int xPixel,yPixel,range=10;
    Vec3b pixel;
    vector<Point> vecPoint;
    if(!image.data)
    {
        cout << "Impossibile leggere l'immagine!\n" << endl;
        exit(0);
    }

    for(int i=0; i<atoi(argv[2]); i++)
    {
        xPixel = rand()%image.rows;
        yPixel = rand()%image.cols;
        if(mask.at<uchar>(xPixel,yPixel) == 0)
        {
            mask.at<uchar>(xPixel,yPixel) = 1;
            outputImage.at<Vec3b>(xPixel,yPixel) = image.at<Vec3b>(xPixel,yPixel);
            pixel = image.at<Vec3b>(xPixel,yPixel);
            vecPoint.clear();
            vecPoint.push_back(Point(xPixel,yPixel));
            //regionGrowingColor(image,xPixel,yPixel,pixel,range,mask,outputImage);
            regionGrowingColorIterative(image,vecPoint,pixel,range,mask,outputImage);
        }
        else
        {
            i--;
        }
    }
    namedWindow("Original",0);
    imshow("Original",image);
    namedWindow("RegionGrowing",0);
    imshow("RegionGrowing",outputImage);

    waitKey();
    return 0;
}
