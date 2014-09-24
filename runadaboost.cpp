#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

#include <QImage>
#include <QColor>
#include <QString>
#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QLabel>

#include "features/ImageTransforms.h"
#include "features/ImageFeatures.h"
#include "features/TrainingParser.h"
#include "adaboost/TrainingData.h"
#include "adaboost/WeakClassifier.h"
#include "adaboost/AdaBooster.h"
#include "tester/ImageTester.h"

using std::vector;
using std::string;
namespace po=boost::program_options;

typedef struct s_popts {
  string trainingPath; // path to file containing training example info (created by gui)
  string testingPathPast;  // path to file from *n* minutes ago
  string testingPathCurrent; // path to file at current time
  string strongclassifier; // path to strong classifier
  string classifiedPath;    // path to output classified file
  int iterations;      // number of feature extraction iterations
  int classifiers;     // number of adaboost weak classifiers
} ProgOpts;

bool parse_args(int argc, char* argv[], ProgOpts& popts);

int main(int argc, char **argv){

  // get command-line arguments
  ProgOpts popts;
  if(!parse_args(argc, argv, popts))
    exit(1);

  AdaBooster ada;
  vector<WeakClassifier> strongClassifier;
  QList<FeatureVector*> ftrvecList;
  
  // if we gave it a training path...
  if (popts.trainingPath.length() != 0){

    qDebug() << "Parsing Training Data";
    QString trainingPath(popts.trainingPath.c_str());
    TrainingParser tparser;
    QList<ImageTransforms*> trainingList = tparser.parseFile(trainingPath);

    qDebug() << "Extracting Feature Vectors";
    ImageFeatures imgftrs;
    ftrvecList = imgftrs.generateFeatureVectors(trainingList, popts.iterations);

    TrainingData td;
    foreach (FeatureVector *fv, ftrvecList){
      td.addFeature(*fv);
    }

    qDebug() << "Training Strong Classifier";
    strongClassifier = ada.getStrongClassifier(td, popts.classifiers);

    if (!popts.strongclassifier.empty()){
      ada.writeStrongClassifier(strongClassifier, popts.strongclassifier);
    }
  }

  ///////////////////////
  // Testing
  //////////////////////

  QImage testImagePast(QString::fromStdString(popts.testingPathPast));
  QImage testImageCur(QString::fromStdString(popts.testingPathCurrent));

  // TODO error if images have different dimensions, etc


  // scale resolution down
  
  int newHeight = 200;
  testImagePast = testImagePast.scaledToHeight(newHeight);
  testImageCur = testImageCur.scaledToHeight(newHeight);
  // */

  if (!popts.strongclassifier.empty()){
    qDebug() << "Reading Strong Classifier From Disk";
    strongClassifier = ada.readStrongClassifier(popts.strongclassifier);
  }

  qDebug() << "Testing Strong Classifier";
  ImageTester tester;
  ImageTransforms testImages(testImagePast, testImageCur, 0, 0, 0);
  Grid classified = tester.testStrongClassifier(testImages, popts.iterations, strongClassifier);

  qDebug() << "Writing Classified Grid to Disk";
  classified.write(QString::fromStdString(popts.classifiedPath));

  return 0;
}

bool parse_args(int argc, char* argv[], ProgOpts& popts){
   string configFile;
   po::options_description desc("Allowed options");
   desc.add_options()
     ("help,h", "Show this message")
     ("config,c", po::value<string>(&configFile), "Config file to read options from")
     ("trainingPath,p", po::value<string>(&popts.trainingPath), "Path to file containing training example info (omit to just read from strongClassifier)")
     ("testingPathPast,a", po::value<string>(&popts.testingPathPast), "Path to file containing testing image from the past")
     ("testingPathCurrent,u", po::value<string>(&popts.testingPathCurrent), "Path to file containing testing image for current timestep")
     ("strongClassifier,t", po::value<string>(&popts.strongclassifier), "Path to strong classifier (omit to avoid writing to disk..requires retraining, though)")
     ("classifiedPath,s", po::value<string>(&popts.classifiedPath), "Path to output classified grid")
     ("iterations,i", po::value<int>(&popts.iterations)->default_value(3), "Number of feature extraction iterations")
     ("classifiers,l", po::value<int>(&popts.classifiers)->default_value(20), "Number of AdaBoost weak classifiers");
   po::variables_map vm;
   po::store(po::parse_command_line(argc, argv, desc), vm);
   po::notify(vm);
   if(vm.count("config")){
     std::ifstream ifs(vm["config"].as<string>().c_str());
     store(parse_config_file(ifs, desc), vm);
     notify(vm);
   }
   po::store(po::parse_command_line(argc, argv, desc), vm);
   po::notify(vm);
   if (vm.count("help") || !vm.count("testingPathPast") ||
     !vm.count("testingPathCurrent") || !vm.count("classifiedPath")){
     std::cout << desc;
     return false;
   }
   if (popts.trainingPath.empty() && popts.strongclassifier.empty()){
     std::cout << "If you don't set the trainingPath, you need a strongClassifier!\n";
     std::cout << desc;
     return false;
   }
   return true;
}

/*
  // Code to view images. Keeping it around for debugging
  // To use it, slap it in main() somewhere
  
  QApplication app(argc, argv);
  
  QWidget diffWindow;
  diffWindow.resize(640,480);
  diffWindow.show();
  
  QWidget sobelWindow;
  sobelWindow.resize(640,480);
  sobelWindow.show();

  // grab image
  QImage diff = blackImgs.diffGrayscale().scaledToHeight(480);
  QImage sobel = blackImgs.sobel().scaledToHeight(480);

  QLabel diffLabel(&diffWindow);
  diffLabel.setPixmap(QPixmap::fromImage(diff));
  diffLabel.show();

  QLabel sobelLabel(&sobelWindow);
  sobelLabel.setPixmap(QPixmap::fromImage(sobel));
  sobelLabel.show();
  
  app.exec();
*/
  