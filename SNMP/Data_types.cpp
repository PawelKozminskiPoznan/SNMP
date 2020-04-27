#include "pch.h"
#include "Data_types.h"

using namespace std;



void Data_types::new_type(string type_name, string primitive,string visibility,int id_vision,string im_ex)
{
		typ* new_type = new typ;
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->primitive_type = primitive::undeclarated3;

		if (visibility == "UNIVERSAL")
			new_type->type_visibility = visibility::universal;
		else if (visibility == "APPLICATION")
			new_type->type_visibility = visibility::application;
		else if (visibility == "CONTEXT SPECIFIC")
			new_type->type_visibility = visibility::context_specific;
		else if (visibility == "PRIVATE")
			new_type->type_visibility = visibility::privatee;
		else
			new_type->type_visibility = visibility::undeclarated;
		new_type->id_type = id_vision;

		if (im_ex == "IMPLICIT")
			new_type->type_evidence = evidence::implicit;
		else if (im_ex == "EXPLICIT")
			new_type->type_evidence = evidence::explicitt;
		else
			new_type->type_evidence = evidence::undeclarated2;
		if (search_typ(type_name))
		{
			typ* compare = get_typ(type_name);
			if (compare->type_visibility != new_type->type_visibility)
			{
				erase(compare);
				all_types.push_back(new_type);
			}
		}
		else
			all_types.push_back(new_type);

}

void Data_types::new_type(string type_name, string primitive, string visibility, int id_vision, string im_ex,int size)
{
		typ* new_type = new typ;
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->primitive_type = primitive::undeclarated3;

		if (visibility == "UNIVERSAL")
			new_type->type_visibility = visibility::universal;
		else if (visibility == "APPLICATION")
			new_type->type_visibility = visibility::application;
		else if (visibility == "CONTEXT SPECIFIC")
			new_type->type_visibility = visibility::context_specific;
		else if (visibility == "PRIVATE")
			new_type->type_visibility = visibility::privatee;
		else
			new_type->type_visibility = visibility::undeclarated;
		new_type->id_type = id_vision;

		if (im_ex == "IMPLICIT")
			new_type->type_evidence = evidence::implicit;
		else if (im_ex == "EXPLICIT")
			new_type->type_evidence = evidence::explicitt;
		else
			new_type->type_evidence = evidence::undeclarated2;
		new_type->size = size;
		if (search_typ(type_name))
		{
			typ* compare = get_typ(type_name);
			if (compare->type_visibility != new_type->type_visibility)
			{
				erase(compare);
				all_types.push_back(new_type);
			}
		}
		else
			all_types.push_back(new_type);
}

void Data_types::new_type(string type_name, string primitive, string visibility, int id_vision, string im_ex, int ogr_min,int ogr_max)
{
		typ* new_type = new typ;
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->primitive_type = primitive::undeclarated3;

		if (visibility == "UNIVERSAL")
			new_type->type_visibility = visibility::universal;
		else if (visibility == "APPLICATION")
			new_type->type_visibility = visibility::application;
		else if (visibility == "CONTEXT SPECIFIC")
			new_type->type_visibility = visibility::context_specific;
		else if (visibility == "PRIVATE")
			new_type->type_visibility = visibility::privatee;
		else
			new_type->type_visibility = visibility::undeclarated;
		new_type->id_type = id_vision;

		if (im_ex == "IMPLICIT")
			new_type->type_evidence = evidence::implicit;
		else if (im_ex == "EXPLICIT")
			new_type->type_evidence = evidence::explicitt;
		else
			new_type->type_evidence = evidence::undeclarated2;
		new_type->min_range = ogr_min;
		new_type->max_range = ogr_max;
		if (search_typ(type_name))
		{
			typ* compare = get_typ(type_name);
			if (compare->type_visibility != new_type->type_visibility)
			{
				erase(compare);
				all_types.push_back(new_type);
			}
		}
		else
			all_types.push_back(new_type);
}

void Data_types::new_type(string type_name,string primitive)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ;
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->my_type = get_typ(primitive);
		all_types.push_back(new_type);
	}
}

typ* Data_types::new_type_sequence(string type_name)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ;
		new_type->name = type_name;
		//new_type->sequence = list <typ>();    // inicjacja listy??
		all_types.push_back(new_type);
		return new_type;
	}
}

typ* Data_types::new_type_sequence(string type_name, string primitive)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ;
		new_type->name = type_name;
		//new_type->sequence = list <typ>();    // inicjacja listy??
		all_types.push_back(new_type);
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->my_type = get_typ(primitive);
		return new_type;
	}
	else
		return get_typ(type_name);
}

typ* Data_types::new_type_choice(string type_name)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ;
		new_type->choice = true;
		new_type->name = type_name;
		//new_type->sequence = list <typ>();    // inicjacja listy??
		all_types.push_back(new_type);
		return new_type;
	}
	else
	{
		typ* new_type = get_typ(type_name);
		new_type->choice = true;
		return new_type;
	}
}

