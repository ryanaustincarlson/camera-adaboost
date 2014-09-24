// tests stuff

#include <ctime>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "WeakClassifier.h"
#include "FeatureVector.h"
#include "TrainingData.h"
#include "circles.h"

using namespace std;

void test_classes();
bool ret_true();

int main(){

	srand(time(NULL));

	int isTrue = ret_true() ? 1 : -1;
	printf("isTrue: %d\n",isTrue);

	TrainingData td = create_circle(2,2);
	td.printData();

}

bool ret_true(){
	return true;
}

void test_classes(){
	vector<float> vec;
	vec.push_back(2.36);
	vec.push_back(5.71);
	vec.push_back(4.89);

	vector<float> v2;
	v2.push_back(1.11);
	v2.push_back(3.33);
	v2.push_back(5.55);

	FeatureVector fv(vec, 1);
	//fv.printFeature();
	FeatureVector fv2(v2, 2);

	TrainingData td;
	if (!td.addFeature(fv))
		printf("error\n");
	if (!td.addFeature(fv2))
		printf("error\n");
	td.printData();
	
	WeakClassifier wc(3, 0.4, true, 30);
	wc.printClassifier();
}
