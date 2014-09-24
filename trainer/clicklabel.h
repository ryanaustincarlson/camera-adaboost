#ifndef _CLICKLABEL_H
#define _CLICKLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QString>

class ClickLabel : public QLabel
{
  Q_OBJECT
  public:
    ClickLabel(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~ClickLabel();
   
  protected:
    void mousePressEvent(QMouseEvent *e);

  signals:
    void clicked(const QPoint &pos);
};

#endif // _CLICK_LABEL_H