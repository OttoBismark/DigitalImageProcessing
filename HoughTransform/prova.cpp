#include <iostream>
#include <cmath>
#include <utility>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define DEG2RAD 0.017459323f

using namespace std;
using namespace cv;

int houghTransform(unsigned char *image, int r, int c);

const unsigned int *getAccu(int *, int *);

std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > getLines(int threshold);

unsigned int *accumulator;
int _accu_h;
int _accu_w;
int _img_w;
int _img_h;

int main(int argc, char **argv)
{
  Mat image, im_blur, im_canny;
  int threshold = atoi(argv[2]);

  image = imread(argv[1], 1);

  if(!image.data)
  {
    cerr << "Immagine inesistente" << endl;
    exit(-1);
  }

  blur(image, im_blur, Size(5,5));
  Canny(im_blur, im_canny, 100, 150, 3);

  imshow("Original", image);
  imshow("Gaussian blur", im_blur);
  imshow("Canny Edge", im_canny);

  int righe = image.rows;
  int colonne = image.cols;

  houghTransform(im_canny.data, righe, colonne);



  if(threshold == 0)
    threshold = righe>colonne?righe/4:colonne/4;

  while(1)
  {
    Mat img_res = image.clone();

    //Search the accumulator
    std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > lines = getLines(threshold);

    //Draw the results
    std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > >::iterator it;
    for(it=lines.begin();it!=lines.end();it++)
    {
      cv::line(img_res, cv::Point(it->first.first, it->first.second), cv::Point(it->second.first, it->second.second), cv::Scalar( 0, 0, 255), 2, 8);
    }

    //Visualize all
    int aw, ah, maxa;
    aw = ah = maxa = 0;
    const unsigned int* accu = getAccu(&aw, &ah);

    for(int p=0;p<(ah*aw);p++)
    {
      if((int)accu[p] > maxa)
        maxa = accu[p];
    }
    double contrast = 1.0;
    double coef = 255.0 / (double)maxa * contrast;

    cv::Mat img_accu(ah, aw, CV_8UC3);
    for(int p=0;p<(ah*aw);p++)
    {
      unsigned char c = (double)accu[p] * coef < 255.0 ? (double)accu[p] * coef : 255.0;
      img_accu.data[(p*3)+0] = 255;
      img_accu.data[(p*3)+1] = 255-c;
      img_accu.data[(p*3)+2] = 255-c;
    }


    imshow("Result", img_res);
    imshow("Canny Edge Detection", im_canny);
    imshow("Accumulator", img_accu);

    char c = cv::waitKey(360000);
    if(c == '+')
      threshold += 5;
    if(c == '-')
      threshold -= 5;
    if(c == 27)
      break;
  }

  waitKey();
}

int houghTransform(unsigned char *i, int r, int c)
{
  double hough_h = ((sqrt(2.0) * (double)(c>r?r:c)) / 2.0);
  _img_h = r;
  _img_w = c;

  accumulator = (unsigned int *) calloc(r*c, sizeof(unsigned int *));
  _accu_h = hough_h * 2.0; // -r -> +r
  _accu_w = 180;
  double center_x = r / 2;
  double center_y = c / 2;

  /*
  Creazione e inizializzazione del vettore di accumulazione di size
    righe  x colonne dell'immagine
    */
  for(int y=0;y<r;y++)
  {
    for(int x=0;x<c;x++)
    {
      if( i[(y*c) + x] > 250 )
      {
        for(int t=0;t<180;t++)
        {
          double r = (((double)x - center_x) * cos((double)t * DEG2RAD)) + (((double)y - center_y) * sin((double)t * DEG2RAD));
          accumulator[ (int)((round(r + hough_h) * 180.0)) + t]++;
        }
      }
    }
  }

  return 0;
}

/*Funzione che torna i punti (x1, y1) e x2, y2/)*/
std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > getLines(int threshold)
{
  std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > lines;

  /*Se il vettore di accumulazione è nullo, ritorna le linee vuote*/
  if(accumulator == 0)
    return lines;


  for(int r=0;r<_accu_h;r++)
  {
    for(int t=0;t<_accu_w;t++)
    {
      if((int)accumulator[(r*_accu_w) + t] >= threshold)
      {
        //Is this point a local maxima (9x9)
        int max = accumulator[(r*_accu_w) + t];
        for(int ly=-4;ly<=4;ly++)
        {
          for(int lx=-4;lx<=4;lx++)
          {
            if( (ly+r>=0 && ly+r<_accu_h) && (lx+t>=0 && lx+t<_accu_w)  )
            {
              if( (int)accumulator[( (r+ly)*_accu_w) + (t+lx)] > max )
              {
                max = accumulator[( (r+ly)*_accu_w) + (t+lx)];
                ly = lx = 5;
              }
            }
          }
        }
        if(max > (int)accumulator[(r*_accu_w) + t])
          continue;


        int x1, y1, x2, y2;
        x1 = y1 = x2 = y2 = 0;

        if(t >= 45 && t <= 135)
        {
          //y = (r - x cos(t)) / sin(t)
          x1 = 0;
          y1 = ((double)(r-(_accu_h/2)) - ((x1 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);
          x2 = _img_w - 0;
          y2 = ((double)(r-(_accu_h/2)) - ((x2 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);
        }
        else
        {
          //x = (r - y sin(t)) / cos(t);
          y1 = 0;
          x1 = ((double)(r-(_accu_h/2)) - ((y1 - (_img_h/2) ) * sin(t * DEG2RAD))) / cos(t * DEG2RAD) + (_img_w / 2);
          y2 = _img_h - 0;
          x2 = ((double)(r-(_accu_h/2)) - ((y2 - (_img_h/2) ) * sin(t * DEG2RAD))) / cos(t * DEG2RAD) + (_img_w / 2);
        }

        lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int> >(std::pair<int, int>(x1,y1), std::pair<int, int>(x2,y2)));

      }
    }
  }

  std::cout << "lines: " << lines.size() << " " << threshold << std::endl;
  return lines;
}


const unsigned int* getAccu(int *w, int *h)
{
  *w = _accu_w;
  *h = _accu_h;

  return accumulator;
}
