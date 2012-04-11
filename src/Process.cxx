#include "Process.h"

#define UNDEFINED -1

Process::Process(Config & _conf) : conf(_conf), selector(UNDEFINED), time(UNDEFINED) {
     parse();
     run();
     save();
}

Process::~Process(){
     delete ds;
}

//TProcess::TProcess(){
//}

int Process::ignore_endl(string &str){
    stringstream ss(str);
    str.clear();
    string s;
    while(getline(ss, s)){
        str+=s;
    };
  return 0;
}

int Process::discomment(stringstream & ss, char ch = '#'){
    string s;
    getline(ss, s, ch);
    ss.clear(); //unset error flag eofbit
    
    ignore_endl(s);
    ss.str(s);

 return 0; 
}

string Process::discomment(string str, char ch = '#'){
    stringstream ss(str);
    string s;
    getline(ss, s, ch);
    ignore_endl(s);
 return s; 
}


int Process::parse(){
ds = new DataStorage("./save data/" + conf.id + '/' + conf.id + ".xml");
dt = conf.dt;

//clearing shared varibles for a case of repetitive calls
read.clear();
impulse.clear();

// buffer varibles
    stringstream ss;
    string s;

//get available variables
    ss << conf.read; discomment(ss);
    while(ss >> s){
           read.push_back(s); // set up user data format
    }
    ss.str(string());ss.clear(); s.clear();

//  which varible is selector?
    ss << conf.selector; discomment(ss);
    ss >> s;
    for(int i = 0; i < read.size(); i++){
       if(s == read[i]){
            selector = i;
            break;
       }
    }
   ss.str(string());ss.clear(); s.clear();

//  which varible is global time?
    ss << conf.time; discomment(ss);
    ss >> s;
    for(int i = 0; i < read.size(); i++){
       if(s == read[i]){
            time = i;
            break;
       }
    }
   ss.str(string());ss.clear(); s.clear();

{  // set up corresponding alias for ROOT parameters [0], [1], ...
   int i = 0; // current parameter number
   for(int i = 0; i < read.size(); ++i){
       ss << '[' << i << ']';
       new TFormula(read[i].c_str(), ss.str().c_str());
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

int Process::run(){

  ds->session_read_start();
  ds->set_user_format(read);

for(int i = 0; i < 5;i++){
  
  data = ds->get_next(); // load data
  if(data.empty()) exit(0); //if error or end of file occured then terminate program

  int select = (selector == UNDEFINED) ? 0 : (int)data[selector];
  double t = (time == UNDEFINED) ? 0 : data[time];
  int elem0 =  t / dt;  // base index
   
    impulse[select][0].SetParameters(&data[0]);
      for(map<string, vector<TF1> >::iterator it = asc.begin(); it != asc.end();++it){ // for each output
         for(double x = impulse[select][0].GetXmin(); x <= impulse[select][0].GetXmax(); x+=dt){//form impulse
           int elem = elem0 + x / dt; //base index + shift
           if(output[it->first].size() < (elem + 1)) output[it->first].resize(elem + 1); 
           output[it->first][elem] += asc[it->first][0].Eval(0) * impulse[select][0].Eval(x);
         }
      }
   
   }

}

int Process::save(){
ofstream fout;
     for(map<string, vector<double> >::iterator it = output.begin(); it != output.end();++it){
          fout.open(it->first.c_str()); 
          for(int i = 0; i < it->second.size(); i++){
                   fout << i*dt << '\t' << it->second[i] << endl;
           }
         fout.close();  
      }
/*
 TCanvas *canvas = new TCanvas(); 
    TGraph * gr = new TGraph(imp.size(), &t[0],&imp[0]);
    gr->Draw("AC");
    stringstream ss; ss << "./images/impulse" << i << ".gif";
    canvas->Print(ss.str().c_str());
    delete canvas;
    delete gr;
}    
   imp.clear();t.clear();
*/
}




