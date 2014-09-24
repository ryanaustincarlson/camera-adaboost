#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "utils/utils.h"
#include "sorting.h"
#include "AdaBooster.h"

using namespace std;

// constructor
AdaBooster::AdaBooster() : err_bound(0.5), 
  dimensions(0), num_features(0), strong_err_threshold(0),
  strongFtrIndices(NULL) {};

AdaBooster::~AdaBooster() {
  if (strongFtrIndices)
    delete [] strongFtrIndices;
}

/*********************************
 * Function: getStrongClassifier
 * -----------------------------
 * This function generates a strong classifier that can accurately distinguish
 * positive training examples from negative training examples
 *
 * td: set of feature vectors
 * num_classifiers: the number of weak classifiers we want to be in the strong
 *    classifier (20 tends to be a good default it seems)
 *
 * returns: a strong classifier -- set of weak classifiers in optimal order
 */
vector<WeakClassifier> AdaBooster::getStrongClassifier(TrainingData& td, unsigned int num_classifiers){
  
  // set dimensions and number of features
  dimensions = td.dimensions();
  num_features = td.size();

  // initialize feature weights
  init_feature_weight(td);
  
  // vector of weak classifiers that make up a strong classifier
  vector<WeakClassifier> strong_classifier;

  // sort circle by features -- store in *sorted*
  create_feature_views(td);
  //td.printData();

  //char garbage[80]; // use this with cin.getline() below

  for (unsigned int i=0; i<num_classifiers; i++){
    // indentify best classifier
    WeakClassifier *wc = get_best_classifier();

    // if index invalid, then we're done constructing our strong classifier
    if (!wc)
      return strong_classifier;

    // otherwise, add best classifier to strong_classifier
    strong_classifier.push_back(*wc);
    //strong_classifier.back().printClassifier();

    // don't need classifier anymore so delete it
    delete wc;

    // update weights of features
    update_feature_weight(td, strong_classifier.back());
  }
  
  // delete sorted array
  delete [] sorted;

  // make sure strongFtrIndices is free
  if (strongFtrIndices){
    delete [] strongFtrIndices;
  }
  strongFtrIndices = new bool[dimensions];
  memset(strongFtrIndices, false, dimensions); // default all to false

  // any dimensions that we use in strong_classifier, set that index to true
  for (unsigned int i=0; i<strong_classifier.size(); i++){
    strongFtrIndices[(int)strong_classifier[i].dimension()] = true;
  }
  
  return strong_classifier;
} 


/***********************************
 * Function: get_best_classifier
 * -----------------------------
 * Calculates the error along each dimension and each feature in that dimension
 * and chooses the best threshold. The weak classifier that results will be
 * inserted into the strong classifier in getStrongClassifier()
 *
 * returns: the best weak classifier given current weights
 */
