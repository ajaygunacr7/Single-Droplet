#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <fstream>

template <typename T>
bool operator<(T& A, T& B)
{
  int N = 0;
  for(int i = 0; i < 3; ++i)
    {
      if (A[i] < B[i])
        N ++;
    }

    if (N > 2)
    {
      return true;
    }

    return false;
}

int main( int argc, char** argv )
{
  int H = 130;
  int W = 170;

  cv::Vec3b Pix,Pix2;
  cv::Vec3b BPix = {150,150,150 };

  std::ofstream File;
  File.open("ButanolDia.txt");

  cv::Mat image;
  cv::Mat Img,ImgC,ImgD;
  int MaxDiaV = 0,MaxDiaH = 0;
  double MaxDia = 0.0d;

  bool ReadH = true;


  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));

  cv::Rect R(380,190,W,H);

  // butanol end : 511888, JetA1 end : 512423

  for(unsigned long int FileNumber = 500000; FileNumber < 500001; FileNumber+=10)
  {
    std::string FileName = "Butanol/100000!" + std::to_string(FileNumber) + ".tif";
    //std::string FileName = "JetA1/10000!" + std::to_string(FileNumber) + ".tif";
    image = cv::imread(FileName.c_str() ,cv::IMREAD_GRAYSCALE);

    if(! image.data )
    {
        std::cout <<  "Image not found or unable to open" << std::endl ;
        return -1;
    }
    MaxDiaH = 0;
    Img = image(R);

    cv::Canny(Img,ImgC,100,120);

    cv::dilate(ImgC,ImgD,kernel);

    //cv::namedWindow( "Ajay 21AE60R01", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Canny Image", ImgD );
    cv::waitKey(0);
    MaxDiaV = 0;

    bool doit = true;


    for (int j = 48; j < H ; ++j)
    {
      doit = true;
      for (int i = 0; i < W/3; ++i)
      {

        Pix = ImgD.at<cv::Vec3b>(j,i);

        if(Pix < BPix)
        {

          for (int k = H - 3; k > 0; --k)
          {
            Pix2 = ImgD.at<cv::Vec3b>(j,k);
            if((Pix2 < BPix)  && doit == true)
            {

              if(k - i + 1 > MaxDiaH)
                MaxDiaH = k - i + 1;

              doit = false;
              break;
            }
          }
        }
      }
    }



    for (int j = 0; j < W/3; j++)
    {
      doit = true;
      for (int i = 0; i < H-5; ++i )
      {
          Pix = ImgD.at<cv::Vec3b>(i,j);
          if(Pix < BPix)
          {
            for (int k = H - 3; k > 0; --k)
            {

              Pix2 = ImgD.at<cv::Vec3b>(k,j);
              if((Pix2 < BPix)  && doit == true)
              {
                if(k - i + 1 > MaxDiaV)
                  MaxDiaV = k - i + 1;

                doit = false;
                break;
              }
            }
          }
      }

    }

    int RodDia = 0;
    int Num = 0;

    for (int j = W/3-5; j < W/3; j++)
    {
      doit = true;
      for (int i = 0; i < H-5; ++i )
      {
          Pix = ImgD.at<cv::Vec3b>(i,j);

          if(Pix < BPix)
          {
            for (int k = H - 3; k > 0; --k)
            {
              Pix2 = ImgD.at<cv::Vec3b>(k,j);
              if((Pix2 < BPix)  && doit == true)
              {
                if(k - i + 1 > RodDia)
                {
                  RodDia  += k - i + 1;
                  Num++;
                }
                doit = false;
                break;
              }
            }
          }
      }
    }


    if (MaxDiaH == 0)
      ReadH = false;

    if (ReadH == false)
      MaxDiaH = 0;

    double RDia = 0.0d;
    double Time = 0.0d;

    RDia = (double) (RodDia/Num);

    Time = (double) (FileNumber - 500000)/3000;

    MaxDia = (double) sqrt(MaxDiaH * MaxDiaV);
    MaxDia = (double) 0.3 * (MaxDia / RDia);

    double DDo;
    double DDoCalc;

    if ((FileNumber - 500000) == 0)
      DDo = MaxDia;


    if (MaxDiaV == RodDia)
      MaxDiaV = 0;

    DDoCalc = (double) pow(MaxDia,2)/pow(DDo,2);

    File << Time << "\t" << MaxDia << "\t" << DDoCalc << std::endl;

   }

  return 0;
}
