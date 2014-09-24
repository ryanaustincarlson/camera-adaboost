#include <fstream>
#include <iostream>
#include "WeakClassifier.h"

// getter methods
float WeakClassifier::dimension(){ return dim; }
float WeakClassifier::weight(){ return wt; }
float WeakClassifier::threshold(){ return thresh; }
bool WeakClassifier::isFlipped(){ return flipped; }

// prints all instance fields of a weak classifier
void WeakClassifier::printClassifier(){
  printf("dimension: %d\t threshold: %.10f\t flipped: %s\t weight: %f\n",
      dim, thresh, (flipped)?"true ":"false",wt);
}