WeakClassifier* AdaBooster::get_best_classifier(){
  // this will hold the best classifier and lowest error so far
  double lowest_err = 1;
  unsigned int index = 0;
  unsigned int dimension = 0;
  double tmp_pos_err, tmp_neg_err;

  //
  // by default, value of feature < threhold of classifier --> is_flipped = false
  // else is_flipped = true
  //
  bool is_flipped = false;
  double flipped_err;

  // get total weight for POS and NEG values
  double pos_wt = 0, neg_wt = 0;
  for (unsigned int i=0; i<sorted[0].size(); i++){
    if (sorted[0][i]->val() == POS)
      pos_wt += sorted[0][i]->weight();
    else
      neg_wt += sorted[0][i]->weight();
  }

  // traverse the dimensions of the features
  for (unsigned int i=0; i<dimensions; i++){

    // since, at j=0, we correctly classify all NEG values and incorrectly
    // classify all POS values, we initialize our tmp values as such
    tmp_pos_err = pos_wt; 
    tmp_neg_err = 0;

    // traverse features
    for (unsigned int j=0; j<num_features; j++){

      // if we correctly classified the point, decrement tmp_pos_err
      if (sorted[i][j]->val() == POS)
        tmp_pos_err -= sorted[i][j]->weight();
      else // otherwise increment tmp_neg_err
        tmp_neg_err += sorted[i][j]->weight();

      // check that next feature vector isn't same as this one or that we
      // are at the end of the list of feature vectors
      if ( (j < num_features-1 && sorted[i][j+1]->at(i) != sorted[i][j]->at(i))
          || j == num_features-1 ){

        // if total current error is less than lowest error, replace it
        if (tmp_pos_err + tmp_neg_err < lowest_err){
          lowest_err = tmp_pos_err + tmp_neg_err;
          dimension = i;
          index = j;
          is_flipped = false;
        }
        // test if flipped error is less than lowest error
        flipped_err = (pos_wt - tmp_pos_err) + (neg_wt - tmp_neg_err);
        if (flipped_err < lowest_err){
          lowest_err = flipped_err;
          dimension = i;
          index = j;
          is_flipped = true;
        }
      }
    }
  }

  // if error is too high, return NULL
  if (lowest_err >= err_bound){
    printf("Error in get_best_classifier: error too large\n");
    return NULL;
  }

  // get weight and threshold
  float weight = weight_classifier(lowest_err); 
  float threshold = sorted[dimension][index]->at(dimension);

  // create weak classifier pointer and return
  WeakClassifier* wc = new WeakClassifier(dimension, threshold, is_flipped, weight);
  return wc;
}

/**************************************
 * Function: weight_classifier
 * ---------------------------
 * given an error value, we compute and return the weight that should be
 * applied to a classifier using the formula found on the wikipedia page for
 * alpha_t
 *
 * err: error used in weighting
 *
 * returns: weight computed using equation
 */
double AdaBooster::weight_classifier(double err){
  // if error is small enough, just return a big number because otherwise
  // we'd return infinity, which is..bad
  if (err < 0.0001)
    return 1000.;

  // calculate new weight 
  double weight = 0.5 * log( (1-err) / err );

  return weight;
}

/*************************************
 * Function: update_feature_weight
 * -------------------------------
 * Given TrainingData and a WeakClassifier that has been weighted in
 * get_best_classifier(), we recalculate the weights of all the features
 *
 * td: training data (set of features)
 * wc: (weighted) weak classifier
 *
 * returns true if successful, false otherwise
 */
bool AdaBooster::update_feature_weight(TrainingData &td, WeakClassifier &wc){
  // check that WeakClassifier has actually been weighted
  if (wc.weight() < 0){
    printf("Error in update_feature_weight: WeakClassifier has invalid weight\n");
    return false;
  }

  // traverse features in feature set and adjust their weights
  for (unsigned int i=0; i<num_features; i++){
    FeatureVector* fv = td.feature(i);
    // either 1 or -1 (used in weight below)
    int is_correct = is_classifier_correct(wc, *fv) ? 1 : -1;

    // calculate and update weight
    // note M_E := 2.71828
    float weight = pow(M_E, (double) -1 * wc.weight() * is_correct);
    td.setWeight(i, td.weight(i)*weight);
  }

  // calculate normalization factor
  float norm = 0;
  for (unsigned int i=0; i<num_features; i++)
    norm += td.weight(i);

  // normalize feature weights
  for (unsigned int i=0; i<num_features; i++)
    td.setWeight(i, td.weight(i)/norm);
  
  return true; // successful
}


/**************************************
 * Fucntion: is_classifier_correct
 * -------------------------------
 * returns true if weak classifier (wc) correctly identified the 
 * feature vector (fv), false otherwise.
 */
bool AdaBooster::is_classifier_correct(WeakClassifier &wc, FeatureVector &fv){
  
  // check if threshold is greater than (or equal to) feature
  bool guess = ( wc.threshold() >= fv.at(wc.dimension()) );

  // if classifier is flipped, negate guess
  guess = wc.isFlipped() ? !guess : guess;

  // find actual value of point
  bool real = ( fv.val() == POS );

  // return if guess and real agree
  return ( real == guess );

}

