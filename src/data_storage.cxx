#include "data_storage.h"

//using namespace std;
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{
    using ::remove;
}
#endif

DataStorage::DataStorage( int size_buff_, int row_size_file_ , int precision_,std::string path_ )
{	

	f_traits = 0;
	user_position = 0;
	suffix_current_file = 1; 
	id = boost::uuids::random_generator()();
	size_buff = size_buff_;
	row_size_file = row_size_file_;
	current_row = 0;
	precision = precision_;
	fl_set_fmt = 0;
	session_read = 0;
	session_write = 0;
	fl_index_create=0;
	create_prj =1;
	load_prj =0;
	path = path_;
	//std::cout << path << std::endl;
	if (boost::filesystem::exists(path) == true)
	{
		if (boost::filesystem::exists(path+"/save data") == false)
		{
			if (boost::filesystem::create_directory(path+"/save data") == true)
			{
				path +="/save data/";
			}else{} 
		}else{
			path +="/save data/";
		}
	}else{
		path.clear();
		path = boost::filesystem::current_path().c_str();
		//printf("%s\n",path.c_str());
		if (boost::filesystem::exists(path+"/save data") == false)
		{
			if (boost::filesystem::create_directory(path+"/save data") == true)
			{
				path +="/save data/";
			}else{} 
		}else{
			path +="/save data/";
		}
		
	} 
	std::string str_id = to_string(id);
	if(boost::filesystem::create_directory(path+str_id) == true)
	{
		path += str_id+"/";	
	}
    	project_file_name = path+str_id+".xml";
	
	//std::string str = to_string(id);
	//std::cout << "uuid=" << str << std::endl;
	save_conf();	
}
DataStorage::DataStorage(std::string prjname)
{	
	session_read = 0;
	session_write = 0;
	useindex = "NULL";
	create_prj =0;
	load_prj =1;
	user_position=0;
	user_position_index =0;
	current_num_read_file=0;
	f_traits = 0;
	path = get_path(prjname)+"/";
	std::cout << path << std::endl;
	try {
		if(prjname.find(".xml")!=-1)
    		{	
			project_file_name = prjname;
			load_conf();
		}else{std::cout << "file which load is not .xml" << std::endl;load_prj = 0;}
    	}
    	catch ( const std::exception & e )
      	{  	std::cout <<"\nFatal error: " <<e.what() <<"\n\n";
		std::cout <<"Hit enter to continue...";
		std::string temp;
		std::getline( std::cin, temp );
	}
}
DataStorage::~DataStorage()
{}

