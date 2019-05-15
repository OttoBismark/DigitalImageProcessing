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
  Mat input=imread(argv[1],1);
  namedWindow("input",0);
  imshow("input",input);

  int type=input.type();
  //input.convertTo(input,CV_32S);
  Mat splitandmerge = splitAndMerge(input);
  //imwrite("splitandmerge.jpg",splitandmerge);
	//Mat ciccio=imread("splitandmerge.jpg",1);
  namedWindow("out",0);
  imshow("out",splitandmerge);
  waitKey(0);
  return 0;
}

Mat splitAndMerge(Mat input){
  Mat output = Mat(input.rows, input.cols, input.type(),Scalar(255));
  split(input,output,0,input.rows-1,0,input.cols-1);
  return output;
}

void split(Mat input, Mat output, int startRowIndex, int endRowIndex, int startColIndex, int endColIndex){
  Vec3b media = average(input,startRowIndex,endRowIndex,startColIndex,endColIndex);
  double varianza = variance(input, startRowIndex, endRowIndex, startColIndex, endColIndex, media);
  double threshold = 30.0;
  //Caso base, non è piu possibile dividere
  if( varianza<=threshold || (endRowIndex-startRowIndex)<2 || (endColIndex - startColIndex)<2 ){
    for(int i=startRowIndex;i<=endRowIndex;i++){
      for(int j=startColIndex;j<=endColIndex;j++){
        output.at<Vec3b>(i,j).val[0]=media.val[0];
        output.at<Vec3b>(i,j).val[1]=media.val[1];
        output.at<Vec3b>(i,j).val[2]=media.val[2];
      }
    }
  }
  else{
    //Calcolo righe e colonne ed effettuo 4 chiamate ricorsive
    int rowVal = (endRowIndex-startRowIndex)/2;
    int colVal = (endColIndex - startColIndex)/2;
    split(input, output,startRowIndex, startRowIndex+rowVal, startColIndex, startColIndex+colVal);//su-sx
    split(input, output,startRowIndex, startRowIndex+rowVal, startColIndex+colVal+1, endColIndex);//su-dx
    split(input, output,startRowIndex+rowVal+1, endRowIndex, startColIndex, startColIndex+colVal);//giu-sx
    split(input, output,startRowIndex+rowVal+1, endRowIndex, startColIndex+colVal+1, endColIndex);//giu-dx

  }

}


Vec3b average(Mat input, int startRowIndex, int endRowIndex, int startColIndex, int endColIndex){
  Vec3b output;
  int val0=0,val1=0,val2=0;
  int denom=0;
  for(int i=startRowIndex; i<=endRowIndex; i++){
    for(int j=startColIndex; j<=endColIndex; j++){
      val0+=input.at<Vec3b>(i,j).val[0];
      val1+=input.at<Vec3b>(i,j).val[1];
      val2+=input.at<Vec3b>(i,j).val[2];
      denom++;
    }
  }
  val0/=denom;
  val1/=denom;
  val2/=denom;
  output.val[0]=val0;
  output.val[1]=val1;
  output.val[2]=val2;
  return (output);
}


double variance(Mat input, int startRowIndex, int endRowIndex, int startColIndex, int endColIndex, Vec3b average){
  long cumSum = 0;
  int denom = 0;
  for(int i=startRowIndex; i<=endRowIndex; i++){
    for(int j=startColIndex; j<=endColIndex; j++){
    /*
    int dist=sqrt(pow(average.val[0]-image.at<Vec3b>(i,j).val[0],2)+
								pow(average.val[1]-image.at<Vec3b>(i,j).val[1],2)+
								pow(average.val[2]-image.at<Vec3b>(i,j).val[2],2));
    */
      int deviation= sqrt(pow(average.val[0]-input.at<Vec3b>(i,j).val[0],2)+
    								pow(average.val[1]-input.at<Vec3b>(i,j).val[1],2)+
    								pow(average.val[2]-input.at<Vec3b>(i,j).val[2],2));
      cumSum += pow(deviation,2);
      denom ++;
    }
  }
  double output = sqrt( (double)cumSum/(denom-1) );
  return output;
}
