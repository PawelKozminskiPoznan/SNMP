#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "TreeMIBfile.h"
#include "Data_types.h"


#pragma once

using namespace std;

class ParsingMIBfile
{
	vector <fstream*> files;
	
	//fstream file;
	bool open_ok;//??
	//regex regex_pattern; // ?
	string line_from_mib;
	string pattern;
	smatch result;
	
	
public:
	TreeMIBfile tree;
	Data_types data_type;
	void load_file(string);
	void import_find();
	node* get_OID(string);
	void object_identifier_find();
	void object_type_find();
	void data_type_find();
	ParsingMIBfile();
	~ParsingMIBfile();
};

