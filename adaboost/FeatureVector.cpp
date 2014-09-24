#include <cstdio>
#include "FeatureVector.h"

using namespace std;

//constructor
FeatureVector::FeatureVector(const vector<float> &in_vec, 
    int in_val) : value(in_val), wt(-1){
  // copy vector of floats
  for (unsigned int i=0; i<in_vec.size(); i++)
    fvec.push_back(in_vec[i]);
}

// returns size of feature vector (useful in addFeature in TrainingData)
unsigned int FeatureVector::size(){ return fvec.size(); }

// more getter methods
int FeatureVector::val(){ return value; }
float FeatureVector::weight(){ return wt; }
float FeatureVector::at(unsigned int i){ return fvec[i]; }

// sets weight to given value 
void FeatureVector::setWeight(float weight){ wt = weight; }

// prints out all instance fields of a feature
void FeatureVector::printFeature(){
  for (unsigned int i=0; i<fvec.size(); i++)
    printf("[%d]: %f ",i,fvec[i]);
  printf("\n\tval: %d, weight: %f\n",value,wt);
}