template<class Archive>
void DataStorage::save(Archive & ar,const unsigned int version) const
{
	ar & BOOST_SERIALIZATION_NVP(format); 
	ar & BOOST_SERIALIZATION_NVP(id);
	ar & BOOST_SERIALIZATION_NVP(data_files_name); 
	ar & BOOST_SERIALIZATION_NVP(size_buff);
	ar & BOOST_SERIALIZATION_NVP(suffix_current_file);
	ar & BOOST_SERIALIZATION_NVP(current_row);
	ar & BOOST_SERIALIZATION_NVP(row_size_file);
	ar & BOOST_SERIALIZATION_NVP(precision);
	ar & BOOST_SERIALIZATION_NVP(index_files);
}
template<class Archive>
void DataStorage::load(Archive & ar,const unsigned int version)
{
	ar & BOOST_SERIALIZATION_NVP(format); 
	ar & BOOST_SERIALIZATION_NVP(id); 
	ar & BOOST_SERIALIZATION_NVP(data_files_name);
	ar & BOOST_SERIALIZATION_NVP(size_buff);
	ar & BOOST_SERIALIZATION_NVP(suffix_current_file);
	ar & BOOST_SERIALIZATION_NVP(current_row);
	ar & BOOST_SERIALIZATION_NVP(row_size_file); 
	ar & BOOST_SERIALIZATION_NVP(precision);
 	ar & BOOST_SERIALIZATION_NVP(index_files);
}
//*********************************************************************
// input data vector for storage
//*********************************************************************
int DataStorage::set_data(std::vector<double> frame)
{
	if((session_write == 1)&&(session_read ==0))//uses in "sesson write"
	{
		int fl=0;
		std::vector <double>::iterator it;
		datas_set.resize(datas_set.size()+1);
		datas_set[datas_set.size()-1].resize(frame.size());
		for (int j=0;j<frame.size();j++)
		{	
			datas_set[datas_set.size()-1][j] = frame[j];	
		}
		if (datas_set.size() >= size_buff)
		{	
			save_data();
		}
		return 1;
	}
	return 0;
}
//*********************************************************************
//setup format input data 
//*********************************************************************  
int DataStorage::set_format(std::vector<std::string> form_str)
{
	if((create_prj==1)&&(fl_set_fmt==0))//setup format if mode"create project" and once
	{	
		fl_set_fmt =1;
		format.resize(form_str.size());
		std::vector <std::string>::iterator it;
		std::vector <std::string>::iterator it1 = format.begin();
		for (it=form_str.begin();it<form_str.end();it++)
		{
			(*it1) = (*it);
			it1++;	
		}
		return 1;
	}
	return 0;
}
//*******************************************************************
//setup format output data (uses in "session read")
//*******************************************************************
int DataStorage::set_user_format(std::vector<std::string> form_user_str)
{
	if(session_read==1) 
	{
		user_format.clear();
		user_format.resize(form_user_str.size());
		std::vector <std::string>::iterator it;
		std::vector <std::string>::iterator it1 = user_format.begin();
		for (it=form_user_str.begin();it<form_user_str.end();it++)
		{
			(*it1) = (*it);
			it1++;	
		}
		int ret = correspondence_fields(user_format,format,num_user_fields); 
		if((ret == user_format.size())&&(user_format.size()!=0))
		{
			invalid_user_format = 0;
		}else{
			invalid_user_format = 1;
		}
		return ret;//return number of found concidence fields in "format" and "user_format"
	}
	return 0;
}
std::vector<std::string> DataStorage::get_user_format()
{
	return user_format;
}

int DataStorage::set_traits(std::map<std::string,std::pair<double,double> > traits_buff_)
{
	if(session_read==1)
	{
		traits_buff.clear();	
		traits_buff = traits_buff_;
		f_traits = 0;
		int flag = 0;
		num_traits_field.clear();
		std::map<std::string,std::pair<double,double> >::iterator it;
		std::vector<std::string> dst;
		for(it =traits_buff.begin();it!=traits_buff.end();it++)
			dst.push_back((*it).first);
		if (traits_buff.size() != 0)	
		{	
			flag = correspondence_fields(dst,format, num_traits_field);
			if (flag >= 1)
			{	
				f_traits = 1;
			}
		}
		if ((flag == dst.size())&&(flag!=0))
		{
			invalid_traits_format = 0;
		}else{
			invalid_traits_format = 1;
		}
		return flag;
	}
	return 0;
}

int  DataStorage::correspondence_fields(std::vector<std::string>& dst,std::vector<std::string>& src,std::vector<int>& num)
{
	int flag = 0;
	int cnt = 0;
	num.clear();
	num.resize(dst.size());
	for(int i=0;i<dst.size();i++)
	{
		flag = 0;
		for(int j=0;j<src.size();j++)
		{
			if(dst[i] == src[j])
			{
				num[i]=j;
				flag = 1;
				cnt++;
				break;
			}
		}
		if(flag==0)
		{
			num[i]=-1;
		}
	}
	return cnt;
}
std::vector<double> DataStorage::DataStorage::get_next()
{
	if(session_read==1)
	{
		if( useindex == "NULL")
		{
			if (f_traits==0)
			{	
				if(invalid_user_format==0)
					return find(1);
			}else if ((invalid_traits_format==0)&&(invalid_user_format==0))
				return find(1);
		}else{
			if (f_traits==0)
			{
				if(invalid_user_format==0)
					return find_use_index(1);
			}else if ((invalid_traits_format==0)&&(invalid_user_format==0))
				return find_use_index(1);
		}
	}
	std::vector<double> frame;
	return frame;
}

