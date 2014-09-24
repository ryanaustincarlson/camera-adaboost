#include <cmath>

#include "utils.h"

/*************************************
 * Function: cascadeTheshold
 * -------------------------
 * returns cascade threshold given by
 *
 *    1 / 2.5*(1 + e ^ -( idx - numclassifiers ) ) + 0.1
 *
 * which is in the range y=.1 to y=.5 centered at x=numclassifiers/2
 */
double cascadeThreshold(int classifierIdx, int numclassifiers)
{
  return 1. / (2.5 * ( 1 + exp( -( classifierIdx - numclassifiers/2 ) ) ) ) + 0.1;
}


/**********************************************
 * Function: normalizeClassification
 * ---------------------------------
 * normalizes classification ( H(x) in eq'n ) 
 * value according to 
 *
 *    1 / (1 + e ^ (- H(x) ) )
 */
double normalizeClassification(double classify)
{
  return 1. / ( 1 + exp( -classify ) );
}
