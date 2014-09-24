// tests some circle-related stuff -- currently kind of broken

#include "circles.h"
#include "box.h"

using namespace std;

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

	circle *box = create_box(num_pos, num_neg);
	write_circle(box, "../../adaboost/out.box");

	box->clear();	delete box;
	return 0;
}
