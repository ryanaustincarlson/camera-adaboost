#ifndef _IMAGE_TESTER_H
#define _IMAGE_TESTER_H

#include <vector>

#include <QImage>

#include "Grid.h"
#include "features/ImageTransforms.h"
#include "adaboost/WeakClassifier.h"
#include "adaboost/TrainingData.h"

using std::vector;

class ImageTester {
  public:
    ImageTester();
    Grid testStrongClassifier(ImageTransforms &testImages, int extractItr, vector< WeakClassifier > strong);

  private:
    double normalizeClassification(double classify);
    bool classifierGuess(WeakClassifier &wc, float value);
};

#endif // _IMAGE_TESTER_H