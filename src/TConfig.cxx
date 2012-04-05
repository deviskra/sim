#include <iostream>
#include <fstream>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "TConfig.h"

TConfig::TConfig(){

//filling configuration file template
   id = "2d7322c4-ef8b-45cd-8ffd-bc156ce6efcd";
   seed = 0;
   dt = 0.001;
   read = " time energy particle xx yy zz";
   selector = "particle";

   impulse[0].push_back("time * energy * x + x**4     #impulse function example");
   impulse[0].push_back("0 1 # x interval");
   impulse[0].push_back("exp(energy) #dispersion example");

   asc["output001.dat"].push_back(" xx^3 * yy * zz^2 #amplitude-space characteristic example");
   asc["output001.dat"].push_back("xx * yy * energy #dispersion example");

   asc["output002.dat"].push_back(" (xx - 7) ^3 * exp(- yy) * zz^2 #amplitude-space characteristic example");
   asc["output002.dat"].push_back("xx * yy * energy^3 #dispersion example");
      
}

TConfig::TConfig(const char * fname){
//Load configuration file
   loadFromXML(fname);
}


int TConfig::saveToXML(const char * _fname){
   std::ofstream fout(_fname);
   boost::archive::xml_oarchive oa(fout);
   oa << boost::serialization::make_nvp("transformation", *this);
   fout.close();

   return 0;
}

int TConfig::loadFromXML(const char _fname[]){
   std::ifstream fin(_fname);
   boost::archive::xml_iarchive ia(fin);
   ia >> boost::serialization::make_nvp("transformation", *this);
   fin.close();
   
   return 0;
}



