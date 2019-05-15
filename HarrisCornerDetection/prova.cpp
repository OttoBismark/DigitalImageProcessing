/*

Harris corner detector

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
#include <algorithm>
#include <iterator>

using namespace std;
using namespace cv;

struct Autovalore
{
    int x;
    int y;
    int lambda;
};

typedef struct Autovalore AUTOVALORE;

Mat HarrisCorner(Mat img)
{
    Mat toModify = img.clone();
    int rows = img.rows;
    int cols = img.cols;
    int Gx,Gy;
    int soglia = 140;
    vector<AUTOVALORE> pixelVector;
    /* Calcolo i gradienti nelle due direzioni e */
    for(int i=3; i<rows-3; i++)
    {
        for(int j=3; j<cols-3; j++)
        {
            Mat C = Mat::zeros(2,2,CV_32F);
            vector<int> eigenvalues;
            for(int x=-3; x<=3; x++)
            {
                for(int y=-3; y<=3; y++)
                {
                    Gx = abs(img.at<uchar>(x+i+1,y+j+1)-img.at<uchar>(x+i,y+j));
                    Gy = abs(img.at<uchar>(x+i+1,y+j)-img.at<uchar>(x+i,y+j+1));
                    C.at<float>(0,0) += Gx*Gx;
                    C.at<float>(1,1) += Gy*Gy;
                    C.at<float>(0,1) += Gx*Gy;
                    C.at<float>(1,0) += Gx*Gy;
                }
            }
            eigen(C,eigenvalues);
            int minimo = min(eigenvalues[0],eigenvalues[1]);
            if(minimo > soglia)
            {
                AUTOVALORE V = {i,j,minimo};
                pixelVector.push_back(V);
            }
        }
    }

    //ordinamento
    for(int i=1;i<pixelVector.size();i++)
    {
        AUTOVALORE V = pixelVector[i];
        int j = i-1;
        while(j>=0 && pixelVector[j].lambda < V.lambda)
            pixelVector[j+1]=pixelVector[j--];
        pixelVector[j+1] = V;
    }
    for(int i=0;i<pixelVector.size();i++)
        cout << "Valore = "<< pixelVector[i].lambda<<endl;
    int cont = 0;

    for(int i=0;i<pixelVector.size();i++)
    {
        for(int j=i+1;j<pixelVector.size();j++)
        {
            if(pixelVector[j].x>=pixelVector[i].x-1 && pixelVector[j].x<=pixelVector[i].x+1 &&
                pixelVector[j].y>=pixelVector[i].y-1 && pixelVector[j].y>=pixelVector[i].y+1)
            {
                pixelVector.erase(pixelVector.begin()+j);
                cont++;
            }
        }
    }

    cout << cont;
    for(int i=0;i<pixelVector.size();i++)
    {
        Point center(cvRound(pixelVector[i].y),cvRound(pixelVector[i].x));
        circle(toModify,center,1,Scalar(255,0,255),1,8,0);
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
    Mat image = imread(argv[1], 0);
    Mat outputImage = Mat::zeros(image.size(),image.type());
    //Mat openCvHarris;
    if(!image.data)
    {
        cout << "Impossibile leggere l'immagine!\n" << endl;
        exit(0);
    }

    outputImage = HarrisCorner(image);

    namedWindow("Original",0);
    imshow("Original",image);

    namedWindow("Harris",0);
    imshow("Harris",outputImage);

    //namedWindow("openCvHarris",0);
    //imshow("openCvHarris",openCvHarris);

    waitKey();
    return 0;
}
