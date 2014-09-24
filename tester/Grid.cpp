#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "Grid.h"

// just allocate rows*cols worth of space
Grid::Grid(int rows, int cols) :
  m_rows(rows), m_cols(cols)
{
  allocateGrid();
}

// read grid from file
Grid::Grid(QString filename)
{
  QFile file(filename);
  if (! file.open(QIODevice::ReadOnly | QIODevice::Text) ){
    return;
  }

  QTextStream fileStream(&file);

  QString garbage;
  fileStream >> garbage >> m_rows >> garbage >> m_cols;

  allocateGrid();

  for (int r=0; r<m_rows; r++){
    for (int c=0; c<m_cols; c++){
      fileStream >> m_grid[r][c];
    }
  }
}

void Grid::allocateGrid()
{
  m_grid = new qreal*[m_rows];
  for (int i=0; i<m_rows; i++){
    m_grid[i] = new qreal[m_cols];
  }
}

Grid::~Grid()
{
  for (int i=0; i<m_rows; i++){
    delete [] m_grid[i];
  }
  delete [] m_grid;
}

void Grid::write(QString filename)
{
  QFile file(filename);
  file.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QTextStream fileStream(&file);

  fileStream << "rows: " << m_rows << "\n"; 
  fileStream << "cols: " << m_cols << "\n";

  for (int r=0; r<m_rows; r++){
    for (int c=0; c<m_cols; c++){
      fileStream << m_grid[r][c] << " ";
    }
    fileStream << "\n";
  }
}

void Grid::print()
{
  for (int r=0; r<m_rows; r++){
    for (int c=0; c<m_cols; c++){
      printf("%.2f ", m_grid[r][c]);
    }
    printf("\n");
  }
}

/////////////////
// setters
////////////////

void Grid::set(int row, int col, float val)
{
  m_grid[row][col] = val;
}

/////////////////
// accessors
/////////////////

qreal Grid::val(int row, int col) const
{
  return m_grid[row][col];
}

int Grid::rows() const
{
  return m_rows;
}

int Grid::cols() const
{
  return m_cols;
}
