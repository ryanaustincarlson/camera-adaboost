#include <cmath>
#include <cstdio>
#include "HaarFeature.h"

HaarFeature::HaarFeature(int rows, int cols, haar_type type) :
  pos(1), neg(-1), zero(0) {
  // set rows/cols
  num_rows = rows;
  num_cols = cols;

  // allocate a 2D array
  feature = new int*[num_rows];
  for (int r=0; r<num_rows; r++)
    feature[r] = new int[num_cols];

  // call helper function
  switch(type){
    case VERT_EDGE:
      vert_edge();
      break;
    case HORIZ_EDGE:
      horiz_edge();
      break;
    case VERT_RIGHT_EDGE:
      vert_right_edge();
      break;
    case VERT_LEFT_EDGE:
      vert_left_edge();
      break;
    case HORIZ_RIGHT_EDGE:
      horiz_right_edge();
      break;
    case HORIZ_LEFT_EDGE:
      horiz_left_edge();
      break;
    case CHECKERBOARD:
      checkerboard();
      break;
    case VERT_THIN_LINE:
      vert_thin_line();
      break;
    case VERT_THICK_LINE:
      vert_thick_line();
      break;
    case HORIZ_THIN_LINE:
      horiz_thin_line();
      break;
    case HORIZ_THICK_LINE:
      horiz_thick_line();
      break;
    case VERT_RIGHT_THIN_LINE:
      vert_right_thin_line();
      break;
    case VERT_LEFT_THIN_LINE:
      vert_left_thin_line();
      break;
    case VERT_RIGHT_THICK_LINE:
      vert_right_thick_line();
      break;
    case VERT_LEFT_THICK_LINE:
      vert_left_thick_line();
      break;
  };
}

// copy constructor TODO: check with Andy -- is this right?
HaarFeature::HaarFeature(const HaarFeature &other) :
  num_rows(other.num_rows), num_cols(other.num_cols),
  pos(other.pos), neg(other.neg), zero(other.zero) {

  feature = new int*[other.num_rows];
  for (int r=0; r<other.num_rows; r++)
    feature[r] = new int[other.num_cols];

  for (int r=0; r<other.num_rows; r++){
    for (int c=0; c<other.num_cols; c++){
      feature[r][c] = other.feature[r][c];
    }
  }
}

HaarFeature::~HaarFeature(){
  for(int r=0; r<num_rows; r++){
    delete [] feature[r];
  }
  delete [] feature;
  feature = NULL;
}

void HaarFeature::printHaar(){
  printf("rows: %d, cols: %d\n",num_rows,num_cols);
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      printf("%d\t",feature[r][c]);
    }
    printf("\n");
  }
}

// fills in feature with values for a vertical edge feature
void HaarFeature::vert_edge(){
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      // if left of center, make values pos
      if (c < num_cols/2)
        feature[r][c] = pos;
      // if right of center, make values neg
      else if (c > num_cols/2)
        feature[r][c] = neg;
      // if along center, make values zero
      else
        feature[r][c] = zero;
    }
  }
}

// fills in feature with values for a horizontal edge feature
void HaarFeature::horiz_edge(){
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      // if above center, make values pos
      if (r < num_rows/2)
        feature[r][c] = pos;
      // if below center, make values neg
      else if (r > num_rows/2)
        feature[r][c] = neg;
      // if along center, make values zero
      else
        feature[r][c] = zero;
    }
  }
}

// fills in feature with values for a diamond -- vertical right feature
void HaarFeature::vert_right_edge(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_col, align;
  // create a horiz edge feature and superimpose it on our feature
  HaarFeature ft(num_cols, num_cols, HORIZ_EDGE);
  for (int r=0; r<num_cols; r++){ // note: num_cols here is on purpose TODO: explain
    for (int c=0; c<num_cols; c++){
      align = r+(num_rows-num_cols)/2;
      abs_col = fabs(num_cols/2 - c);
      // if left of center
      if (c < num_cols/2 && align+abs_col < num_rows)
        feature[align+abs_col][c] = ft.feature[r][c];
      // if right of center
      else if (c > num_cols/2 && align-abs_col >= 0)
        feature[align-abs_col][c] = ft.feature[r][c];
      // else, along center
      else
        feature[align][c] = ft.feature[r][c];
    }
  }
}

