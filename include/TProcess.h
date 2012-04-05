#include "TConfig.h"#include <map>#include <vector>#include <TApplication.h>#include <TCanvas.h>#include <TGraph.h>#include <TF1.h>#include "data_storage.h"#include "TConfig.h"#ifndef TProcess_h#define TProcess_h 1using namespace std;class TProcess{public:   //TProcess();   TProcess(TConfig &);   ~TProcess();private: //Methods    int parse(); //     int discomment(stringstream &, char); // drop out comments (all after char)    string discomment(string, char );    int ignore_endl(string &); // ignore all end of line symbols in parsing string    int run();private: //Variables   TConfig conf;   vector<string> format;   vector<double> data; // container for read data   map<string, double> var; // <format, data>   map<int, vector<TF1> > impulse; // <selector, transformation>, here impulse[selector][0] is average value;                                   // impulse[selector][1] is dispersion.   map<string, vector<TF1> > asc; // amplitude-space characteristic <filename, transformation>   map<string, vector<double> > output;//this one will contain data of each output   ::DataStorage * ds;  //    };#endif