/**************************************
 * Function: getStrongError
 * ------------------------
 * calculates error rates at each "level" of the strong classifier; i.e. at
 * each weak classifier
 *
 * td: Training data to check strong classifier against
 * strong: strong classifier (i.e. ordered set of weak classifiers)
 *
 * returns: a list of the errors at each level
 */
vector<vector <double> > AdaBooster::getStrongError(TrainingData &td, vector<WeakClassifier> &strong){
  unsigned int true_pos, false_pos, true_neg, false_neg;
  float precision, recall;
  vector< vector<double> > strong_err;
  vector<double> stats;

  // clear false_indices
  false_indices.clear();
  
  // set dimensions and number of features
  dimensions = td.dimensions();
  num_features = td.size();

  // initialize vector of num_ftrs to zero
  vector<double> classify;
  float classifyNorm;
  for (unsigned int i=0; i<num_features; i++)
    classify.push_back(0.0);

  // keeps track of which features have been eliminated
  // by the cascade. If a feature needs further classification,
  // the array[i] == true, o/wise false
  bool *cascadeCounter = new bool[num_features];
  memset(cascadeCounter, true, num_features); // default all to true

  int sign;
  // traverse all weak classifiers
  for (unsigned int i=0; i<strong.size(); i++){
    
    true_pos = false_pos = true_neg = false_neg = precision = recall = 0;
    // traverse all features
    for (unsigned int j=0; j<num_features; j++){
      
      // check what the classifier guessed. If weak classifier decided
      // the feature was POS, sign = 1, otherwise sign = -1
      if ( (strong[i].threshold() > td.at(j,strong[i].dimension()) && !strong[i].isFlipped()) ||
          (strong[i].threshold() < td.at(j,strong[i].dimension()) && strong[i].isFlipped()) ) {
        sign = 1;
      } else {
        sign = -1;
      }

      // make sure cascade says we want to keep evaluating this point
      if (cascadeCounter[j]){
        // calculate classify so far
        classify[j] += strong[i].weight() * sign;
      }

      // normalize the classification score
      classifyNorm = normalizeClassification(classify[j]);

      // is normalized score below cascade threshold? If so, we're done with this feature
      if (cascadeCounter[j] && classifyNorm < cascadeThreshold(i, strong.size())){
        cascadeCounter[j] = false;
      }
      
      // check classification against reality
      if (classify[j] >= strong_err_threshold && td.val(j) == POS) {
        true_pos++;
        
      } else if (classify[j] >= strong_err_threshold && td.val(j) == NEG){
        false_pos++;
        
        // if we're at the last weak classifier and we still can't classify this point
        if (i == strong.size()-1)
          false_indices.push_back(j); // add index to false indices vector
          
      } else if (classify[j] < strong_err_threshold && td.val(j) == POS){
        false_neg++;
        
        // similarly, we can't classify the point
        if (i == strong.size()-1)
          false_indices.push_back(j);
        
      } else {
        true_neg++;
      }
      
    }
    // calculate some stats and push into strong_err
    stats.clear();
    stats.push_back((double)(false_pos + false_neg)/num_features); // flat error percentage
    stats.push_back((double)(true_pos)/(true_pos+false_pos)); // precision
    stats.push_back((double)(true_pos)/(true_pos+false_neg)); // recall
    stats.push_back((double)true_pos); // true positives
    stats.push_back((double)true_neg); // true negatives
    stats.push_back((double)false_pos); // false positives
    stats.push_back((double)false_neg); // false negatives

    strong_err.push_back(stats);
  }
  return strong_err;
}

/**************************************
 * Function: init_feature_weight
 * -----------------------------
 * Initializes weights of features to be 1/(total num of features)
 *
 * td: training data -- set of features
 */