std::vector<double> DataStorage::DataStorage::get_prev()
{
	if(session_read==1)
	{
		if( useindex == "NULL")
		{
			if (f_traits==0)
			{
				if(invalid_user_format==0)
					return find(2);
			}else if ((invalid_traits_format==0)&&(invalid_user_format==0))
				return find(2);
		}else{
			if (f_traits==0)
			{
				if(invalid_user_format==0)
					return find_use_index(2);
			}else if ((invalid_traits_format==0)&&(invalid_user_format==0))
				return find_use_index(2);
		}
	}
	std::vector<double> frame;
	return frame;
}
std::vector <double> DataStorage::find_use_index(int fl)
{	
	double var=0.0;
	std::vector <double> frame;
	std::map <std::string, std::vector<std::vector<double> > >::iterator it;
	it = index_buff.find(useindex);
	int cnt_good_field = 0;
	int num_good_field = 0;
	for(int i=0;i<num_traits_field.size();i++)
	{
		if(num_traits_field[i] !=-1)  
		{
			num_good_field++;
		}
	}
	if (it!= index_buff.end())
	do{     
		cnt_good_field = 0;
		frame.clear();
		frame.resize(user_format.size());
		if(((user_position_index>=(*it).second.size())&& (fl==1)) ||((user_position_index<=0)&&(fl==2)))
		{
			/*for(int i=0;i<frame.size();i++)
			{
				frame[i]=-1;
			}*/
			frame.clear();
			return frame;	
		}


		if (fl ==2 )
		{
			user_position_index--;
		}
		int num_file = (int)(*it).second[user_position_index][2];
		std::string filename = data_files_name[num_file];
		std::vector<std::string> stg;
		stg.push_back(path + filename);
		if ((old_num_file!=num_file)||(datas_get_i.size()==0))
			load_data(stg,datas_get_i);
		old_num_file = num_file;
		int num_str = (int)(*it).second[user_position_index][1];
		for(int i=0;i<user_format.size();i++)
		{
			//if(num_user_fields[i] != -1)			
			//{			
				frame[i] = datas_get_i[num_str][num_user_fields[i]];
			
			//}else{
			//	frame[i]=-1;
			//}
		}
		
		if(f_traits == 1)
		{	
			std::map <std::string, std::pair<double,double> >::iterator it1;
			it1 = traits_buff.begin();
			for(int i=0;i<num_traits_field.size();i++)
			{	
				//if(num_traits_field[i] !=-1)  
				//{
					var = datas_get_i[num_str][num_traits_field[i]];
					
					if ((var >=(*it1).second.first)&&(var<=(*it1).second.second))
					{
						cnt_good_field++;					
					}
				//}
				it1++;				
			}
		}
		if (fl ==1 )
		{
			user_position_index++;
		}
		

	}while((f_traits == 1)&&(cnt_good_field!=num_good_field));

	return frame;
}
std::vector <double> DataStorage::find(int fl)
{
	std::vector <double> frame;
	
	double var=0.0;
	int cnt_good_field = 0;
	int num_good_field = 0;
	for(int i=0;i<num_traits_field.size();i++)
	{
		if(num_traits_field[i] !=-1)  
		{
			num_good_field++;
		}
	}
	do
	{	
		cnt_good_field = 0;
		frame.clear();
		frame.resize(user_format.size());
		if ((user_position <=0)&&(fl==2))
		{	
			if(current_num_read_file > 1)
			{
				current_num_read_file--;
				std::vector<std::string> stg;
				stg.push_back(path + data_files_name[current_num_read_file-1]);
				load_data(stg,datas_get);
				user_position = datas_get.size();	
			}else{		
				/*for(int i=0;i<user_format.size();i++)
				{
					frame[i]=-1;
				}*/
				frame.clear();
				return frame;
			} 
			
		}

		if ((user_position >= datas_get.size())&&(fl==1))
		{	
			if(current_num_read_file < data_files_name.size())
			{
				current_num_read_file++;
				std::vector<std::string> stg;
				stg.push_back(path + data_files_name[current_num_read_file-1]);
				load_data(stg,datas_get);
				user_position = 0;	
			}else{		
				/*for(int i=0;i<user_format.size();i++)
				{
					frame[i]=-1;
				}*/
				frame.clear();
				return frame;
			} 		
		}
		if (fl == 2)
			user_position--;
	
		for(int i=0;i<user_format.size();i++)
		{
			//if(num_user_fields[i] != -1)			
			//{			
				frame[i] = datas_get[user_position][num_user_fields[i]];
				//printf("UUUU=%f\n",frame[i]);
			//}else{
			//	frame[i]=-1;
			//}
		}
		if(f_traits == 1)
		{	
			std::map <std::string, std::pair<double,double> >::iterator it1;
			it1 = traits_buff.begin();
			for(int i=0;i<num_traits_field.size();i++)
			{			
				//if(num_traits_field[i] !=-1)  
				//{
					var = datas_get[user_position][num_traits_field[i]];
						
					if ((var >=(*it1).second.first)&&(var<=(*it1).second.second))
					{
						cnt_good_field++;//printf("%d//%d\n",cnt_good_field,num_good_field);					
					}
				//}
					it1++;				
			}
		}
		if (fl == 1)
			user_position++;
	
	}while((f_traits == 1) && (cnt_good_field != num_good_field));
	
	return frame;
}

