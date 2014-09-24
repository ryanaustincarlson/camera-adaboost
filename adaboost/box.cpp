#include "box.h"

circle *create_box(int num_pos, int num_neg){
	// allocate box
	circle *box = new circle(2*num_pos + 2*num_neg);

	// create positive distribution
	circ_coord coord;
	for (int i=0; i<2*num_pos; i+=2){
		coord.val = POS;

		// top left corner
		coord.x = (float)rand()/RAND_MAX / 2;
		coord.y = 0.5 + (float)rand()/RAND_MAX / 2;

		box->circ[i] = coord;
	
		// bottom right corner
		coord.x = 0.5 + (float)rand()/RAND_MAX / 2;
		coord.y = (float)rand()/RAND_MAX / 2;

		box->circ[i+1] = coord;
	}

	// create negative distribution
	for (int i=2*num_pos; i<box->num_points; i+=2){
		coord.val = NEG;
	
		// top right corner	
		coord.x = 0.5 + (float)rand()/RAND_MAX / 2;
		coord.y = 0.5 + (float)rand()/RAND_MAX / 2;

		box->circ[i] = coord;

		// bottom left corner
		coord.x = (float)rand()/RAND_MAX / 2;
		coord.y = (float)rand()/RAND_MAX / 2;

		box->circ[i+1] = coord;
	}
	return box;
}

line create_box_line(){
	line myline;

	for (int i=0; i<2; i++){
		myline.pts[i].val = ZERO;

		myline.pts[i].x = (float)rand()/RAND_MAX;
		myline.pts[i].y = (float)rand()/RAND_MAX;
	}
	return myline;
}
