#include <string>
#include <fstream>
#include <iostream>
#include <QtGui/QApplication>
#include <boost/program_options.hpp>
#include "classifiedviewer.h"

using std::string;
namespace po=boost::program_options;

typedef struct s_popts {
  string testImage; 
  string classifiedGrid; 
} ProgOpts;

bool parse_args(int argc, char* argv[], ProgOpts& popts);

int main(int argc, char *argv[])
{
  // get command-line arguments
  ProgOpts popts;
  if(!parse_args(argc, argv, popts))
    exit(1);
  
  QApplication a(argc, argv);
  
  ClassifiedViewer w;
  
  if (!popts.testImage.empty()){
    w.setTestImage(QString::fromStdString(popts.testImage));
  }

  if (!popts.classifiedGrid.empty()){
    w.setClassifiedGrid(QString::fromStdString(popts.classifiedGrid));
  }
  
  w.show();
  return a.exec();
}

bool parse_args(int argc, char* argv[], ProgOpts& popts){
   string configFile;
   po::options_description desc("Allowed options");
   desc.add_options()
     ("help,h", "Show this message")
     ("config,c", po::value<string>(&configFile), "Config file to read options from")
     ("testImage,t", po::value<string>(&popts.testImage), "Test Image File")
     ("classifiedGrid,g", po::value<string>(&popts.classifiedGrid), "Classified Grid File");
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
   if (vm.count("help")){
     std::cout << desc;
     return false;
   }
   return true;
}