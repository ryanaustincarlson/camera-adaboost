// DEFUNCT!
// implements adaboost algorithm
// DEFUNCT!

#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>
#include "circles.h"
#include "box.h"
#include "sorting.h"

using namespace std;

// global variables
const int numClassify = 5;
const float errUpperBound = 0.5;

// output file names
const string circFileName("/home/rcarlso1/summer_09/bridge_gis/adaboost/out.circ");
const string lineFileName("/home/rcarlso1/summer_09/bridge_gis/adaboost/out.line");

// function prototypes
vector<line> run_adaboost(circle *mycirc, unsigned int num_lines);
line *get_best_classifier(circle *mycirc, vector<circ_coord *> *sorted, 
    unsigned int dim);
double weight_classifier(double err);
int update_feature_weight(circle *mycirc, line myline);
vector<double> get_strong_err(circle *mycirc, vector<line> &strong);
int init_feature_weight(circle *mycirc);
vector<line> create_lines(int num_lines);

int main(int argc, char *argv[]){
  // seed random num generator
  srand(time(NULL));

  int num_pos, num_neg;
  num_pos = num_neg = 2; // default values
  if (argc >= 2 && atoi(argv[1])){
    num_pos = atoi(argv[1]);
    if (argc >= 3 && atoi(argv[2]))
      num_neg = atoi(argv[2]);
    else
      num_neg = num_pos;
  }

  circle *c = create_circle(num_pos, num_neg);
  //circle *c = create_box(num_pos, num_neg);

  /*line myline = create_line();
    write_line(myline, "out.line");
    check_line(c, myline);*/

  init_feature_weight(c);
  write_circle(c, circFileName);
  //print_circle(c);

  vector<line> strong_classifier = run_adaboost(c, numClassify);

  vector<double> strong_err = get_strong_err(c, strong_classifier);
  for (unsigned int i=0; i<strong_err.size(); i++)
    printf("err[%d]: %.30f\n",i,strong_err.at(i));

  //
  // below: has been moved temporarily (?) into run_adaboost
  //
  /*	
      for (int i=0; i<(signed)strong_classifier.size(); i++){
  //printf("\tmylines[%d]:\n",i);
  // print_line(mylines.at(i));
  if (i>0)
  append_line(strong_classifier.at(i), lineFileName);
  else
  write_line(strong_classifier.at(i), lineFileName);
  //check_line(c, mylines.at(i));
  //cout << endl;
  }
  */

  c->clear();	delete c;
  return 0;
}

/***********************************
 * Function: run_adaboost INCLASS
 * ----------------------
 * this function combines the basic functions of the adaboost algorithm below
 * so that our code can be cleaner/modular
 *
 * mycirc: set of features
 * mylines: set of classifiers
 * num_classifiers: number of classifiers
 *
 * returns: strong classifier -- set of weak classifiers in optimal order 
 */
vector<line> run_adaboost(circle *mycirc, unsigned int num_lines){
  // create vector of weak classifiers that make up a strong classifier
  vector<line> strong_classifier;

  // sort circle by x and y -- store in sorted
  vector <circ_coord *> *sorted = sort_circle(mycirc);

  //char garbage[80];
  for (unsigned int i=0; i<num_lines; i++){
    // indentify best classifier
    line *myline = get_best_classifier(mycirc, sorted, 2);

    // if index invalid, then we're done constructing our strong classifier
    if (myline == NULL){
      return strong_classifier;
    }

    // otherwise, add best classifier to strong_classifier
    strong_classifier.push_back(*myline);

    // don't need line anymore so delete it
    delete myline;

    write_circle(mycirc, circFileName); // rewrite feature set

    // update weights of features
    update_feature_weight(mycirc, strong_classifier.back());

    // add most recent classifier to lineFileName
    if (i>0)
      append_line(strong_classifier.at(i), lineFileName);
    else
      write_line(strong_classifier.at(i), lineFileName);
    //cin.getline(garbage, 80);

  }

  // delete sorted array
  delete [] sorted;

  return strong_classifier;
}


/***********************************
 * Function: get_best_classifier INCLASS
 * -----------------------------
 * calculates error for all classifiers given a set of classifiers and features
 * and returns the classifier with the lowest error
 *
 * mycirc: set of features
 * mylines: set of classifiers
 * num_classifiers: number of classifiers
 *
 * returns: index of best weak classifier (currently: an elt of mylines)
 * 		if error is too large, returns -1
 */
