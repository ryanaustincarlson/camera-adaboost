#ifndef _FEATURE_VECTOR_H
#define _FEATURE_VECTOR_H

#include <vector>

const int POS = 1;
const int NEG = -1;

/**********************************
 * Class: FeatureVector
 * --------------------
 * Feature vectors have a value, weight, and vector of features (floats) that
 * can be compared with other feature vectors
 */
class FeatureVector {
  public:
    FeatureVector(const std::vector<float> &in_vec, 
        int in_val); // constructor

    unsigned int size();
    int val();
    float weight();
    float at(unsigned int i);

    void setWeight(float weight);

    void printFeature();

  private:
    std::vector <float> fvec; // feature vector
    int value; // value (POS or NEG)
    float wt; // weight
};

#endif