void AdaBooster::init_feature_weight(TrainingData &td){
  // set all points to same weight = 1/(# of features)
  for (unsigned int i=0; i<num_features; i++)
    td.setWeight(i, 1./num_features);
}

/**************************************
 * Function: create_feature_views
 * ------------------------------
 * For each dimension, we sort the feature vectors and store the result in
 * *sorted*. Note that *sorted* is a little complicated -- we need the sorting to
 * be fast so we sort feature vector pointers, and we have a vector of these
 * for each dimension.
 *
 * td: training data -- set of features
 */
void AdaBooster::create_feature_views(TrainingData &td){
  sorted = new vector<FeatureVector *>[dimensions];
  // add a feature pointer to sorted for each dimension in the training data
  for (unsigned int i=0; i<dimensions; i++){
    for (unsigned int j=0; j<num_features; j++){
      sorted[i].push_back(td.feature(j));
    }
  }

  // sort each feature vector pointer in sorted
  for (unsigned int i=0; i<dimensions; i++)
    sort(sorted[i].begin(), sorted[i].end(), idx_cmp(i));
}

/*********************************************
 * Function: getFalseIndices
 * -------------------------
 * Returns false_indices vector, which is a list of indices of features that
 * we incorrectly guessed (either false_pos or false_neg). Used in validation
 * stats.
 */
vector<int> AdaBooster::getFalseIndices(){ return false_indices; }


/********************************************
 * Function: getStrongFtrIndices
 * -----------------------------
 * Returns the strong classifier feature indices, which is an array of
 * booleans telling you which dimensions were selected for the strong
 * classifier. It's used to reduce the number of features we need to
 * extract over the testing area
 */
bool* AdaBooster::getStrongFtrIndices(){ return strongFtrIndices; }

/**********************************************
 * Function: printStrongStats
 * ----------------------------
 * given a strong error vector, we compute some statistics about the error and
 * print out the results. Statistics are as follows:
 *    percent error
 *    precision
 *    recall
 *    true positive
 *    true negative
 *    false positive
 *    false negative
 */
void AdaBooster::printStrongStats(vector< vector<double> > strong_err){
  printf("idx\terror\tprcsn\trecall\t  tp\ttn\tfp\tfn\n");
  printf("---\t-----\t-----\t------\t  --\t--\t--\t--\n");
  for (unsigned int i=0; i<strong_err.size(); i++)
    printf("[%d]\t%.3f\t%.3f\t%.3f\t  %d\t%d\t%d\t%d\n",i,
        strong_err[i][0]*100,strong_err[i][1]*100,strong_err[i][2]*100,
        (int)strong_err[i][3], (int)strong_err[i][4], 
        (int)strong_err[i][5], (int)strong_err[i][6]);
  printf("---\t-----\t-----\t------\t  --\t--\t--\t--\n");
  printf("idx\terror\tprcsn\trecall\t  tp\ttn\tfp\tfn\n\n");
}

// input/output of strong classifier
void AdaBooster::writeStrongClassifier(vector< WeakClassifier >& strong, string outfname)
{
  ofstream outfile(outfname.c_str());

  outfile << strong.size() << endl;

  for (unsigned int i=0; i<strong.size(); i++){
    WeakClassifier wc = strong[i];
    outfile << wc.dimension() << " " << wc.isFlipped() << " " <<
      wc.threshold() << " " << wc.weight() << endl;
  }
  outfile.close();
}

vector< WeakClassifier > AdaBooster::readStrongClassifier(string infname)
{
  ifstream infile(infname.c_str());
  int size;
  infile >> size;

  float dimension, threshold, weight;
  bool isflipped;

  vector<WeakClassifier> strong;
  for (int i=0; i<size; i++){
    infile >> dimension >> isflipped >> threshold >> weight;
    strong.push_back(WeakClassifier(dimension, threshold, isflipped, weight));
  }
  return strong;
}

