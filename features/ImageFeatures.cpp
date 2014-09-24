#include <cmath>
#include <climits>
#include <cstdio>
#include <stdexcept>
#include <cstdlib>

#include <QColor>
#include <QPair>

#include "ImageFeatures.h"

using std::vector;

ImageFeatures::ImageFeatures() :
  m_nodata(-9999)
{
  
}

ImageFeatures::~ImageFeatures()
{
  foreach (HaarFeature *haarftr, m_haarlist){
    delete haarftr;
  }
}

int ImageFeatures::featureVectorSize(int numIterations)
{
  generateHaarFeatures(numIterations);
  return m_haarlist.size() + 4;
}

// used for training
QList<FeatureVector*> ImageFeatures::generateFeatureVectors(QList< ImageTransforms* > trainingList, int numIterations)
{
  // want numIterations to be intuitive, but the "last" set
  // of features in the while loop must be numIterations==0
  numIterations--;
  
  generateHaarFeatures(numIterations);

  
  QList<FeatureVector*> ftrvecList;
  vector<float> extracted;
  foreach (ImageTransforms *tImgs, trainingList){
    extracted = generateFeatureVectorsHelper(tImgs, numIterations);
    ftrvecList.append(new FeatureVector(extracted, tImgs->trainingType()));
  }
  return ftrvecList;
}

// used for testing
QList< FeatureVector* > ImageFeatures::generateFeatureVectors(ImageTransforms& testingImages, int numIterations, bool* ftrIndices)
{
  // want numIterations to be intuitive, but the "last" set
  // of features in the while loop must be numIterations==0
  numIterations--;
  
  generateHaarFeatures(numIterations);
  
  QImage testImageCur = testingImages.img1();
  QImage testImagePast = testingImages.img2();

  vector<float> extracted;
  QList <FeatureVector*> ftrvecList;
  int sofar, total = testImageCur.width()*testImageCur.height();
  
  for (int h=0; h<testImageCur.height(); h++){
    for (int w=0; w<testImageCur.width(); w++){

      sofar = h*testImageCur.width()+w;
      printf("Progress: %.2f, %d of %d\r", 100.*sofar/total, sofar, total);
      fflush(stdout);

      // set focus for this testing image
      testingImages.setyFocus(h);
      testingImages.setxFocus(w);

      // extract feature vectors
      extracted = generateFeatureVectorsHelper(&testingImages, numIterations, ftrIndices);
      ftrvecList.append(new FeatureVector(extracted, testingImages.trainingType()));
    }
  }
  printf("\n");
  return ftrvecList;
}

std::vector< float > ImageFeatures::generateFeatureVectorsHelper(ImageTransforms* tImgs, int numIterations, bool* ftrIndices)
{
  QList<QImage*> imageList = tImgs->images();

  // set size for speed
  vector<float> extracted;
  foreach (QImage* img, imageList){

    int numftrs = extractSingleFeature(img, -1, tImgs->xFocus(), tImgs->yFocus(), numIterations);
    for (int i=0; i<numftrs; i++){
      extracted.push_back(extractSingleFeature(img, i, tImgs->xFocus(), tImgs->yFocus(), numIterations));
    }

  }
  return extracted;
}

