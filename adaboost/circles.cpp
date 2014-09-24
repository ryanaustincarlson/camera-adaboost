// creates circles using TrainingData class implementation

#include <cmath>
#include <cstdlib>
#include "circles.h"

using namespace std;

/***********************************
 * Function: create_circle
 * -----------------------
 * Creates a circular distribution (radius 1) of POS and NEG values, with
 * num_pos POS values inside a radius=1/2, and num_neg NEG values outside that
 * radius
 */
TrainingData create_circle(int num_pos, int num_neg){
	// create TrainingData object (circle, dimension 2)
	TrainingData circle;
	
	// create positive distribution within radius=1/2
	vector <float> coord;
	float radius, theta;
	for (int i=0; i<num_pos; i++){
		// get radius and angle
		radius = sqrt((float)rand()/RAND_MAX)/2;
		theta = 2*M_PI*(float)rand()/RAND_MAX;

		// convert to x and y
		coord.push_back(radius * cos(theta));
		coord.push_back(radius * sin(theta));
		coord.push_back(5.0f);
		//coord.push_back(coord[0]*coord[0] + coord[1]*coord[1]);

		FeatureVector fv(coord, POS);
		circle.addFeature(fv);
		coord.clear();
	}
	// create negative distribution for 1/2 <= radius <= 1
	for (int i=num_pos; i<num_pos+num_neg; i++){
		// get radius and angle
		radius = sqrt(3*(float)rand()/RAND_MAX+1)/2;
		theta = 2*M_PI*(float)rand()/RAND_MAX;
		
		// convert to x and y
		coord.push_back(radius * cos(theta));
		coord.push_back(radius * sin(theta));
		coord.push_back(1.5f);
		//coord.push_back(coord[0]*coord[0] + coord[1]*coord[1]);

		FeatureVector fv(coord, NEG);
		circle.addFeature(fv);
		coord.clear();
	}
	return circle;
}

