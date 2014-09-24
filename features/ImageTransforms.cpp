#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <cmath>

#include <QColor>

#include "ImageTransforms.h"

// // constructor -- also calculates difference between img1 and img2 and sobel
ImageTransforms::ImageTransforms(const QImage &img1, const QImage &img2, int trainingType, int xFocus, int yFocus)
  : m_img1(img1),
    m_img2(img2),
    m_trainingType(trainingType),
    m_xFocus(xFocus),
    m_yFocus(yFocus)
{
  
  if (img1.format() != img2.format()){
    char errmsg[100]; 
    sprintf(errmsg, "In ImageFeatures constructor: images are different formats");
    throw std::runtime_error(errmsg);
  }

  calculateImgDiff();
  calculateSobel();
}

QList<QImage*> ImageTransforms::images()
{
  QList<QImage*> imgs;
  imgs.append(&m_imgdiffGray);
  imgs.append(&m_sobel);
  return imgs;
}


void ImageTransforms::calculateImgDiff()
{
  int maxheight = std::max(m_img1.height(),m_img2.height());
  int maxwidth = std::max(m_img1.width(),m_img2.width());
  
  QImage tmp1 = m_img1.copy(0,0,maxwidth,maxheight);
  QImage tmp2 = m_img2.copy(0,0,maxwidth,maxheight);
 
  m_imgdiff = QImage(maxwidth, maxheight, m_img1.format());
  m_imgdiff.fill(0);

  m_imgdiffGray = QImage(maxwidth, maxheight, m_img1.format());
  m_imgdiffGray.fill(0);
  
  // calculate difference between img1 and img2
  for (int y=0; y<maxheight; y++){
    for (int x=0; x<maxwidth; x++){
      
      QColor color1 = tmp1.pixel(x,y);
      QColor color2 = tmp2.pixel(x,y);
      QColor colordiff;
      colordiff.setRed( abs( color1.red() - color2.red() ));
      colordiff.setGreen( abs( color1.green() - color2.green() ));
      colordiff.setBlue( abs( color1.blue() - color2.blue() ));
      
      m_imgdiff.setPixel(x,y,colordiff.rgb());

      m_imgdiffGray.setPixel(x,y,toGray(colordiff));
    }
  }
}

QRgb ImageTransforms::toGray(QColor color){
  int gray = (color.red()*11 + color.green()*16 + color.blue()*5)/32;
  return QColor(gray, gray, gray).rgb();
}

// this function was adapted for Qt from code found at
// http://www.pages.drexel.edu/~weg22/edge.html
void ImageTransforms::calculateSobel(){
  // rows == height
  // cols == width

  m_sobel = QImage(m_imgdiff.width(), m_imgdiff.height(), m_imgdiff.format());
  
  int GX[3][3];
  int GY[3][3];
  /* 3x3 GX Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
  GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
  GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
  GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;

  /* 3x3 GY Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
  GY[0][0] =  1; GY[0][1] =  2; GY[0][2] =  1;
  GY[1][0] =  0; GY[1][1] =  0; GY[1][2] =  0;
  GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;

  int X,Y;
  int I,J;
  long sumX, sumY;
  int SUM;
  /*---------------------------------------------------
   * SOBEL ALGORITHM STARTS HERE
   * ------------------------------------------------*/
  for(Y=0; Y<=(m_imgdiff.height()-1); Y++)  {
    for(X=0; X<=(m_imgdiff.width()-1); X++)  {
      sumX = 0;
      sumY = 0;

      /* image boundaries */
      if(Y==0 || Y==m_imgdiff.height()-1){
        SUM = 0;
      } else if(X==0 || X==m_imgdiff.width()-1) {
        SUM = 0;

      /* Convolution starts here */
      } else {

        /*-------X GRADIENT APPROXIMATION------*/
        for(I=-1; I<=1; I++)  {
          for(J=-1; J<=1; J++)  {
            sumX += qGray(m_imgdiff.pixel(X+I,Y+J))*GX[I+1][J+1];
            //sumX = sumX + (int)( (*(source.data + X + I + (Y + J)*source.width())) * GX[I+1][J+1]);
          }
        }
        if(sumX>255)  sumX=255;
        if(sumX<0)    sumX=0;

        /*-------Y GRADIENT APPROXIMATION-------*/
        for(I=-1; I<=1; I++)  {
          for(J=-1; J<=1; J++)  {
            sumY += qGray(m_imgdiff.pixel(X+I,Y+J))*GY[I+1][J+1];
            //sumY = sumY + (int)( (*(source.data + X + I + (Y + J)*source.width())) * GY[I+1][J+1]);
          }
        }
        if(sumY>255) sumY=255;
        if(sumY<0) sumY=0;

        SUM = abs(sumX) + abs(sumY); /*---GRADIENT MAGNITUDE APPROXIMATION (Myler p.218)----*/
      }

      int pixcolor = abs(255-SUM);
      m_sobel.setPixel(X,Y,QColor(pixcolor,pixcolor,pixcolor).rgb());
    }
  }
}

///////////////////////
// accessor methods
///////////////////////

QImage ImageTransforms::diff() const
{
  return m_imgdiff;
}

QImage ImageTransforms::diffGrayscale() const
{
  return m_imgdiffGray;
}

QImage ImageTransforms::sobel() const
{
  return m_sobel;
}

QImage ImageTransforms::img1() const
{
  return m_img1;
}

QImage ImageTransforms::img2() const
{
  return m_img2;
}

int ImageTransforms::xFocus() const
{
  return m_xFocus;
}

int ImageTransforms::yFocus() const
{
  return m_yFocus;
}

int ImageTransforms::trainingType() const
{
  return m_trainingType;
}

////////////////////////
// setter methods
////////////////////////

void ImageTransforms::setxFocus(int xFocus)
{
  m_xFocus = xFocus;
}

void ImageTransforms::setyFocus(int yFocus)
{
  m_yFocus = yFocus;
}
