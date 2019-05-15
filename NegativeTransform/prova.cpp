/*
Operazione puntuale: negativo di un immagine

input: immagine (Scala di grigio è preferibile)

output: immagine in negativo.

Operazione: output = 255 (Valore di massima intensità) - input
y = (L-1) - x

*/
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  /*
  Caricamento immagine. Il campo 0 significa che l'immagine viene
  trasformata in scala di grigio (non in bianco e nero)
  */
  // Mat img = imread("lena.jpg", 0);
  Mat img;
  Mat img2;


  if(argv[1] == nullptr)
  {
    cerr <<"Errore! Inserire un argomento..." << endl;
    cerr <<"Usage: ./<nome_exe> [nomeImmagine.estensione]" << endl;
    exit(-1);
  }
  else
  {
    cout << "OK! Immagine caricata correttamente!"<< endl;
  }

  /*
  IMREAD_GRAYSCALE equivale a mettere 0
   come secondo argomento di imread
  */
  img = imread(argv[1], IMREAD_GRAYSCALE);
  /*Copio l'immagine in un'altra variabile*/
  img2 = img;

  imshow("Lena normale gray", img);
  /*Operazione di negativo dell'immagine*/
  img2 = 255-img;
  imshow("Lena neg", img2);
  waitKey();
  return 0;
}
