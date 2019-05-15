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

void RegionGrowingF(Mat img, vector<Point> vP, Vec3b pixel, int T, Mat maschera, Mat appoggio){
    while(!vP.empty()){//fin quando il vettore non è vuoto
        Point p = vP.back();
        int x = p.x;
        int y = p.y;
        vP.pop_back();
        
        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                if((img.at<Vec3b>(i,j).val[0]<=220) && (img.at<Vec3b>(i,j).val[1]<=220) && (img.at<Vec3b>(i,j).val[2]<=220)){
                    if((x+i>=0) && (x+i<img.rows)){
                        if((y+j>=0) && (y+j<img.cols)){
                            int distanza = sqrt(pow(pixel.val[0]-img.at<Vec3b>(x+i,y+j).val[0],2) +
                                                pow(pixel.val[1]-img.at<Vec3b>(x+i,y+j).val[1],2) +
                                                pow(pixel.val[2]-img.at<Vec3b>(x+i,y+j).val[2],2));
                                if(distanza < T && maschera.at<uchar>(x+i,y+j)==0){
                                    maschera.at<uchar>(x+i,j+y) = 1;
                                    appoggio.at<Vec3b>(x+i,y+j) = pixel;
                                    vP.push_back(Point(x+i,y+j));
                                }
                            }
                        }
                    }
                }
            }
    }
}

int main(int argc, char **argv){
    Mat img = imread(argv[1],1);
    Mat img2 = Mat::zeros(img.size(),img.type());
    Mat maschera = Mat::zeros(img.size(),img.type());
    
    if(argc!=2){
        cout<<"inserisci immagine"<<endl;
        exit(0);
    }

    if(img.empty()){
        cout<<"immagine non caricata"<<endl;
        exit(0);
    }
    
    int xP,yP, T = 80, iterazioni = 1000; //con questa soglia ho l'immagine più chiara e dettagliata
    //più aumento il valore della T e più colori ho, più itero e più dettagli ho
    vector <Point> vP;
    Vec3b pixel; //pixel a colori;
    
    for(int i=0;i<iterazioni;i++){ //quante iterazioni sa da fare?!
        xP=rand()%img.rows;
        yP=rand()%img.cols;
        if(maschera.at<uchar>(xP,yP) == 0){
            maschera.at<uchar>(xP,yP) = 1;
            img2.at<Vec3b>(xP,yP) = img.at<Vec3b>(xP,yP);
            pixel = img.at<Vec3b>(xP,yP);
            vP.clear();
            vP.push_back(Point(xP,yP));
            RegionGrowingF(img,vP,pixel,T,maschera,img2);
        }
        else
            i--;
    }

    Mat img3= img2.clone();

    for(int i=0;i<img2.rows;i++){
            for(int j=0;j<img2.cols;j++){
                if((img2.at<Vec3b>(i,j).val[0]>=200) && (img2.at<Vec3b>(i,j).val[1]>=200) && (img2.at<Vec3b>(i,j).val[2]>=200)){
                   // cout<<img2.at<Vec3b>(i,j).val[0]<<" "<<img2.at<Vec3b>(i,j).val[1]<<" "<<img2.at<Vec3b>(i,j).val[2]<<endl;
                    img2.at<Vec3b>(i,j).val[0]=0;
                    img2.at<Vec3b>(i,j).val[1]=0;
                    img2.at<Vec3b>(i,j).val[2]=0;
                }
            }
    }
   
    namedWindow("Original",0);
    imshow("Original", img);
    namedWindow("RegionGrowing",0);
    imshow("RegionGrowing", img3);
    namedWindow("RegionGrowing con nero",0);
    imshow("RegionGrowing con nero", img2);
    
    waitKey();
    return 0;
}
