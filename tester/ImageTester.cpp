#include <QDebug>

#include <cmath>

#include "features/ImageFeatures.h"
#include "utils/utils.h"
#include "ImageTester.h"

ImageTester::ImageTester()
{

}

Grid ImageTester::testStrongClassifier(ImageTransforms &testImages, int extractItr, vector< WeakClassifier > strong)
{
  QList<QImage*> imgs = testImages.images();
  int imageHeight = imgs[0]->height();
  int imageWidth = imgs[0]->width();
  
  Grid classified(imageHeight, imageWidth);

  double classify;
  int sign;

  ImageFeatures testFtrs;
  int sizeOfFeature = testFtrs.extractSingleFeature(imgs[0], -1, 0, 0, extractItr);

  float feature;
  int dimension, featureIdx;
  for (int y=0; y<imageHeight; y++){
    for (int x=0; x<imageWidth; x++){
      classify = 0.0;
      
      printf("Progress: %.2f%%\r", 100.*(y*imageWidth+x)/(imageHeight*imageWidth));
      fflush(stdout); // */

      for (unsigned int i=0; i<strong.size(); i++){
        dimension = (int)strong[i].dimension();
        featureIdx = dimension - (dimension/sizeOfFeature) * sizeOfFeature; // note use of integer division
        feature = testFtrs.extractSingleFeature( imgs[dimension/sizeOfFeature],
                                                 featureIdx, x, y, extractItr);

        // check what the classifier guessed. If weak classifier decided
        // the feature was POS, sign = 1, otherwise sign = -1
        classifierGuess(strong[i], feature) ? sign = 1 : sign = -1;

        // calculate classify so far
        classify += strong[i].weight() * sign;

        // we're done testing, just output that score
        if (normalizeClassification(classify) < cascadeThreshold(i, strong.size())){
          break;
        }
      }
      classified.set(y, x, normalizeClassification(classify));
    }
  }
/* // TODO remove!
  // traverse all features
  for (unsigned int j=0; j<testData.size(); j++){
    // traverse all weak classifiers
    classify = 0.0;
    for (unsigned int i=0; i<strong.size(); i++){
      // check what the classifier guessed. If weak classifier decided
      // the feature was POS, sign = 1, otherwise sign = -1
      classifierGuess(strong[i], testData.at(j,strong[i].dimension())) ? sign = 1 : sign = -1;

      // calculate classify so far
      classify += strong[i].weight() * sign;
    }
    // write normalized classify to buffer
    row = j / imageWidth;
    col = j - row*imageWidth;
    classified.set(row, col, normalizeClassification(classify));
  }
*/
  return classified;
}


/**********************************************
 * Function: normalizeClassification
 * ---------------------------------
 * normalizes classification ( H(x) in eq'n )
 * value according to
 *
 *    1 / (1 + e ^ (- H(x) ) )
 *
 * initially scales 0 --> 1, then expand to 0 --> 255...well, not right now, but maybe later?
 */
double ImageTester::normalizeClassification(double classify){
  return 1. / ( 1 + exp( -classify ) );
}

/**********************************************
 * Function: classifierGuess
 * -------------------------
 * Returns true if classifier decided a feature was POS, otherwise return false
 */
bool ImageTester::classifierGuess(WeakClassifier &wc, float value){
  return ( (wc.threshold() > value && !wc.isFlipped()) ||
      (wc.threshold() < value && wc.isFlipped()) );
}