#include "TProcess.h"

TProcess::TProcess(TConfig & _conf) : conf(_conf) {
     ds = new DataStorage("./save data/" + conf.id + '/' + conf.id + ".xml");
     parse();
     run();
}

TProcess::~TProcess(){
     delete ds;
}

//TProcess::TProcess(){
//}

int TProcess::ignore_endl(string &str){
    stringstream ss(str);
    str.clear();
    string s;
    while(getline(ss, s)){
        str+=s;
    };
  return 0;
}

int TProcess::discomment(stringstream & ss, char ch = '#'){
    string s;
    getline(ss, s, ch);
    ss.clear(); //unset error flag eofbit
    
    ignore_endl(s);
    ss.str(s);

 return 0; 
}

string TProcess::discomment(string str, char ch = '#'){
    stringstream ss(str);
    string s;
    getline(ss, s, ch);
    ignore_endl(s);
 return s; 
}


int TProcess::parse(){
//clearing shared varibles for a case of repetitive calls
var.clear();
format.clear();
impulse.clear();

    stringstream ss;
    string s;
    
//get available variables
    ss << conf.read; discomment(ss);
    while(ss >> s){
           var[s] = 0;
    }
    ss.str(string());ss.clear(); s.clear();


{  // set up corresponding alias for ROOT parameters [0], [1], ...
   int i = 0; // current parameter number
   for(map<string, double>::iterator it = var.begin(); it != var.end();++it, ++i){
       ss << '[' << i << ']';
       format.push_back(it->first); //set up user data format
       new TFormula(it->first.c_str(), ss.str().c_str());
       ss.str(string());
    }

}

//parse impulse functions
{ double xmin, xmax;
   for(map<int, vector<string> >::iterator it = conf.impulse.begin(); it != conf.impulse.end();++it){
       ss << discomment(it->second[1]);
       ss >> xmin >> xmax;
       
       impulse[it->first].push_back(TF1("", discomment(it->second[0]).c_str(), xmin, xmax));
       impulse[it->first].push_back(TF1("", discomment(it->second[2]).c_str() ));

        ss.str(string());
    }
}

//parse amplitude-space functions
for(map<string, vector<string> >::iterator it = conf.asc.begin(); it != conf.asc.end();++it){
    
       asc[it->first].push_back(TF1("", discomment(it->second[0]).c_str() ));
       asc[it->first].push_back(TF1("", discomment(it->second[1]).c_str() ));
       
       output[it->first] = vector<double>();//this one will contain data of each output

       ss.str(string());
}

return 0;
}

int TProcess::run(){

  ds->session_read_start();
  ds->set_user_format(format);

  data = ds->get_next();
  if(data.empty()) exit(0); //if error or end of file occured then terminate program
  int selector = 0;

//form impulse
//{
  vector<double> imp; vector<double> t;
  impulse[selector][0].SetParameters(&data[0]);
  for(double x = impulse[selector][0].GetXmin(); x < impulse[selector][0].GetXmax(); x+=conf.dt){
    t.push_back(x);
    imp.push_back(impulse[selector][0].Eval(x));
   }
//}
    TCanvas *canvas = new TCanvas(); 
    TGraph * gr = new TGraph(imp.size(), &t[0],&imp[0]);
    //impulse[selector][0].Draw();
    gr->Draw("AC");
    canvas->Print("./images/impulse.gif");
    delete canvas;
    delete gr;

}
