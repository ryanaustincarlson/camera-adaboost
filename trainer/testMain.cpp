#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QString>

#include "clicklabel.h"

int main(int argc, char** argv){
  QApplication app(argc, argv);
  
  QWidget window;
  window.resize(320, 240);
  window.show();

  // grab image
  QString fname = "/home/ryan/Pictures/General/Bamboo.jpg";
  QPixmap img(fname);
  img = img.scaledToHeight(320);

  QLabel label(&window);
  label.setPixmap(img);
  label.show();

  ClickLabel clabel(&window);

  app.exec();
  
  return 0;
}
