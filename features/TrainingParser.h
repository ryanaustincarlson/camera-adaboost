#ifndef _TRAINING_PARSER_H
#define _TRAINING_PARSER_H

#include <QString>

#include "ImageTransforms.h"

class TrainingParser {
  public:
    TrainingParser();
    QList<ImageTransforms*> parseFile(QString fname);

  private:
    
};

#endif // _TRAINING_PARSER_H