// fills in feature with values for a diamond -- vertical left feature
void HaarFeature::vert_left_edge(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_col, align;
  // create a horiz edge feature and superimpose it on our feature
  HaarFeature ft(num_cols, num_cols, HORIZ_EDGE);
  for (int r=0; r<num_cols; r++){ // note: num_cols here is on purpose TODO: explain
    for (int c=0; c<num_cols; c++){
      align = r+(num_rows-num_cols)/2;
      abs_col = fabs(num_cols/2 - c);
      // if left of center
      if (c < num_cols/2 && align-abs_col < num_rows)
        feature[align-abs_col][c] = ft.feature[r][c];
      // if right of center
      else if (c > num_cols/2 && align+abs_col >= 0)
        feature[align+abs_col][c] = ft.feature[r][c];
      // else, along center
      else
        feature[align][c] = ft.feature[r][c];
    }
  }
}

// fills in feature with values for a diamond -- horizontal right feature
void HaarFeature::horiz_right_edge(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_row, align;
  // create a horiz edge feature and superimpose it on our feature
  HaarFeature ft(num_rows, num_rows, HORIZ_EDGE);
  for (int r=0; r<num_rows; r++){ 
    for (int c=0; c<num_rows; c++){ // note: num_rows here is on purpose TODO: explain
      align = c+(num_cols-num_rows)/2;
      abs_row = fabs(num_rows/2 - r);
      // if left of center
      if (r < num_rows/2 && align+abs_row < num_cols)
        feature[r][align+abs_row] = ft.feature[r][c];
      // if right of center
      else if (r > num_rows/2 && align-abs_row >= 0)
        feature[r][align-abs_row] = ft.feature[r][c];
      // else, along center
      else
        feature[r][align] = ft.feature[r][c];
    }
  }
}

// fills in feature with values for a diamond -- horizontal right feature
void HaarFeature::horiz_left_edge(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_row, align;
  // create a horiz edge feature and superimpose it on our feature
  HaarFeature ft(num_rows, num_rows, HORIZ_EDGE);
  for (int r=0; r<num_rows; r++){ 
    for (int c=0; c<num_rows; c++){ // note: num_rows here is on purpose TODO: explain
      align = c+(num_cols-num_rows)/2;
      abs_row = fabs(num_rows/2 - r);
      // if left of center
      if (r < num_rows/2 && align-abs_row < num_cols)
        feature[r][align-abs_row] = ft.feature[r][c];
      // if right of center
      else if (r > num_rows/2 && align+abs_row >= 0)
        feature[r][align+abs_row] = ft.feature[r][c];
      // else, along center
      else
        feature[r][align] = ft.feature[r][c];
    }
  }
}

// fills in feature with values for a checkerboard feature
void HaarFeature::checkerboard(){
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      // if along center lines, mark as zero
      if (r == num_rows/2 || c == num_cols/2)
        feature[r][c] = zero;
      // if in upper left corner, mark as pos
      else if (r < num_rows/2 && c < num_cols/2)
        feature[r][c] = pos;
      // if in lower right, mark as pos 
      else if (r > num_rows/2 && c > num_cols/2)
        feature[r][c] = pos;
      // otherwise (upper right, lower left), mark as neg
      else
        feature[r][c] = neg;
    }
  }
}

// fills in feature with values for a thin vertical line feature
void HaarFeature::vert_thin_line(){
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      // if in first 3rd, mark as pos
      if (c < num_cols/3)
        feature[r][c] = pos;
      // if in 2nd 3rd, mark as neg
      else if (c < 2*num_cols/3)
        feature[r][c] = neg;
      // else in 3rd 3rd, mark as pos
      else
        feature[r][c] = pos;
    }
  }
}

// fills in feature with values for a thick vertical line feature
void HaarFeature::vert_thick_line(){
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      // if in first quarter, mark as pos
      if (c < num_cols/5)
        feature[r][c] = pos;
      // if in 2nd/3rd quarter, mark as neg
      else if (c < 4*num_cols/5)
        feature[r][c] = neg;
      // else in last quarter, mark as pos
      else
        feature[r][c] = pos;
    }
  }
}

// fills in feature with values for a thin horizontal line feature
void HaarFeature::horiz_thin_line(){
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      // if in first 3rd, mark as pos
      if (r < num_rows/3)
        feature[r][c] = pos;
      // if in 2nd 3rd, mark as neg
      else if (r < 2*num_rows/3)
        feature[r][c] = neg;
      // else in 3rd 3rd, mark as pos
      else
        feature[r][c] = pos;
    }
  }
}

