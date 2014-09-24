#include "clicklabel.h"

ClickLabel::ClickLabel(QWidget* parent, Qt::WindowFlags f)
  : QLabel(parent, f)
{
  
}

ClickLabel::~ClickLabel()
{

}

void ClickLabel::mousePressEvent(QMouseEvent* e)
{
  QLabel::mousePressEvent(e);
  emit clicked(e->pos());
}
