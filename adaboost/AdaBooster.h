#ifndef _ADA_BOOSTER_H
#define _ADA_BOOSTER_H

#include <vector>
#include "FeatureVector.h"
#include "TrainingData.h"
#include "WeakClassifier.h"

/*************************
 * Class: AdaBooster
 * -----------------
 * This class uses a set of training data to generate a strong classifier. 
 */
class AdaBooster {
  public:
    AdaBooster(); // constructor
    ~AdaBooster();
    std::vector<WeakClassifier> getStrongClassifier(TrainingData &td, unsigned int num_classifiers);
    std::vector< std::vector<double> > getStrongError(TrainingData &td,
        std::vector<WeakClassifier> &strong);
    std::vector<int> getFalseIndices();
    void printStrongStats(std::vector< std::vector<double> > strong_err);
    bool *getStrongFtrIndices();

    void writeStrongClassifier(std::vector<WeakClassifier> &strong, std::string outfname);
    std::vector<WeakClassifier> readStrongClassifier(std::string infname);


  private:
    const float err_bound; // upper error bound
    std::vector<FeatureVector *> *sorted;
    unsigned int dimensions;
    unsigned int num_features;

    // contains list of indices of features that we incorrectly guessed
    // (either false_pos or false_neg). Useful for validation stats.
    std::vector<int> false_indices;

    // threshold used in getStrongError to determine whether a weak
    // classifier classified a point as POS or NEG
    float strong_err_threshold; 

    // array to keep track of which indices were actually used
    // in the strong classifier
    bool *strongFtrIndices;

    WeakClassifier* get_best_classifier();
    double weight_classifier(double err);
    bool update_feature_weight(TrainingData &td, WeakClassifier &wc);
    bool is_classifier_correct(WeakClassifier &wc, FeatureVector &fv);
    void init_feature_weight(TrainingData &td);
    void create_feature_views(TrainingData &td);
};

#endif
