#include <iostream>
#include <fstream>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "Config.h"

Config::Config(){

//filling configuration file template
   data_source = "./save data/2d7322c4-ef8b-45cd-8ffd-bc156ce6efcd/2d7322c4-ef8b-45cd-8ffd-bc156ce6efcd.xml \n # here place path to data storage configuration file";
   seed = 0;
   dt = 0.001;
   read = " deposit globalTime index particleName #list of used varibles. Also, ones have to be inevitably defined at data storage configuration file. You may use them and predefined x varible below to define transformation";
   selector = "particleName  # denotes varible which responds to particle type. Till one doesn't specify it, the impulse transformation (tag named first) uses 0 always";
   time = "globalTime #if this is not specified impulse won't generate. Only amplitude-space transformation will be applied";

   impulse[0].push_back(" x + deposit  #function which describes impulse form");
   impulse[0].push_back("10 # x changes from 0 to this value, i.e. this is duration");
   //impulse[0].push_back("exp(energy) #dispersion example");
      asc["output001.dat"].push_back(" globalTime + x^2 #amplitude-space characteristic");
   asc["output001.dat"].push_back(" 0 #dispersion");

   asc["output002.dat"].push_back(" deposit + sin(globalTime) #amplitude-space characteristic");
   asc["output002.dat"].push_back(" exp(-deposit) #dispersion");
   
   write = "globalTime # additional output. If time doesn't specified this cause just copy corresponding values to output file, otherwise meaningful only time varible";
}

Config::Config(const char * fname){
//Load configuration file
   loadFromXML(fname);
}


int Config::saveToXML(const char * _fname){
   std::ofstream fout(_fname);
   boost::archive::xml_oarchive oa(fout);
   oa << boost::serialization::make_nvp("transformation", *this);
   fout.close();

   return 0;
}

int Config::loadFromXML(const char _fname[]){
   std::ifstream fin(_fname);
   boost::archive::xml_iarchive ia(fin);
   ia >> boost::serialization::make_nvp("transformation", *this);
   fin.close();
   
   return 0;
}



