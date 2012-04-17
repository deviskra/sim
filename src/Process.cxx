#include "Process.h"

#define UNDEFINED -1

Process::Process(Config & _conf) : conf(_conf), selector(UNDEFINED), time(UNDEFINED) {
     parse();     
}

Process::~Process(){
     delete ds;
}

//TProcess::TProcess(){
//}

int Process::ignore_endl(string &str, char ch = '\n'){
    stringstream ss(str);
    str.clear();
    string s;
    while(getline(ss, s, ch)){
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
dt = conf.dt;
seed = conf.seed;

//clearing shared varibles for a case of repetitive calls if any
read.clear();
impulse.clear();

// buffer varibles
    stringstream ss;
    string s;

//open data storage
    ds = new DataStorage(discomment(conf.data_source));


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

//  which varible is time?
    ss << conf.time; discomment(ss);
    ss >> s;
    for(int i = 0; i < read.size(); i++){
       if(s == read[i]){
            time = i;
            break;
       }
    }
   ss.str(string());ss.clear(); s.clear();


// read output format vector
    ss << conf.write; discomment(ss);
    while(ss >> s){
      for(int i = 0; i < read.size(); i++){
       if(s == read[i]){
            write.push_back(i);
            break;
         }
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
{ double xmin = 0, xmax;
   for(map<int, vector<string> >::iterator it = conf.impulse.begin(); it != conf.impulse.end();++it){
       ss << discomment(it->second[1]);
       ss >> xmax;
       
       impulse[it->first].push_back(TF1("", discomment(it->second[0]).c_str(), xmin, TMath::Abs(xmax)));
       //impulse[it->first].push_back(TF1("", discomment(it->second[2]).c_str() )); //dispersion if any

        ss.str(string());
    }
}

//parse amplitude-space functions
for(map<string, vector<string> >::iterator it = conf.asc.begin(); it != conf.asc.end();++it){
    
       asc[it->first].push_back(TF1("", discomment(it->second[0]).c_str() )); //
       asc[it->first].push_back(TF1("", discomment(it->second[1]).c_str() )); //dispersion
       
       output[it->first] = vector<double>(); //this one will contain data of each output

       ss.str(string());
       }
return 0;
}

int Process::run(){

ds->session_read_start();

TRandom rnd; //ROOT random generator

//if unrecognized varible has met then terminate program
if(read.size() != ds->set_user_format(read)) {
	cout << "Error: Data source doesn't contain requested varibles." << endl;
	exit(0);
}

if (time == UNDEFINED) {

   map<string, ofstream * > foutput;
   for(map<string, vector<TF1> >::iterator it = asc.begin(); it != asc.end();++it){
      foutput[it->first] = new ofstream(it->first.c_str());
   }

  for(int line = 1;; line++){

  data = ds->get_next(); // load data
  if(data.empty()) break;

  cout << "Processing line " << line << " .... " << endl;

     for(map<string, vector<TF1> >::iterator it = asc.begin(); it != asc.end();++it){ // for each output
        rnd.SetSeed(seed); //set the same seed for every output 
        asc[it->first][0].SetParameters(&data[0]); asc[it->first][1].SetParameters(&data[0]);
          for(int i = 0; i < write.size(); i++){ *foutput[it->first] << data[ write[i] ] << '\t';} // additional output
           *foutput[it->first] << rnd.Gaus(asc[it->first][0].Eval(0), asc[it->first][1].Eval(0)) << endl;
      }
   }    
   
  //The whole thing works well without next lines, so I don't sure they are really essential.
  for(map<string, vector<TF1> >::iterator it = asc.begin(); it != asc.end();++it){
      foutput[it->first] -> close();
      delete foutput[it->first];
  }
  
} else {

for(int line = 1;; line++){

  data = ds->get_next(); // load data
  if(data.empty()) break;

  cout << "Processing line " << line << " .... " << endl;

  int select = (selector == UNDEFINED) ? 0 : data[selector];
  double t = data[time];
  int elem0 =  t / dt;  // base index
  
  impulse[select][0].SetParameters(&data[0]);
  for(map<string, vector<TF1> >::iterator it = asc.begin(); it != asc.end();++it){ // for each output
    rnd.SetSeed(seed); //set the same seed for every output 
    asc[it->first][0].SetParameters(&data[0]); asc[it->first][1].SetParameters(&data[0]);
    for(double x = impulse[select][0].GetXmin(); x <= impulse[select][0].GetXmax(); x+=dt){//form impulse
      int elem = elem0 + x / dt; //base index + shift
      if(output[it->first].size() < (elem + 1)) output[it->first].resize(elem + 1); // 
        output[it->first][elem] += 
           rnd.Gaus(asc[it->first][0].Eval(0) * impulse[select][0].Eval(x)/*average*/, asc[it->first][1].Eval(0) /*dispersion*/);
       }
   }
   
 }

// form output  
   bool isTime = false;
   for(int i = 0; i < write.size(); i++){
      if(write[i] == time) {isTime = true; break;}   
   }
   
   ofstream fout;
   for(map<string, vector<double> >::iterator it = output.begin(); it != output.end();++it){
     fout.open(it->first.c_str()); 
     for(int i = 0; i < it->second.size(); i++){
         if(isTime) fout << i*dt << '\t';
         fout << it->second[i] << endl;
         }
     fout.close();  
    }

  }

}




