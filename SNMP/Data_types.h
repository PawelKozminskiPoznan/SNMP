#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;

enum visibility
{
	universal,
	application,
	context_specific,
	privatee,
	undeclarated
};

enum evidence
{
	implicit,
	explicitt,
	undeclarated2
};

enum primitive
{
	integer,
	octet_string,
	object_identifier,
	null,
	undeclarated3
};

struct typ
{
	string name;
	int min_range;
	int max_range;
	int min_size;
	int max_size;
	int size;
	visibility type_visibility;
	int id_type;
	evidence type_evidence;
	primitive primitive_type;
	typ* my_type;
	bool choice;
	list <typ*> sequence;
	typ()// pare konstruktorów ?
	{
		name = "undeclarated";
		min_range = -1;
		max_range = -1;
		max_size = -1;
		min_size = -1;
		size = -1;
		type_visibility = visibility::undeclarated;
		id_type = -1;
		type_evidence = evidence::undeclarated2;
		primitive_type = primitive::undeclarated3;
		my_type = nullptr;
		choice = false;
		//sequence ??
	}
	bool operator != (const typ& a) const
	{
		if (a.name == this->name && a.type_visibility == this->type_visibility && a.min_range == this->min_range &&
			a.max_range == this->max_range && a.size == this->size && a.id_type == this->id_type && a.type_evidence == this->type_evidence
			&& a.primitive_type == this->primitive_type && a.my_type == this->my_type && a.choice == this->choice &&
			a.sequence == this->sequence && a.max_size == this->max_size && a.min_size == this->min_size)
			return false;
		else
			return true;
	}
};


class Data_types
{
	list <typ*> all_types;
public:
	typ* new_type_sequence(string);
	typ * new_type_sequence(string , string );
	typ* new_type_choice(string type_name);
	void new_type(typ* seq, string type_name, string type);
	void new_type(typ* seq, string type_name, string type, int);
	void new_type(typ* seq, string type_name, string type, int, int);
	void new_type(string, string);
	void new_type(string, string, string, int, string);
	void new_type(string, string, string, int, string, int);
	void new_type(string, string, string, int, string, int, int);
	void new_type(string, string, int, int, bool);
	void new_type(string , string , int);
	void primitive_type();
	bool search_typ(string);
	typ* get_typ(string name);
	void erase(typ* a);
	void show_types();
	Data_types();
	~Data_types();
};


