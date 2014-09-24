#ifndef _GRID_H_ // worried about #define from other class
#define _GRID_H_

#include <QString>

class Grid {
  public:
    Grid(int rows, int cols);
    Grid(QString filename);
    ~Grid();

    void write(QString filename);
    
    void set(int row, int col, float val);
    void print();

    qreal val(int row, int col) const;
    int rows() const;
    int cols() const;

  private:
    qreal **m_grid;
    int m_rows, m_cols;

    void allocateGrid(); // only call this from the constructor!
};

#endif // _GRID_H_