float ImageFeatures::extractSingleFeature(QImage* img, int idx, int xpix, int ypix, int extract_itr)
{
 if (m_haarlist.empty()){
   generateHaarFeatures(extract_itr);
  }

  // if index is less than zero, return the number
  // of features we have
  if (idx < 0){
    return m_haarlist.size() + (extract_itr+1)*4;
  }

  // extract all haar features first
  if ((unsigned)idx < m_haarlist.size()){
    return convolveHaar(img, idx, xpix, ypix);
  }

  // finally do min, max, avg, variance
  int offset = m_haarlist.size();
  float fnc;
  int numstats = 4;
  for (int k=0; k<=extract_itr; k++){

    fnc = std::pow(2.,2*(extract_itr-k)+1); // 2^(2n+1)

    // extract minimum
    if (idx == offset+numstats*k){
      return minmax(img, xpix, ypix, fnc).first;

    // extract maximum
    } else if (idx == offset+numstats*k+1){
      return minmax(img, xpix, ypix, fnc).second;

    // extract avg
    } else if (idx == offset+numstats*k+2){
      return avg(img, xpix, ypix, fnc);

    // extract variance
    } else if (idx == offset+numstats*k+3){
      return variance(img, xpix, ypix, fnc);
    }
  }
  
  // error if index isn't valid
  printf("in Feature::extractSingleFeature, invalid index (%d)\n", idx);
  exit(2);
}


qreal ImageFeatures::avg(QImage *img, int xpix, int ypix, int k)
{
  qreal total = 0.0;
  int count = 0;
  
  // traverse a box of length 2k+1
  for (int y=-k; y<=k; y++){
    for (int x=-k; x<=k; x++){
      
      // check if value is in bounds
      if (inbounds(img, xpix+x,ypix+y)){
        
        // add pixel value to total and increment count
        total += pixelvalue((QColor)(img->pixel(xpix+x, ypix+y)));
        count++;
      }
    }
  }
  // return the average
  if (count == 0){
    char errmsg[100]; 
    sprintf(errmsg, "In ImageFeatures::avg, no pixels in bounds");
    throw std::runtime_error(errmsg);
  }
  return total/count;
}

// same as avg but square each pixelvalue before averaging together
qreal ImageFeatures::avgsqr(QImage *img, int xpix, int ypix, int k)
{
  qreal total = 0.0;
  int count = 0;
  
  // traverse a box of length 2k+1
  for (int y=-k; y<=k; y++){
    for (int x=-k; x<=k; x++){
      
      // check if value is in bounds
      if (inbounds(img, xpix+x,ypix+y)){
        
        // add pixel value to total and increment count
        qreal pixval = pixelvalue((QColor)(img->pixel(xpix+x, ypix+y)));
        total += pixval*pixval;
        count++;
      }
    }
  }
  // return the average
  if (count == 0){
    char errmsg[100]; 
    sprintf(errmsg, "In ImageFeatures::avgsqr, no pixels in bounds");
    throw std::runtime_error(errmsg);
  }
  return total/count;
}

// <x^2> - <x>^2
qreal ImageFeatures::variance(QImage *img, int xpix, int ypix, int k)
{
  // get squared average
  // <x^2>
  qreal avgSquared = avgsqr(img, xpix, ypix, k);

  // get average and square it
  // <x>^2
  qreal average = avg(img, xpix, ypix, k);
  average *= average;

  // compute variance
  // <x^2> - <x>^2
  return avgSquared - average;
}

QPair<int, int> ImageFeatures::minmax(QImage *img, int xpix, int ypix, int k)
{
  int max = INT_MIN;
  int min = INT_MAX;
  int tmpX, tmpY, pixelval;
  // traverse a box of length 2k+1
  for (int y=-k; y<=k; y++){
    for (int x=-k; x<=k; x++){
      
      // check if value is in bounds
      if (inbounds(img, xpix+x,ypix+y)){
        tmpX = xpix+x;
        tmpY = ypix+y;

        pixelval = pixelvalue((QColor)(img->pixel(tmpX,tmpY)));
        
        // check if pixel value is greater than current max
        if (pixelval > max)
          max = pixelval;
        
        // check if val is less than current min
        if (pixelval < min)
          min = pixelval;
      }
    }
  }
  // push back min and max values and return
  return QPair<int, int>(min, max);
}