std::vector<std::string> DataStorage::DataStorage::get_format()
{
	return format;
}
int DataStorage::save_conf()
{
	std::ofstream ofs(project_file_name.c_str());
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);	
	oa << boost::serialization::make_nvp("class_obj",*this);
	return 0;
}
int DataStorage::load_conf()
{
try{
	std::ifstream ifs(project_file_name.c_str());
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);
	ia >> boost::serialization::make_nvp("class_obj",*this);
}
catch ( const std::exception & e )
    {  	
		std::cout <<"\nFatal error: " <<e.what() <<"\n\n";
		std::cout <<"Hit enter to continue...";
		std::string temp;
		std::getline( std::cin, temp );
		return 0;
	}
	return 0;
}
int DataStorage::txt_to_arch()
{	std::string str = "%."+inttostr(precision)+ "g";
	const char * out_format = str.c_str();
	std::string filename_gz = current_filename + ".gz";
	gzFile file_gz = gzopen (filename_gz.c_str(),"wb9"); 
	std::vector<std::string> stg;
	stg.push_back(current_filename);
	load_data (stg,datas_get);
	for(int i=0;i<datas_get.size();i++)
  	{	for(int j=0;j<datas_get[i].size();j++)
		{	
			gzprintf(file_gz,out_format ,datas_get[i][j]);
                	if (j<datas_get[i].size()-1)
			{
				gzprintf(file_gz,"\t");
			}
		}
		gzprintf(file_gz,"\n");
	}
	datas_get.clear();
    gzclose(file_gz);	
    data_files_name[data_files_name.size()-1] = get_filename(filename_gz);
 	boost::filesystem::remove(current_filename);
	return 1;
}
int DataStorage::save_data()
{	
	std::string str = "%."+inttostr(precision)+ "g";
	const char * out_format = str.c_str();
	FILE * data_out_file;
	data_out_file = NULL;
	for(int i=0;i<datas_set.size();i++)
	{
		if (boost::filesystem::exists(current_filename.c_str()) == false)
		{	
			data_files_name.resize(data_files_name.size()+1);
			data_files_name[data_files_name.size()-1] = get_filename(current_filename);
			current_row=0;
		}
		if(data_out_file == NULL)
			data_out_file = fopen(current_filename.c_str(),"a");
		if(current_row < row_size_file)
		{
			for(int j=0;j<datas_set[i].size();j++)
			{
				fprintf(data_out_file, out_format ,datas_set[i][j]);	
				if (j<datas_set[i].size()-1)
				{
					fprintf(data_out_file,"\t");
				}	
			}		
			fprintf(data_out_file,"\n");
			current_row = current_row + 1; 
		}	
		if(current_row >= row_size_file)
		{     
			fclose(data_out_file);
			data_out_file = NULL;
			txt_to_arch();
			current_filename.clear();
			suffix_current_file++;
			std::string str_id = to_string(id);
			current_filename = path + str_id+'_' +inttostr(suffix_current_file)+".txt";
		}
	}	
	if(data_out_file != NULL)
		fclose(data_out_file);	
	datas_set.clear();
	datas_set.resize(0);
	save_conf();		
	return 0;
}
int DataStorage::load_data(std::vector<std::string> filename,std::vector <std::vector <double> >& buff)
{
	buff.clear();
	buff.resize(0);
	for(int ii=0; ii<filename.size(); ii++)
	{
		if(filename[ii].find(".gz")!=-1) 
		{				
			load_data_from_arch(filename[ii],buff);
		}else{
			load_data_from_file(filename[ii],buff);
		}
	}
	return 1;
}
std::string DataStorage::inttostr(int var)
{
	std::ostringstream out;
	out << var;
	return out.str();
}