// fills in feature with values for a thick horizontal line feature
void HaarFeature::horiz_thick_line(){
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      // if in first quarter, mark as pos
      if (r < num_rows/5)
        feature[r][c] = pos;
      // if in 2nd/3rd quarter, mark as neg
      else if (r < 4*num_rows/5)
        feature[r][c] = neg;
      // else in last quarter, mark as pos
      else
        feature[r][c] = pos;
    }
  }
}

// fills in feature values for a thin vertial-right line feature
void HaarFeature::vert_right_thin_line(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_col, align;
  // create a vert thin line feature and superimpose it on our feature
  HaarFeature ft(num_cols/3,num_cols, VERT_THIN_LINE);
  for (int r=0; r<num_cols/3; r++){
    for (int c=0; c<num_cols; c++){
      align = r+num_cols/2;
      abs_col = fabs(num_cols/2-c);
      // if left of center
      if (c < num_cols/2 && align+abs_col < num_rows)
        feature[align+abs_col][c] = ft.feature[r][c];
      // if right of center
      else if (c > num_cols/2 && align-abs_col >= 0)
        feature[align-abs_col][c] = ft.feature[r][c];
      // else, along center
      else
        feature[align-abs_col][c] = ft.feature[r][c];
    }
  }
}

// fills in feature values for a thin vertical-left line feature
void HaarFeature::vert_left_thin_line(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_col, align;
  // create a vert thin line feature and superimpose it on our feature
  HaarFeature ft(num_cols/3,num_cols, VERT_THIN_LINE);
  for (int r=0; r<num_cols/3; r++){
    for (int c=0; c<num_cols; c++){
      //align = (num_rows)/2-r;
      align = r+num_cols/2;
      abs_col = fabs(num_cols/2-c);
      // if left of center
      if (c < num_cols/2 && align-abs_col >= 0)
        feature[align-abs_col][c] = ft.feature[r][c];
      // if right of center
      else if (c > num_cols/2 && align+abs_col < num_rows)
        feature[align+abs_col][c] = ft.feature[r][c];
      // else, along center
      else
        feature[align+abs_col][c] = ft.feature[r][c];
    }
  }
}

// fills in feature values for a thick vertical-right line feature
void HaarFeature::vert_right_thick_line(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_col, align;
  // create a vert thin line feature and superimpose it on our feature
  HaarFeature ft(num_rows-num_cols+1,num_cols, VERT_THICK_LINE);
  for (int r=0; r<num_rows-num_cols+1; r++){
    for (int c=0; c<num_cols; c++){
      align = r+num_cols/2;
      abs_col = fabs(num_cols/2-c);
      // if left of center
      if (c < num_cols/2 && align+abs_col < num_rows)
        feature[align+abs_col][c] = ft.feature[r][c];
      // if right of center
      else if (c > num_cols/2 && align-abs_col >= 0)
        feature[align-abs_col][c] = ft.feature[r][c];
      // else, along center
      else
        feature[align-abs_col][c] = ft.feature[r][c];
    }
  }
}
  
// fills in feature values for a thick vertical-right line feature
void HaarFeature::vert_left_thick_line(){
  // initialize all indices to zero
  for (int r=0; r<num_rows; r++){
    for (int c=0; c<num_cols; c++){
      feature[r][c] = 0;
    }
  }

  int abs_col, align;
  // create a vert thin line feature and superimpose it on our feature
  HaarFeature ft(num_rows-num_cols+1,num_cols, VERT_THICK_LINE);
  for (int r=0; r<num_rows-num_cols+1; r++){
    for (int c=0; c<num_cols; c++){
      align = r+num_cols/2;
      abs_col = fabs(num_cols/2-c);
      // if left of center
      if (c < num_cols/2 && align-abs_col >= 0)
        feature[align-abs_col][c] = ft.feature[r][c];
      // if right of center
      else if (c > num_cols/2 && align+abs_col < num_rows)
        feature[align+abs_col][c] = ft.feature[r][c];
      // else, along center
      else
        feature[align+abs_col][c] = ft.feature[r][c];
    }
  }
}

  

// getter methods
int HaarFeature::ftr(int r, int c) { return feature[r][c]; }
int HaarFeature::rows(){ return num_rows; }
int HaarFeature::cols(){ return num_cols; }
