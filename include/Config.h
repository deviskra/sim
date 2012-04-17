#include <iostream>
#include <fstream>

#include <vector>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#ifndef Config_h
#define Config_h 1


class Config{

public:
   double seed, dt;
   
   std::string data_source, read, selector, time, write;
   
   std::map<std::string, std::vector<std::string> > asc; // <filename, transformation>
                                                    
   std::map<int, std::vector<std::string> > impulse; // <selector, transformation>
  
     
public:

   Config();
   Config(const char *);
   int saveToXML(const char *);
   int loadFromXML(const char *);
   
};


// the code below is only for TConfig class saving to XML configuration file

namespace boost{
   namespace serialization {
   
      template<typename Archive>
      void serialize(Archive &ar, Config &conf, const unsigned int){
         ar & make_nvp("data_source", conf.data_source)
            & make_nvp("discretization", conf.dt)
            & make_nvp("seed", conf.seed)
			
            & make_nvp("read", conf.read)
            & make_nvp("selector", conf.selector)
            & make_nvp("time", conf.time)
            & make_nvp("impulse", conf.impulse)
			
            & make_nvp("amplitude-space", conf.asc)
            & make_nvp("write", conf.write);
      }

}
}
#endif
