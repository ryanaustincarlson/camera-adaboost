#include <fstream>
#include <iostream>
#include <cstdio>
#include "TrainingData.h"

using namespace std;

/***********************************
 * Function: addFeature
 * --------------------
 * Adds in_ftr to data.
 *
 * Returns: true on success; false otherwise.
 */
bool TrainingData::addFeature(FeatureVector in_ftr){
  // if dimensions has not yet been set, set it based on dimensions of in_ftr
  if (!num_dimensions)
    num_dimensions = in_ftr.size();

  // check that in_ftr has same dimension as the object requires
  else if (in_ftr.size() != num_dimensions) 
    return false;

  data.push_back(in_ftr);

  return true;
}

// returns size of data (# of features)
unsigned int TrainingData::size(){ return data.size(); }

// returns number of dimensions (size of a feature vector)
unsigned int TrainingData::dimensions(){ return num_dimensions; }

// returns val, weight at the ith index of data
int TrainingData::val(unsigned int i){ return data[i].val(); }
float TrainingData::weight(unsigned int i) { return data[i].weight(); }

// returns feature (a float) at (index in data, dimension in feature vector)
float TrainingData::at(unsigned int idx, unsigned int dimension){
  return data[idx].at(dimension);
}

// returns feature vector at index specified
FeatureVector* TrainingData::feature(unsigned int idx){ return &data[idx]; }

// sets weight to value given at index
void TrainingData::setWeight(unsigned int idx, float weight){
  data[idx].setWeight(weight);
} 

// prints out all instance fields of training data
void TrainingData::printData(){
  printf("num_dimensions: %d, num_features: %d\n",
      num_dimensions, data.size());
  for (unsigned int i=0; i<data.size(); i++){
    printf("[[%d]]\t",i);
    data[i].printFeature();
  }
}

// writes out necessary instance fields of training data
void TrainingData::writeData(string filename){
  ofstream outFile(filename.c_str());

  for (unsigned int i=0; i<data.size(); i++){
    for (unsigned int d=0; d<num_dimensions; d++){
      outFile << data[i].at(d) << " ";
    }
    outFile << data[i].val() << " " << 
      data[i].weight() << endl;
  }
}