int DataStorage::load_data_from_file(std::string filename, std::vector <std::vector <double> >& buff)
{
	std::ifstream in_mas;
	in_mas.open(filename.c_str(),std::ios_base::in);
	char b[1];
    char T[20];
    for (int l=0;l<20;l++) T[l]=' ';
    int i = 0;
    int k = 0;
    int j = 0;
	int fl = 1;
    in_mas.clear();
    in_mas.seekg(0,std::ios_base::beg);
    while (!in_mas.get(b[0]).eof())
    {
        if ((b[0]=='0')||(b[0]=='1')||(b[0]=='2')||(b[0]=='3')
		||(b[0]=='4')||(b[0]=='5')||(b[0]=='6')||(b[0]=='7')||(b[0]=='8')
		||(b[0]=='9')||(b[0]=='+')||(b[0]=='-')||(b[0]=='.')||((b[0]=='*')&&(k!=0))
		||(((b[0]=='e')||(b[0]=='E'))&&(k!=0)))
        {
            T[k] = b[0];
            k++;
        } else
        {
            if (k>0)
            {	
				if (fl==1)
				{
					buff.resize(buff.size()+1);
					fl=0;
				}
				buff[i].resize(buff[i].size()+1);
				buff[i][j] = atof(T);
				for(int l=0;l<20;l++)
               		T[l]=' ';
				j++;
				k=0;
            }
			if(b[0]=='\n')
			{i++;fl=1;j=0;}
        }
    }
    in_mas.clear();
	in_mas.close();
	return 0;
}
int DataStorage::load_data_from_arch(std::string archname, std::vector <std::vector <double> >& buff)
{	

	gzFile f = gzopen(archname.c_str(),"rb9");
	int fl = 1;
	char b;
	char T[20];
	for(int i=0;i<20;i++)
		T[i]=' ';
	int i=0;
	int j=0;
	int k=0;
	while((b = (char)gzgetc(f))!= EOF)
	{
		if ((b=='0')||(b=='1')||(b=='2')||(b=='3')
		||(b=='4')||(b=='5')||(b=='6')||(b=='7')||(b=='8')
		||(b=='9')||(b=='+')||(b=='-')||(b=='.')||((b=='*')&&(k!=0))
		||(((b=='e')||(b=='E'))&&(k!=0)))
		{	
			T[k]=b;
			k++;
		}else
		{
			if(k>0)
			{	
				if (fl==1)
				{
					buff.resize(buff.size()+1);
					fl=0;
				}
				buff[i].resize(buff[i].size()+1);
				buff[i][j] = atof(T);
				for(int l=0;l<20;l++)
					T[l]=' ';
				j++;
				k=0;
			}
			if(b=='\n')
			{i++;fl=1;j=0;}
		}
	}
	gzclose(f);
	return 0;
}
std::string DataStorage::get_filename(std::string str)
{
	size_t found;
	found=str.find_last_of("/\\");
	return str.substr(found+1); 
}
std::string DataStorage::get_path(std::string str)
{
	size_t found;
	found=str.find_last_of("/\\");
	return str.substr(0,found);
}
int DataStorage::SolveSize(std::ifstream& in_mas)
{      
	int size = 0;
    int p = 0;
    char b[1];
    b[0]=' ';
    while (!in_mas.get(b[0]).eof())
    {
        if((b[0]=='\n')&& (p > 0))
        {
            size++;
            p = 0;
        }else
        {
            p++;
		}
    }
    if (in_mas.get(b[0]).eof())
    {
        size++;
        in_mas.clear();
        in_mas.seekg(-1,std::ios_base::end);
        in_mas.get(b[0]);
        if (b[0] =='\n' )
        {
            size--;
        }
	}
}
int DataStorage::increment(int inc[], int size) {
// inc[] массив, в которые заносятся инкременты
// size размерность этого массива
	int p1, p2, p3, s;
	p1 = p2 = p3 = 1;
	s = -1;
	do 
	{// заполняем массив элементов по формуле Роберта Седжвика
		if (++s % 2) 
		{
			inc[s] = 8*p1 - 6*p2 + 1;
		} else {
			inc[s] = 9*p1 - 9*p3 + 1;
			p2 *= 2;
			p3 *= 2;
		}
        p1 *= 2;
// заполняем массив, пока текущая инкремента хотя бы в 3 раза меньше количества элементов в массиве
	} while(3*inc[s] < size);  
	return s > 0 ? --s : 0;// возвращаем количество элементов в массиве
}
 
