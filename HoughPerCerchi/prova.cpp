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
                            acc.at<double>(r-min,a,b)++;
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

void rg(Mat img, Mat output, Mat mask, vector<Point>vecPoint, vector<Vec3b> pixel, int T)
{
while(!vecPoint.empty())
{
Point p=vecPoint.back();

int x=p.x;
int y=p.y;

vecPoint.pop_back();

for(int i=-1;i<=1;i++)
{
for(int j=-1;j<=1;j++)
{
if((x+i>=0) && (x+i<img.rows))
{
if((y+j>=0) && (y+j<img.cols))
{
    for(int w=0;w<pixel.size();w++)
    {
int dist=sqrt(pow(pixel[w].val[0]-img.at<Vec3b>(x+i,y+j).val[0],2)
		+pow(pixel[w].val[1]-img.at<Vec3b>(x+i,y+j).val[1],2)
		+pow(pixel[w].val[2]-img.at<Vec3b>(x+i,y+j).val[2],2));

if(dist<T && mask.at<uchar>(x+i,y+j)==0)
{
mask.at<uchar>(x+i,y+j)=1;
output.at<Vec3b>(x+i,y+j)=pixel[w];
vecPoint.push_back(Point(x+i,y+j));
}
    }
}
}
}
}
}

}


int main(int argc, char **argv)
{
Mat img=imread(argv[1],1);
Mat output=Mat::zeros(img.size(),img.type());
Mat mask=Mat::zeros(img.size(),img.type());
    Mat canny=Mat::zeros(img.size(),img.type());

//Vec3b pixel;
    vector<Vec3b>pixel;
    
    pixel.push_back(Vec3b(22,55,237)); //sfera rossa
    pixel.push_back(Vec3b(67,196,0)); //sfera verde
    
    int seed=100,T=80,i;

vector<Point>vecPoint;

for(i=0;i<seed;i++)
{
int x=rand()%img.rows;
int y=rand()%img.cols;

if(mask.at<uchar>(x,y)==0)
{
//mask.at<uchar>(x,y)=1;
//output.at<Vec3b>(x,y)=img.at<Vec3b>(x,y);
//pixel=img.at<Vec3b>(x,y);

    
    
vecPoint.clear();

vecPoint.push_back(Point(x,y));

rg(img,output,mask,vecPoint,pixel,T);
}
else
i--;
}


    Canny(img,canny,160,200,3);
    
   vector<Vec3f>circles=hough(canny,40,55,85);
    
    for(int i=0;i<circles.size();i++)
    {
    Point center(cvRound(circles[i][2]),cvRound(circles[i][1]));
    int radius=cvRound(circles[i][0]);
    
    circle(img,center,3,Scalar(0,255,0),-1,8);
        circle(img,center,radius,Scalar(0,0,0),2,8,0);
    }
    
    
namedWindow("img",0);
imshow("img",img);

    namedWindow("output",0);
    imshow("output",output);
    
    namedWindow("canny",0);
    imshow("canny",canny);


waitKey(0);
return 0;
}