int ImageFeatures::convolveHaar(QImage* img, int idx, int xpix, int ypix)
{
  int total = 0;

  // traverse each HaarFeature
  for (int y=-m_haarlist[idx]->rows()/2; y<=m_haarlist[idx]->rows()/2; y++){
    for (int x=-m_haarlist[idx]->cols()/2; x<=m_haarlist[idx]->cols()/2; x++){

      if (inbounds(img, xpix+x,ypix+y)){

        // align points and take weird dot product between
        // HaarFeature and gdal_window
        total += m_haarlist[idx]->ftr( y+m_haarlist[idx]->rows()/2, x+m_haarlist[idx]->cols()/2)
        * pixelvalue(img->pixel(xpix+x, ypix+y));
      }
    }
  }
  return total;
}

void ImageFeatures::generateHaarFeatures(int numFtrs)
{
  // if we've already run this, don't do it again
  if (!m_haarlist.isEmpty()){
    return;
  }

  int edge_dim, line_dim, diamond_dim;
  while (numFtrs >= 0){
    // dimensions for edge Haar-like features
    edge_dim = 2*std::pow(2.,numFtrs) + 1;

    // dimensions for line features
    line_dim = 2*numFtrs + 1;

    // dimensions for diamonds
    diamond_dim = 2*std::pow(2.,numFtrs+1) + 1;

    // make vertical edge feature
    m_haarlist.append(new HaarFeature(edge_dim, edge_dim, VERT_EDGE));

    // horizontal edge feature
    m_haarlist.append(new HaarFeature(edge_dim, edge_dim, HORIZ_EDGE));

    // checkerboard feature
    m_haarlist.append(new HaarFeature(edge_dim, edge_dim, CHECKERBOARD));

    // diamond -- vertical right edge feature
    m_haarlist.append(new HaarFeature(diamond_dim, edge_dim, VERT_RIGHT_EDGE));

    // diamond -- vertical left edge feature
    m_haarlist.append(new HaarFeature(diamond_dim, edge_dim, VERT_LEFT_EDGE));

    // diamond -- horizontal right edge feature
    m_haarlist.append(new HaarFeature(edge_dim, diamond_dim, HORIZ_RIGHT_EDGE));

    // diamond -- horizontal left edge feature
    m_haarlist.append(new HaarFeature(edge_dim, diamond_dim, HORIZ_LEFT_EDGE));

    // vertical thin line feature
    m_haarlist.append(new HaarFeature(line_dim, 3*line_dim, VERT_THIN_LINE));

    // vertical thick line feature
    m_haarlist.append(new HaarFeature(line_dim, 5*line_dim, VERT_THICK_LINE));

    // horizontal thin line feature
    m_haarlist.append(new HaarFeature(3*line_dim, line_dim, HORIZ_THIN_LINE));

    // horizontal thick line feature
    m_haarlist.append(new HaarFeature(5*line_dim, line_dim, HORIZ_THICK_LINE));

    // vertical-right thin line
    m_haarlist.append(new HaarFeature(4*line_dim-1,3*line_dim,VERT_RIGHT_THIN_LINE));

    // vertical-left thin line
    m_haarlist.append(new HaarFeature(4*line_dim-1,3*line_dim,VERT_LEFT_THIN_LINE));

    // vertical-right thick line
    m_haarlist.append(new HaarFeature(6*line_dim-1, 5*line_dim, VERT_RIGHT_THICK_LINE));

    // vertical-left thick line
    m_haarlist.append(new HaarFeature(6*line_dim-1, 5*line_dim, VERT_LEFT_THICK_LINE));

    numFtrs--;
  }
}

bool ImageFeatures::inbounds(QImage *img, int x, int y)
{
  if (x < 0 || x >= img->width() || y < 0 || y >= img->height()){
    return false;
  }
  return true;
}

qreal ImageFeatures::pixelvalue(QColor color)
{
  // for now, returns avg of three colors. Note that
  // if image is grayscale (which we are assuming)
  // then this is same as selecting any of the three
  // primary colors
  return (color.red() + color.green() + color.blue())/3;
}

