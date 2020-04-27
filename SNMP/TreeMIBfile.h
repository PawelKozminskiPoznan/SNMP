#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <string>
#include <sstream>
#include "Data_types.h"

using namespace std;

struct node
{
	int OID_nr;
	node* parent;
	vector <node*> children;
	string name;
	string syntax;//type to zastêpuje
	typ* type;
	string access;
	string status;
	string description;
};

class TreeMIBfile
{
	node root_ISO;
	
public:
	void add_node(string,string,int OID, vector <string> parsed = vector<string>(), typ* = nullptr);
	node* search_node_name(string name);
	string get_node_OID(string name);
	node* search_node_OID(queue <int>,string);
	bool check_exsistance_node(string);
	void show_tree(node*,int = 1);
	node* getISO();
	TreeMIBfile();
	~TreeMIBfile();
};

