#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H 1

#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#include <boost/config.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_archive_exception.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_serialize.hpp> 
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
#include <zlib.h>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{
    using ::remove;
}
#endif

//using namespace std; 

class DataStorage
{	

	private:
	friend class boost::serialization::access;
	std::map<std::string,std::pair<double,double> > traits_buff;
	std::vector <std::vector <double> > datas_set;//буфер данных записи
	std::vector <std::vector <double> > datas_get;//буфер данных чтения по файлам
	std::vector <std::vector <double> > datas_get_i;//буфер данных чтения при поиске по индексу
	std::map <std::string, std::vector<std::vector<double> > > index_buff;// буфер данных индексов пара <имя индекса,данные>
	std::vector <std::string> format;// формат данных для записи
	std::vector <std::string> user_format;// формат данных для чтения
	std::vector<int> num_field_to_build_indexes;// вектор номеров полей данных соответствующих названию индекса 
	std::vector <int> num_user_fields; //вектор номеров полей данных соответствующих формату для чтения
	std::vector <std::string> index_files;// вектор названий файлов индексов
	std::vector <std::string> index_fields;// вектор имен индексов которые нужно построить
	std::string current_filename;// имя текущего файла в который производиться запись данных 
	int current_num_read_file;// имя текущего файла из которого производиться чтение
	std::string path;// путь к проекту 
	int f_traits;// флаг режима поиска по диапазону значения для некторого поля или полей
	int invalid_user_format;
	int invalid_traits_format;
	int create_prj;
	int load_prj;
	int session_read;
	int session_write;
	std::string useindex;
	int precision;
	int fl_index_create;// флаг сигнализации для построения индексов по завершению сесии записи данных
	//std::ofstream data_out_file;
	//std::ifstream data_from_file;
	std::vector < std::string > data_files_name; // вектор имен файлов данных
	std::string project_file_name;
	std::vector <double> find_use_index(int fl);
	//std::map <std::string,int> data_files_name;
	boost::uuids::uuid id;// уникальный идентификатор присваивающийся проекту
	int suffix_current_file;// суффих файла 
	int num_frame;// номер фрейма 
	int user_position_index;
	int current_num_data_file;
	int size_buff;
	int current_row;
	int row_size_file;
	int user_position;
	std::string field;
	int fl_set_fmt;
	int old_num_file;
	//int first;
	//int last;
	std::vector<int> num_traits_field;
	std::vector <double> find(int);
	int save_conf();
	int load_conf();
	int save_data();
	int load_data(std::vector<std::string>,std::vector <std::vector <double> >&);
	int compress();
	int decompress();
	int correspondence_fields(std::vector<std::string>& dst,std::vector<std::string>& src,std::vector<int>& num);
	std::string inttostr(int var);
	int SolveSize(std::ifstream&);
	int load_data_from_file(std::string,std::vector <std::vector <double> >&);
	int load_data_from_arch(std::string,std::vector <std::vector <double> >&);
	int load_index(std::string indexname);
	int save_data_in_file(std::string);
	int txt_to_arch();
	//int load_index(std::string indexname);
	std::string get_filename(std::string);
	std::string get_path(std::string);
	int increment(int inc[], int size);
	int build_index(std::string field, int asc);
	template<class T>
	void shellSort(std::vector<std::vector<T> >& a); 
	template<class Archive>
        void save(Archive & ar,const unsigned int version) const;
	template<class Archive>
	void load(Archive & ar,const unsigned int version);
	
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	public:
	DataStorage(int size_buff_= 10, int row_size_file_=20, int precision_=4, std::string path_ = boost::filesystem::current_path().c_str());
	DataStorage(std::string);
	~DataStorage();
	int set_data(std::vector<double>);
	int set_format(std::vector<std::string>);
	int set_user_format(std::vector<std::string>);
	//int set_traits(std::string,double,double);
	int set_traits(std::map<std::string,std::pair<double,double> > traits_buff_);
	std::vector<double> get_next();
	std::vector<double> get_prev();
	std::vector<std::string> get_format();
	std::vector<std::string> get_user_format();
	void session_write_start();
	void session_write_stop();
	void session_read_start();
	void session_read_stop();
	int set_indexes(std::vector<std::string>);
	int set_use_index(std::string);
	std::vector<std::string> get_indexes();
	std::string get_use_index();
	void seek_f_beg();
	void seek_f_end();
	void seek_i_beg();
	void seek_i_end();

};
BOOST_CLASS_VERSION(DataStorage, 2)
#endif
