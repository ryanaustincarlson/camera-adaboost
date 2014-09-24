#ifndef _TRAINING_DATA_H
#define _TRAINING_DATA_H

#include <vector>
#include <string>
#include "FeatureVector.h"

class TrainingData {
  public:
    TrainingData() :
      num_dimensions(0) {}; // constructor

    bool addFeature(FeatureVector in_ftr);
    unsigned int size();
    unsigned int dimensions();

    int val(unsigned int i);
    float weight(unsigned int i);

    float at(unsigned int idx, unsigned int dimension);
    FeatureVector* feature(unsigned int idx);

    void setWeight(unsigned int idx, float weight);

    // Output
    void printData();
    void writeData(std::string filename);

  private:
    std::vector <FeatureVector> data; // set of feature vectors
    unsigned int num_dimensions; // size of each feature vector
};

#endif
