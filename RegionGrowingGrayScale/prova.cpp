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

void regionGrowingGreyIterative(Mat img, vector<Point> vecPoint, int pixel, int T, Mat mask, Mat toModify)
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
                    if((y+j>=0) && (y+j<img.cols))
                        if(abs(pixel-img.at<uchar>(x+i,y+j)) < T && mask.at<uchar>(x+i,y+j)==0)
                        {
                            mask.at<uchar>(x+i,y+j) = 1;
                            toModify.at<uchar>(x+i,y+j) = img.at<uchar>(x+i,y+j);
                            vecPoint.push_back(Point(x+i,y+j));
                        }
            }
        }
    }
}


void regionGrowingGrey(Mat img,int x, int y, int pixel, int T, Mat mask, Mat toModify)
{
    for(int i=-1; i<=1; i++)
    {
        for(int j=-1; j<=1; j++)
        {
            if((x+i>=0) && (x+i<img.rows))
                if((y+j>=0) && (y+j<img.cols))
                    if(abs(pixel-img.at<uchar>(x+i,y+j)) < T && mask.at<uchar>(x+i,y+j)==0)
                    {
                        mask.at<uchar>(x+i,y+j) = 1;
                        toModify.at<uchar>(x+i,y+j) = img.at<uchar>(x+i,y+j);
                        regionGrowingGrey(img,x+i,y+j,pixel,T,mask,toModify);
                    }
        }
    }
}


int main(int argc, char **argv)
{
    if(argv[1] == nullptr)
    {
      cerr << "Inserire due argomenti da passare al main" << endl;
      exit(-1);
    }
    Mat image = imread(argv[1], 0);
    Mat outputImage = Mat::zeros(image.size(),image.type());
    Mat mask = Mat::zeros(image.size(),image.type());

    int xPixel,yPixel,pixel,range;
    vector<Point> vecPoint;
    if(!image.data)
    {
        cout << "Impossibile leggere l'immagine!\n" << endl;
        exit(0);
    }

    cout << "Size immagine: " << image.size() << endl;
    //for(int i=0; i<3; i++)
    //{
        cout << "Inserisci xPixel: ";
        cin >> xPixel;
        cout << "Inserisci yPixel: ";
        cin >> yPixel;
        cout << "Inserisci la soglia: ";
        cin >> range;

        mask.at<uchar>(xPixel,yPixel) = 1;
        outputImage.at<uchar>(xPixel,yPixel) = image.at<uchar>(xPixel,yPixel);

        pixel = image.at<uchar>(xPixel,yPixel);
        vecPoint.push_back(Point(xPixel,yPixel));
        // regionGrowingGrey(image,xPixel,yPixel,pixel,range,mask,outputImage);
        regionGrowingGreyIterative(image,vecPoint,pixel,range,mask,outputImage);
    //}

    namedWindow("Original",0);
    imshow("Original",image);
    namedWindow("RegionGrowing",0);
    imshow("RegionGrowing",outputImage);

    waitKey();
    return 0;
}
