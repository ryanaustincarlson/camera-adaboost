// adaboost test program

#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include "AdaBooster.h"
#include "circles.h"
#include "terrain/grid.h"

using namespace std;

#define NUM_FEATURES 12

grid *test_strong_classifier(grid *ingrid, TrainingData td, vector<WeakClassifier> strong);
TrainingData read_ftrs(string filename);

int main(int argc, char *argv[]){
	// seed random num generator
	srand(time(NULL));

	int num_pos, num_neg;
	num_pos = num_neg = 100; // default values
	if (argc >= 2 && atoi(argv[1])){
		num_pos = atoi(argv[1]);
		if (argc >= 3 && atoi(argv[2]))
			num_neg = atoi(argv[2]);
		else
			num_neg = num_pos;
	}

	//TrainingData circle = create_circle(num_pos, num_neg);
	TrainingData td = read_ftrs(
			"/home/rcarlso1/summer_09/bridge_gis/trunk/extraction/ftrs");
	//td.printData();

	grid *ingrid = read_grid("/scratch/rcarlso1/bridge_test_sobel");
	
	AdaBooster ada;

	vector<WeakClassifier> strong;

	//ada.runAdaBoost(circle, 50);
	strong = ada.getStrongClassifier(td, 10);
	exit(2);

	TrainingData test_data = read_ftrs(
			"/home/rcarlso1/summer_09/bridge_gis/trunk/extraction/ftrs_test");

	grid *mygrid = test_strong_classifier(ingrid, test_data, strong);
	//write_grid(mygrid, "/home/rcarlso1/summer_09/bridge_gis/trunk/extraction/classified");
	write_grid(mygrid, "/scratch/rcarlso1/bridge_classified");
	/*
	float tmp;
	for (int r=0; r<mygrid->rows; r++){
		for (int c=0; c<mygrid->cols; c++){
			tmp = mygrid->pixels[r][c];
			if (tmp > 3)
				printf("[%d][%d] = %f\n",r,c,tmp);
		}
	}

	printf("DONE\n");*/

	return 0;
}

///////////////////////////////////////////
//
//	COPIED FROM GET_STRONG_ERROR()
//
///////////////////////////////////////////
grid *test_strong_classifier(grid *mygrid, TrainingData td, vector<WeakClassifier> strong){
	double classify;
	int sign;

	// copy header of mygrid to outgrid
	grid *outgrid = new grid();
	copy_header(outgrid, mygrid);
	
	//Allocate grid
	outgrid->pixels = new float*[outgrid->rows];
	for(int r=0; r<outgrid->rows; r++){
		outgrid->pixels[r] = new float[outgrid->cols];
	}

	//printf("outgrid rows: %d, cols: %d\n",outgrid->rows, outgrid->cols); // TODO: remove

	// traverse all features
	for (unsigned int j=0; j<td.size(); j++){
		// traverse all weak classifiers
		classify = 0.0;
		for (unsigned int i=0; i<strong.size(); i++){
			// check what the classifier guessed. If weak classifier decided
			// the feature was POS, sign = 1, otherwise sign = -1
			if ( (strong[i].threshold() > td.at(j,strong[i].dimension()) && !strong[i].isFlipped()) ||
					(strong[i].threshold() < td.at(j,strong[i].dimension()) && strong[i].isFlipped()) )
				sign = 1;
			else
				sign = -1;

			// calculate classify so far
			classify += strong[i].weight() * sign;
		}
		// write classify to correct location in outgrid
		//printf("row: %d, col: %d\n",j/outgrid->cols,j%outgrid->cols); // TODO: remove
		outgrid->pixels[j/outgrid->cols][j%outgrid->cols] = classify;
		//outgrid->pixels[j/outgrid->cols][j%outgrid->cols] = td.at(j,
	}

	return outgrid;
}

TrainingData read_ftrs(string filename){
	ifstream inFile(filename.c_str());

	TrainingData ftrs;

	int pos_neg, size;
	vector <float> feat;
	float val;
	inFile >> size;
	for (int i=0; i<size; i++){
		inFile >> pos_neg;
		for (int i=0; i<NUM_FEATURES; i++){
			inFile >> val;
			feat.push_back(val);
		}

		FeatureVector fv(feat, pos_neg);
		ftrs.addFeature(fv);
		feat.clear();
	}

	inFile.close();
	return ftrs;
}