line *get_best_classifier(circle *mycirc, vector<circ_coord *> *sorted, unsigned int dim){
  // this will hold the best classifier and lowest error so far
  double lowest_err = 1;
  unsigned int index = 0;
  unsigned int dimension = 0;
  double tmp_pos_err, tmp_neg_err;

  //
  // by default, vertical lines face "left" and horizontal lines face "down".
  // if we need to flip that to get a better result, we tag is_flipped as true
  //
  bool is_flipped = false;
  double flipped_err;

  // get total weight for POS and NEG values
  double pos_wt = 0, neg_wt = 0;
  for (unsigned int i=0; i<sorted[0].size(); i++){
    if (sorted[0][i]->val == POS)
      pos_wt += sorted[0][i]->weight;
    else
      neg_wt += sorted[0][i]->weight;
  }

  //printf("\npos: %f, neg: %f\n",pos_wt,neg_wt); // TODO: remove

  // traverse the dimensions of the features
  for (unsigned int i=0; i<dim; i++){

    // since, at j=0, we correctly classify all NEG values and incroccetly
    // classify all POS values, we initialize our tmp values as such
    tmp_pos_err = pos_wt; 
    tmp_neg_err = 0;

    // traverse features
    for (unsigned int j=0; j<sorted[i].size(); j++){
      // if we correctly classified the point, decrement tmp_pos_err
      if (sorted[i][j]->val == POS)
        tmp_pos_err -= sorted[i][j]->weight;
      else // otherwise increment tmp_neg_err
        tmp_neg_err += sorted[i][j]->weight;

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

  /*printf("lowest: (%f, %f)\n",sorted[dimension][index]->x,
    sorted[dimension][index]->y);// TODO: remove
    if (dimension == 1)
    printf("horizontal, ");
    else
    printf("vertical, ");
    if (is_flipped)
    printf("flipped\n");
    else
    printf("not flipped\n");*/

  // if error is too high, return -1
  if (lowest_err >= errUpperBound){
    printf("Error in get_best_classifier: error too large\n");
    return NULL;
  }

  // TODO: not sure how this will translate in bridge detection. kind of hard-coded
  // it here...

  // set instances of new lines
  line *myline = new line();
  myline->val = ZERO;
  if (dimension == 0){ // vertical lines
    myline->pts[0].x = myline->pts[1].x = sorted[dimension][index]->x;
    myline->pts[0].y = -1; 
    myline->pts[1].y = 1;
    if (is_flipped){
      myline->pts[0].y *= -1;
      myline->pts[1].y *= -1;
    }
  } else { // horizontal lines
    myline->pts[0].y = myline->pts[1].y = sorted[dimension][index]->y;
    myline->pts[0].x = 1;
    myline->pts[1].x = -1;
    if (is_flipped){
      myline->pts[0].x *= -1;
      myline->pts[1].x *= -1;
    }
  }

  // get new weight and return
  myline->weight = weight_classifier(lowest_err);
  return myline;
}

/**************************************
 * Function: weight_classifier INCLASS
 * ---------------------------
 * given an error value, we compute and return the weight that should be
 * applied to a classifier using the formula found on the wikipedia page for
 * alpha(sub)t
 *
 * err: error used in weighting
 *
 * returns: weight as computed using equation
 */
double weight_classifier(double err){
  // calculate new weight	
  double weight = 0.5 * log( (1-err) / err );
  //printf(" error: %.15f, new weight: %f\n", err, weight);
  return weight;
}

/*************************************
 * Function: update_feature_weight INCLASS
 * -------------------------------
 * Given a set of features and a classifier that has been weighted in
 * get_best_classifier(), we recalculate the weights of all the features
 *
 * mycirc: set of features
 * myline: set of classifiers
 *
 * returns true if successful, false otherwise
 */
int update_feature_weight(circle *mycirc, line myline){
  // check that mycirc has been initialized
  if (!mycirc){
    printf("Error in update_feature_weight: mycirc is NULL\n");
    return false;
  }

  // check that myline has actually been weighted
  if (myline.weight < 0){
    printf("Error in update_feature_weight: myline has invalid weight\n");
    return false;
  }

  // traverse features in feature set and adjust their weights
  for (int i=0; i<mycirc->num_points; i++){
    mycirc->circ[i].weight *= pow(M_E, (double)-1 * myline.weight * 
        is_line_correct(myline, mycirc->circ[i]));
  }

  // calculate normalization factor
  float norm = 0;
  for (int i=0; i<mycirc->num_points; i++)
    norm += mycirc->circ[i].weight;

  // normalize feature weights
  for (int i=0; i<mycirc->num_points; i++)
    mycirc->circ[i].weight /= norm;

  return true; // successful
}

/**************************************
 * Function: get_strong_err
 * ------------------------
 * calculates error rates at each "level" of the strong classifier; i.e. at
 * each weak classifier
 *
 * strong: strong classifier (i.e. a list of weak classifiers)
 *
 * returns: a list of the errors at each level
 */
vector<double> get_strong_err(circle *mycirc, vector<line> &strong){
  vector<double> strong_err;

  // initialize vector of num_points to zero
  vector<double> classify;
  for (int i=0; i<mycirc->num_points; i++)
    classify.push_back(0);

  // traverse all weak classifiers
  for (unsigned int i=0; i<strong.size(); i++){
    float num_errors = 0;
    // traverse points in circle
    for (int j=0; j<mycirc->num_points; j++){
      // calculate classify so far
      classify.at(j) += strong.at(i).weight * 
        left_of(strong.at(i), mycirc->circ[j]);

      // check if classify was "correct"
      if (!(classify.at(j) > 0 && mycirc->circ[j].val == 1 ||
            classify.at(j) < 0 && mycirc->circ[j].val == -1))
        num_errors++;
    }
    // push error % into vector
    strong_err.push_back(num_errors/mycirc->num_points);
  }
  return strong_err;
}


/**************************************
 * Function: init_feature_weight INCLASS
 * -----------------------------
 * Initializes weights of features to be 1/(total num of features)
 *
 * mycirc: set of features
 *
 * returns true if successful, false otherwise
 */
int init_feature_weight(circle *mycirc){
  // check that mycirc has been initialized
  if (!mycirc){
    printf("Error in init_feature_weight: mycirc is NULL\n");
    return false;
  }

  // set all points to same weight = 1/num_points
  int num_points = mycirc->num_points;
  for (int i=0; i<num_points; i++)
    mycirc->circ[i].weight = 1./num_points;
  return true;
}

/******************************
 * Function: create_lines
 * ----------------------
 * creates a list of randomly generated lines (classifiers) and returns them
 *
 * num_lines: number of lines to create
 *
 * returns: vector of (currently: randomly) generated 
 * 		classifiers (currently: lines)
 */
vector<line> create_lines(int num_lines){
  vector<line> mylines(0);

  for (int i=0; i<num_lines; i++)
    //mylines.push_back(create_smart_line());
    //mylines.push_back(create_line());
    mylines.push_back(create_box_line());

  return mylines;
}