void Data_types::new_type(typ* seq, string type_name, string primitive)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ();
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->my_type = get_typ(primitive);
		if (!new_type->my_type && new_type->primitive_type == primitive::undeclarated3)//?
		{
			typ* new_t = new typ();
			new_t->name = primitive;
			all_types.push_back(new_t);
			new_type->my_type = get_typ(primitive);
		}
		seq->sequence.push_back(new_type);
	}
	else
	{
		//ustawiæ wskaŸnik na ten typ??
	}
}

void Data_types::new_type(typ* seq, string type_name, string primitive, int size)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ();
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->my_type = get_typ(primitive);
		new_type->size = size;
		seq->sequence.push_back(new_type);
	}
}

void Data_types::new_type(typ* seq, string type_name, string primitive, int min, int max)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ();
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->my_type = get_typ(primitive);
		new_type->min_range = min;
		new_type->max_range = max;
		seq->sequence.push_back(new_type);
	}
}

void Data_types::new_type(string type_name, string primitive, int min, int max, bool size)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ;
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->my_type = get_typ(primitive);
		if (size)
		{
			new_type->max_size = max;
			new_type->min_size = min;
		}
		else
		{
			new_type->max_range = max;
			new_type->min_range = min;
		}
		all_types.push_back(new_type);
	}
}

void Data_types::new_type(string type_name, string primitive, int size)
{
	if (!search_typ(type_name))
	{
		typ* new_type = new typ;
		new_type->name = type_name;
		if (primitive == "INTEGER")
			new_type->primitive_type = primitive::integer;
		else if (primitive == "OCTET STRING")
			new_type->primitive_type = primitive::octet_string;
		else if (primitive == "OBJECT IDENTIFIER")
			new_type->primitive_type = primitive::object_identifier;
		else if (primitive == "NULL")
			new_type->primitive_type = primitive::null;
		else
			new_type->my_type = get_typ(primitive);
		new_type->size = size;
		all_types.push_back(new_type);
	}
}

bool Data_types::search_typ(string name)
{
	bool find = false;
	list <typ*>::iterator i;
	for (i = all_types.begin(); i != all_types.end(); i++)
	{
		if (name == (*i)->name)
		{
			find = true;
			exit;
		}
	}
	return find;
}

void Data_types::primitive_type()
{
	for (int i = 0; i < 4; i++)
	{
		typ* new_type = new typ();
		new_type->type_visibility = visibility::universal;
		if (i == 0)
		{
			new_type->name = "INTEGER";
			new_type->primitive_type = primitive::integer;
		}
		else if (i == 1)
		{
			new_type->name = "OCTET STRING";
			new_type->primitive_type = primitive::octet_string;
		}
		else if (i == 2)
		{
			new_type->name = "OBJECT IDENTIFIER";
			new_type->primitive_type = primitive::object_identifier;
		}
		else
		{
			new_type->name = "NULL";
			new_type->primitive_type = primitive::null;
		}
		all_types.push_back(new_type);
	}
}// potrzebne ?

typ* Data_types::get_typ(string name)
{
	bool find = false;
	list <typ*>::iterator i;
	for (i = all_types.begin(); i != all_types.end(); i++)
	{
		typ* a = *i;
		if (name == (*i)->name)
		{
			find = true;
			return a;
			exit;
		}
		else if(a->sequence.size()>0)
		{
			list <typ*>::iterator j;
			for (j = all_types.begin(); j != all_types.end(); j++)
			{
				if (name == (*i)->name)
				{
					a = *j;
					return a;
					exit;
				}
			}
		}
	}
	return nullptr;
}

void Data_types::erase(typ* a)
{
	list <typ*>::iterator i;
	list <typ*>::iterator aa;
	for (i = all_types.begin(); i != all_types.end(); i++)
	{
		if (a == (*i))
		{
			all_types.erase(i);
			break;
		}
	}
}

void Data_types::show_types()// commented writing to file
{
	list <typ*>::iterator i;
	//ofstream outfile;
	//outfile.open("date_types.txt", ios::out | ios::app);
	for (i = all_types.begin(); i != all_types.end(); i++)
	{
		cout << (*i)->name <<endl;
		//outfile << (*i)->name << endl;
		if ((*i)->choice)
		{
			cout << "CHOICE" << endl;
			//outfile << "CHOICE" << endl;
		}
		list <typ*>::iterator j;
		for (j = (*i)->sequence.begin(); j != (*i)->sequence.end(); j++)
		{
			cout <<" "<< (*j)->name << endl;
			//outfile << " " << (*j)->name << endl;
		}
	}
	//outfile.close();
}


Data_types::Data_types()
{
}


Data_types::~Data_types()
{
}