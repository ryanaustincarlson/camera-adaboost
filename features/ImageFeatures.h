#ifndef _IMAGE_FEATURES_H
#define _IMAGE_FEATURES_H

#include <QList>
#include <QPair>

#include <vector>

#include "ImageTransforms.h"
#include "HaarFeature.h"
#include "adaboost/FeatureVector.h"

using std::vector;

class ImageFeatures {
  public:
    ImageFeatures();
    ~ImageFeatures();
    QList<FeatureVector*> generateFeatureVectors(QList<ImageTransforms*> trainingList, int numIterations);
    QList<FeatureVector*> generateFeatureVectors(ImageTransforms &testingImages, int numIterations, bool* ftrIndices);

    float extractSingleFeature(QImage* img, int idx, int xpix, int ypix, int extract_itr);

  private:
    int m_nodata; 

    QList<ImageTransforms*> m_trainingList;
    FeatureVector *m_ftrvec;
    QList<HaarFeature*> m_haarlist;

    qreal avg(QImage *img, int xpix, int ypix, int k);
    qreal avgsqr(QImage *img, int xpix, int ypix, int k);
    qreal variance(QImage *img, int xpix, int ypix, int k);
    QPair<int, int> minmax(QImage *img, int xpix, int ypix, int k);
    int featureVectorSize(int numIterations);

    vector<float> generateFeatureVectorsHelper(ImageTransforms* tImgs, int numIterations, bool* ftrIndices=NULL);

    int convolveHaar(QImage* img, int idx, int xpix, int ypix);
    void generateHaarFeatures(int numFtrs);
    
    bool inbounds(QImage *img, int x, int y);
    qreal pixelvalue(QColor color);
};

#endif