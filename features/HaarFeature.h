#ifndef _HAAR_FEATURE_H
#define _HAAR_FEATURE_H

typedef enum haar_type_t {
  VERT_EDGE = 0,
  HORIZ_EDGE,
  VERT_RIGHT_EDGE,
  VERT_LEFT_EDGE,
  HORIZ_RIGHT_EDGE,
  HORIZ_LEFT_EDGE,
  CHECKERBOARD,
  VERT_THIN_LINE,
  VERT_THICK_LINE,
  HORIZ_THIN_LINE,
  HORIZ_THICK_LINE,
  VERT_RIGHT_THIN_LINE,
  VERT_LEFT_THIN_LINE,
  VERT_RIGHT_THICK_LINE,
  VERT_LEFT_THICK_LINE,
} haar_type;

class HaarFeature {
  public:
    HaarFeature(int rows, int cols, haar_type type);
    HaarFeature(const HaarFeature &other);
    ~HaarFeature();
    int ftr(int r, int c);
    void printHaar();
    int rows();
    int cols();

  private:
    int num_rows, num_cols;
    int **feature;
    int pos, neg, zero;

    void vert_edge();
    void horiz_edge();
    void vert_right_edge();
    void vert_left_edge();
    void horiz_right_edge();
    void horiz_left_edge();
    void checkerboard();
    void vert_thin_line();
    void vert_thick_line();
    void horiz_thin_line();
    void horiz_thick_line();
    void vert_right_thin_line();
    void vert_left_thin_line();
    void vert_right_thick_line();
    void vert_left_thick_line();
};

#endif
