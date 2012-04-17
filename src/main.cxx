#include "Config.h"
#include "Process.h"

int main(int argc, char * argv[]){
   Config conf;
   //conf.saveToXML("conf.xml");
   conf.loadFromXML("conf.xml");

   Process pr(conf);
   pr.run();

   
}