template<class T>
void DataStorage::shellSort(std::vector<std::vector<T> >& a) 
{
// inc инкремент, расстояние между элементами сравнения
// i и j стандартные переменные цикла
// seq[40] массив, в котором хранятся инкременты
	int size = a.size();
	int inc, i, j, seq[40];
	int s;//количество элементов в массиве seq[40]
  // вычисление последовательности приращений
	s = increment(seq, size);
	while (s >= 0) 
	{
        //извлекаем из массива очередную инкременту
        inc = seq[s--];
// сортировка вставками с инкрементами inc
		for (i = inc; i < size; i++) 
		{
			T temp = a[i][0];
			T temp1 = a[i][1];
			T temp2 = a[i][2];
// сдвигаем элементы до тех пор, пока не дойдем до конца или не упорядочим в нужном порядке
			for (j = i-inc; (j >= 0) && (a[j][0] > temp); j -= inc)
			{
				a[j+inc][0] = a[j][0];
				a[j+inc][1] = a[j][1]; 
				a[j+inc][2] = a[j][2];
			}
// после всех сдвигов ставим на место j+inc элемент, который находился на i месте
			a[j+inc][0] = temp;
			a[j+inc][1] = temp1;
			a[j+inc][2] = temp2;
		}
	}
}
int DataStorage::build_index(std::string field, int asc)
{
	std::vector<std::string> dst;
	dst.push_back(field);
	std::vector<int> num;
	if (correspondence_fields(dst,format, num)==1)
	{
		if(data_files_name.size()>0)
		{	
			std::map <std::string, std::vector<std::vector<double> > >::iterator it;	
			std::vector<std::vector<double> > initbuff;
			std::vector<std::vector<double> > databuff;
			it = index_buff.find(field);
			if(it != index_buff.end())
			{
				index_buff.erase(it);	
			}
			std::vector<std::string> stg;
			for(int i=0;i<data_files_name.size();i++)
			{
				stg.clear();			
				stg.push_back(path + data_files_name[i]);
				load_data(stg,databuff);
				int oldsize = initbuff.size();
				initbuff.resize(oldsize+databuff.size());
				for(int ii =0;ii<databuff.size();ii++)
				{
					initbuff[oldsize + ii].resize(3);
					initbuff[oldsize + ii][0] = databuff[ii][num[0]];
					initbuff[oldsize + ii][1] = (double)ii;
					initbuff[oldsize + ii][2] = (double)i;
				}	
			}
			shellSort(initbuff);
			it = index_buff.end();
			index_buff.insert (it, std::pair<std::string, std::vector<std::vector<double> > > (field , initbuff ));
			FILE *f;
			std::string namebin = path + field+".bin";
			f = fopen(namebin.c_str(),"wb");
			int sizeB = initbuff.size()*3;
			double* buffer = new double[sizeB];
			int cnt=0;
			for (int i=0;i<initbuff.size();i++)
				for (int j=0;j<initbuff[i].size();j++)
				{	
					buffer[cnt] = initbuff[i][j];
					cnt++;
				}
			fwrite (buffer , sizeof(double) ,sizeB  , f );
			fclose (f);
			delete[] buffer;
			int flag =0;
			for(int i =0;i<index_files.size();i++)
				if (index_files[i]== field+".bin")
					flag++;
			if (flag == 0)		
				index_files.push_back(field+".bin");
			save_conf();
			return 1;
		}	
	}
	return 0;
}
int DataStorage::load_index(std::string indexname)
{
	FILE *f;
	f = fopen(indexname.c_str(),"rb");
	fseek (f , 0 , SEEK_END);
	int lSize = ftell (f);
	rewind (f);
	int lenBuf = lSize/sizeof(double);
	double* buffer = new double[lenBuf];
	for (int i=0;i<lenBuf;i++)
		buffer[i]=0;
	fread (buffer, sizeof(double) , lenBuf , f );
	fclose (f);
	std::string field;	
	field = get_filename(indexname);
	size_t found;
	found = field.find_last_of(".");
	field = field.substr(0,found);
	std::map <std::string, std::vector<std::vector<double> > >::iterator it;
	it = index_buff.find(field);
	if(it != index_buff.end())
	{
		index_buff.erase(it);	
	}
	std::vector<std::vector<double> > initbuff;
	initbuff.resize(lenBuf/3);
	int cnt=0;
	for (int i=0;i<initbuff.size();i++)
	{	
		initbuff[i].resize(3);
		for (int j=0;j<initbuff[i].size();j++)
		{	
			initbuff[i][j]=buffer[cnt];
			cnt++;
		}
	}
	it = index_buff.end();
	index_buff.insert (it, std::pair<std::string, std::vector<std::vector<double> > > (field , initbuff ));
	delete[] buffer;
	return 0;
}
int DataStorage::set_indexes(std::vector<std::string> fields_)
{
	if(session_write == 1)
	{
		index_fields.clear();
		for(int i=0;i<fields_.size();i++)
		{
			index_fields.push_back(fields_[i]);
		}
		int ret = correspondence_fields(fields_,format, num_field_to_build_indexes);
		if (ret > 0)
		{
			fl_index_create = 1;
		}else{
			fl_index_create = 0;
		}
		return ret;
	}
	return 0;	
}
void DataStorage::session_read_start()
{
	if ((load_prj == 1)&&(session_write==0)&&(session_read==0))
	{	
		invalid_user_format = 1;
		invalid_traits_format = 1;
		old_num_file = 0;
		session_read = 1;
		session_write = 0;
		user_position = 0;
		user_position_index = 0;
		datas_get.clear();
		datas_get_i.clear();
		f_traits = 0;
		useindex = "NULL";
		current_filename.clear();
		if(data_files_name.size()>0)
		{
			std::vector<std::string> stg;
			stg.push_back(path + data_files_name[0]);
			load_data(stg,datas_get);
			current_num_read_file = 1;
		}else{std::cout << "not data file for load in project"<<std::endl; }
		if(index_files.size()>0)
		{       
			index_buff.clear();
			for(int i= 0;i<index_files.size();i++)
			{
				load_index(path+index_files[i]);
			}	
		}
	}
}
void DataStorage::session_read_stop()
{
	if ((load_prj == 1)&&(session_write == 0)&&(session_read==1))
	{	
		session_read = 0;
		datas_get.clear();
		datas_get_i.clear();
	}
}
void DataStorage::session_write_start()
{
	if ((session_read == 0)&&(session_write == 0))
	{
		session_write = 1;
		datas_set.clear();
		current_filename.clear();
		if(index_files.size()>0)
		{
			for(int i =0;i<index_files.size();i++)
			{
				boost::filesystem::remove(path + index_files[i]);			
			}
			index_files.clear();
			index_fields.clear();
			index_buff.clear();
			save_conf();
		}
		if(data_files_name.size()>0)
		{
			if (data_files_name[data_files_name.size()-1].find(".gz")==-1)
			{	
				current_filename = path + data_files_name[data_files_name.size()-1];
			}else{
				std::string str_id = to_string(id);
				current_filename = path + str_id +'_'+inttostr(suffix_current_file)+".txt";
			}
		}else{ 	suffix_current_file=1;
			std::string str_id = to_string(id);
			current_filename = path + str_id +'_'+inttostr(suffix_current_file)+".txt";
		}
	}
}
void DataStorage::session_write_stop()
{
	if((session_read ==0)&&(session_write ==1))
	{
		session_write = 0;
		if (datas_set.size()!=0)
		{
			save_data();
		}
		datas_set.clear();
		if(fl_index_create == 1)
		{
			for(int i =0;i<index_fields.size();i++)
			{
				if(num_field_to_build_indexes[i]!=-1)
					build_index(index_fields[i],0);
			}
		}
	}
}
int DataStorage::set_use_index(std::string index)
{
	if ((index != "NULL")&&(session_read == 1))
	{
		useindex.clear();
		if (index_buff.find(index)== index_buff.end())
		{	
			if (build_index(index,0)!=0)
			{	
				useindex = index;
				return 1;
			}
			useindex = "NULL";
			return 0;
		} 
		useindex = index;
		return 1;
	}else {
		useindex = "NULL";
	}
	return 0;
}
std::vector<std::string> DataStorage::get_indexes()
{	
	std::vector<std::string> indexname;
	if (session_read==1)
	{
		if (index_buff.size()!=0)
		{	
			indexname.resize(index_buff.size());	
			std::map <std::string,std::vector<std::vector<double> > >::iterator it;
			std::vector <std::string>::iterator it1 = indexname.begin();
			for (it = index_buff.begin(); it != index_buff.end(); it++)
			{
				(*it1) = (*it).first;
				it1++;	
			}
		} 
	}
	return indexname;
}
std::string DataStorage::get_use_index()
{
	if (session_read==1)
	{
		return  useindex;
	}
	std::string stg;
	stg.clear();
	return stg;
}
void DataStorage::seek_f_beg()
{
	if(session_read == 1)
	{
		if(data_files_name.size()!=0)
		{
			user_position = 0;
			std::vector<std::string> stg;
			stg.push_back(path + data_files_name[0]);
			load_data(stg,datas_get);
			current_num_read_file = 1;
		}else{
			user_position = 0;
			current_num_read_file = 0;
		}
	}
}
void DataStorage::seek_f_end()
{
	if(session_read == 1)
	{
		if(data_files_name.size()!=0)
		{
			current_num_read_file = data_files_name.size();
			std::vector<std::string> stg;
			stg.push_back(path + data_files_name[current_num_read_file-1]);
			load_data(stg,datas_get);
			user_position = datas_get.size();
			printf("last file size = %d\n",datas_get.size());
			
		}else{
			current_num_read_file =0;
			user_position = 0;
		}		
	}
}
void DataStorage::seek_i_beg()
{
	if(session_read == 1)
	{
		user_position_index = 0;		
	}
}
void DataStorage::seek_i_end()
{
	if(session_read == 1)
	{
		if (useindex!="NULL")
		{	
			std::map <std::string,std::vector<std::vector<double> > >::iterator it;
			it = index_buff.find(useindex);
			if (it!=index_buff.end())
			{
				user_position_index = (*it).second.size();
			}
		}		
	}
}
