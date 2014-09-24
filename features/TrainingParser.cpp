#include <stdexcept>

#include <QFile>
#include <QTextStream>
#include <QImage>

#include "TrainingParser.h"

TrainingParser::TrainingParser()
{

}

QList< ImageTransforms* > TrainingParser::parseFile(QString fname)
{
  QFile file(fname);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
    char errmsg[100+fname.size()]; 
    sprintf(errmsg, "In TrainingParser::parseFile, cannot open '%s'", fname.toStdString().c_str());
    throw std::runtime_error(errmsg);
  }

  QTextStream fstream(&file);

  QList<ImageTransforms*> tImgsList;
  int trainingType;
  QString fname1, fname2;
  int xFocus, yFocus;
  int width, height;
  while (!fstream.atEnd()){
    fstream >> trainingType >> fname1 >> fname2 >> xFocus >> yFocus >> width >> height;

    // since there's probably a newline at end of file, we check if
    // all vars are valid before moving on. SuperHacky? maybe.
    if (fname1.isNull() || fname2.isNull() || width == 0 || height == 0){
      continue;
    }

    QImage img1(fname1);
    img1 = img1.scaled(width, height);

    QImage img2(fname2);
    img2 = img2.scaled(width, height);

    tImgsList.append(new ImageTransforms(img1, img2, trainingType, xFocus, yFocus));
  }
  file.close();
  
  return tImgsList;
}

