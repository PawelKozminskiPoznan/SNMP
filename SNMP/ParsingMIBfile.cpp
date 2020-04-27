#include "pch.h"
#include "ParsingMIBfile.h"
#include <sstream> 

using namespace std;

void ParsingMIBfile::load_file(string file_named)
{
	fstream* file = new fstream();
	files.push_back(file);
	file->open(file_named, ios::in | ios::out);
	if (file->good() == true)
	{
		cout << "Uzyskano dostep do pliku: "<< "mibs/"<<file_named << std::endl;
		open_ok = true;
		//jesli true to mozna dzialac
	}
	else
	{
		cout << "Brak dostêpu do pliku!" << std::endl;
		open_ok = false;
	}
}


void ParsingMIBfile::import_find()
{
	bool black_list = false;
	pattern = ".*?FROM ([A-Z0-9-]+)(.*|\s*)";
	regex regex_pattern(pattern);
	string pattern2 = ".*?(OBJECT-TYPE)";
	regex regex_black_list(pattern2);
	fstream* fil = files.front();
	while (getline(*fil,line_from_mib) && !black_list)
	{
		if (regex_match(line_from_mib, result, regex_pattern))
		{
			string a = "mibs/";
			a.append(result[1]);
			a.append(".txt");
			load_file(a);
		}
		if (regex_match(line_from_mib, result, regex_black_list))
			black_list = true; //znajduje tylko jeden plik
	}
}


node* ParsingMIBfile::get_OID(string oid_string)
{
	queue <int> oid_int;
	pattern = "([0-9]+)";
	regex regex_pattern(pattern);
	string oid_string_old = oid_string;
	while (regex_search(oid_string, result, regex_pattern))
	{
		stringstream convert(result[1]);
		oid_string = result.suffix().str();
		int j;
		convert >> j;
		oid_int.push(j);
	}
	return tree.search_node_OID(oid_int,oid_string_old);
}


void ParsingMIBfile::object_identifier_find()
{
	vector <fstream*> files2;
	copy(files.begin(), files.end(), back_inserter(files2));
	fstream* fil = files.back();
	string pattern = " *([a-z0-9-]+) +OBJECT IDENTIFIER ::= \\{ ([a-z0-9-]+).* ([0-9]+) \\}";
	regex regex_pattern(pattern);
	string pattern2 = "([a-z0-9-]+\\([0-9]+\\))";
	regex regex_pattern_second(pattern2);
	smatch result_second;
	string pattern3 = "[a-z0-9-]+";
	regex regex_pattern_third(pattern3);
	smatch result_third;
	string pattern3_beta = "([0-9]+?)";
	regex regex_pattern_third_beta(pattern3_beta);
	smatch result_third_beta;
	bool second_search = false;
	bool files_to_search = true;
	bool last = false;
	while (files_to_search)
	{
		getline(*fil, line_from_mib);
		if (regex_match(line_from_mib, result, regex_pattern))
		{
			string first_node_in_bracket = result[1];
			second_search = false;
			stringstream convert(result[3]);
			int j;
			convert >> j;
			string node_grand_parent;
			while (regex_search(line_from_mib, result_second, regex_pattern_second)) // znajduje np. "org(3) dim(2)"
			{
				string syntax = result_second[1];
				if (regex_search(syntax, result_third, regex_pattern_third))
				{
					regex_search(syntax, result_third_beta, regex_pattern_third_beta);
					string node_name = result_third[0];
					stringstream convert(result_third_beta[0]);
					int j;
					convert >> j;
					if (!second_search)
					{
						string node_parent = result[2];
						tree.add_node(node_name, node_parent, j);
					}
					else
						tree.add_node(node_name, node_grand_parent, j);
				}
				if (!result_third.empty())
					node_grand_parent = result_third[0];
				line_from_mib = result_second.suffix().str();
				second_search = true;
			}
			if (!second_search)
				tree.add_node(result[1], result[2], j);
			else
				tree.add_node(first_node_in_bracket, node_grand_parent, j);
		}
		if (fil->eof())
		{
			fil->clear();
			fil->seekg(0, ios::beg);
			files.pop_back();
			if (!files.empty())
				fil = files.back();
			else
				files_to_search = false;
		}
	}
	copy(files2.begin(), files2.end(), back_inserter(files));
}


void ParsingMIBfile::object_type_find()
{
	int iterator_files = files.size() - 1;
	fstream* fil = files.back();
	vector <string> parsed;
	string pattern = "([a-zA-Z-_0-9]*) OBJECT-TYPE";
	regex regex_pattern_obj_name(pattern);
	string pattern2 = " *SYNTAX  (.*)";
	regex regex_pattern_syntax(pattern2);
	smatch result_second;
	string pattern3 = " *ACCESS  (.*)";
	regex regex_pattern_acces(pattern3);
	smatch result_third;
	string pattern3_beta = " *STATUS  (.*)";
	regex regex_pattern_status(pattern3_beta);
	smatch result_third_beta;
	string pattern_description = " *DESCRIPTION";
	regex regex_pattern_description(pattern_description);
	smatch result_description;
	string pattern_end = " *::= \\{ ([a-zA-Z0-9-]+).* ([0-9]+) \\}";
	regex regex_pattern_end(pattern_end);
	smatch result_end;

	string pattern_syntax_type = " *([A-Za-z]+) ?([A-Za-z]+)? ?(.*)? *";
	regex regex_syntax_type(pattern_syntax_type);
	smatch result_syntax_type;
	string pattern_syntax_type_seq = " *SEQUENCE OF ([A-Za-z]+) *";
	regex regex_syntax_type_seq(pattern_syntax_type_seq);
	smatch result_syntax_type_seq;
	string pattern_syntax_type_seq_2 = " *([A-Za-z]+) *\\{ *";
	regex regex_syntax_type_seq_2(pattern_syntax_type_seq_2);
	smatch result_syntax_type_seq_2;
	string pattern_syntax_type_seq_3 = " *([A-Za-z]+) *\\{ ([A-Za-z]+).*?([A-Za-z]+).*";
	regex regex_syntax_type_seq_3(pattern_syntax_type_seq_3);
	smatch result_syntax_type_seq_3;
	string pattern_typ_nr = " *([A-Za-z]+) *\\{(.*)";
	regex regex_typ_nr(pattern_typ_nr);
	smatch result_typ_nr;

	string pattern_size = ".*\\((SIZE).*";
	regex regex_pattern_size(pattern_size);
	smatch result_size;
	string pattern_size_amount = ".*\\(SIZE \\(([0-9]+)\\).*";
	regex regex_size_amount(pattern_size_amount);
	smatch result_size_amount;
	string pattern_size_min_max = ".*SIZE *\\(([0-9]+)..([0-9]+)\\).*";
	regex regex_pattern_size_min_max(pattern_size_min_max);
	smatch result_size_min_max;
	string pattern_min_max = ".*\\(([0-9]+)..([0-9]+)\\).*";
	regex regex_pattern_min_max(pattern_min_max);
	smatch result_min_max;
	string pattern_sequence_variable = " *([A-Za-z]+-?[A-Za-z]+) ?([A-Za-z]+)? ?(.*)? *";
	regex regex_pattern_sequence_variable(pattern_sequence_variable);
	smatch result_sequence_variable;
	string pattern_sequence_one_type = " *([A-Za-z0-9]+-?[A-Za-z0-9]+).*";
	regex regex_sequence_one_type(pattern_sequence_one_type);
	smatch result_sequence_one_type;
	string pattern_sequence_end = " *(\\}) *";
	regex regex_pattern_sequence_end(pattern_sequence_end);
	smatch result_sequence_end;
	bool obj_type = false;
	bool syntax = false;
	bool acces = false;
	bool status = false;
	bool description = false;
	bool files_to_search = true;
	string description_text = "";
	typ *type = nullptr;
	while (files_to_search)
	{
		getline(*fil, line_from_mib);
		if (regex_match(line_from_mib, result, regex_pattern_obj_name) && !obj_type)
		{
			parsed.push_back(result[1]);
			obj_type = true;
		}
		if (regex_match(line_from_mib, result_second, regex_pattern_syntax) && !syntax)
		{
			parsed.push_back(result_second[1]);// zamist tego wskaŸnik na typ
			string line = result_second[1];
			string id;
			if (data_type.search_typ(line) && !(regex_match(line, result_syntax_type_seq_2, regex_syntax_type_seq_2)))
			{
				type = data_type.get_typ(line);
			}
			else //dynamiczne tworzenie nowego typu danych
			{
				if (regex_match(line, result_syntax_type_seq, regex_syntax_type_seq))	// parsed: SEQUENCE OF ...
				{
					string seq = result_syntax_type_seq[1];
					data_type.new_type(line, seq);
					type = data_type.get_typ(line);
				}
				else if (regex_match(line, result_syntax_type_seq_2, regex_syntax_type_seq_2))// parsed: INTEGER{
				{
					string seq_primitive = result_syntax_type_seq_2[1];
					string name;
					typ* seq = nullptr;
					bool end_of_seq = false;
					if (!data_type.search_typ(line))
					{
						seq = data_type.new_type_sequence(line, seq_primitive);
						getline(*fil, line_from_mib);
					}
					else
					{
						getline(*fil, line_from_mib);
						if (regex_match(line_from_mib, result_sequence_one_type, regex_sequence_one_type))// parsed: other(1), 
						{
							name = result_sequence_one_type[1];
							line.append(name);
							if (data_type.search_typ(line))
								end_of_seq = true;
							seq = data_type.new_type_sequence(line, seq_primitive);
						}
					}
					while (!end_of_seq)
					{
						if (regex_match(line_from_mib, result_sequence_one_type, regex_sequence_one_type))// parsed: other(1), 
						{
							name = result_sequence_one_type[1];
							data_type.new_type(seq, name, seq_primitive);
						}
						getline(*fil, line_from_mib);
						if (regex_match(line_from_mib, result_sequence_end, regex_pattern_sequence_end))
						{
							end_of_seq = true;
						}
					}
					type = data_type.get_typ(line);
				}
				else if (regex_match(line, result_syntax_type_seq_3, regex_syntax_type_seq_3))// parsed: INTEGER{ as(0)
				{
					bool exist = true;
					string seq_primitive = result_syntax_type_seq_3[1];
					string name;
					typ* seq = nullptr;
					if (!data_type.search_typ(line))
					{
						seq = data_type.new_type_sequence(line, seq_primitive);
						exist = false;
					}
					else// nie powtarzaj¹ siê wiec nie potrzebne ?
					{
						name = result_syntax_type_seq_3[2];
						line.append(name);
						if (data_type.search_typ(line))
							exist = true;
						else
							exist = false;
						seq = data_type.new_type_sequence(line, seq_primitive);
					}
					if (!exist)
					{
						name = result_syntax_type_seq_3[2];
						data_type.new_type(seq, name, seq_primitive);
						name = result_syntax_type_seq_3[3];
						data_type.new_type(seq, name, seq_primitive);
					}
					type = data_type.get_typ(line);
				}
				else if (regex_match(line, result_syntax_type, regex_syntax_type))// parsed: INTEGER
				{
					string type_name = result_syntax_type[1];
					if (result_syntax_type[2] == "IDENTIFIER" || result_syntax_type[2] == "STRING")
					{
						type_name.append(" ");
						type_name.append(result_syntax_type[2]);
					}
					if (regex_match(line, result_size_min_max, regex_pattern_size_min_max)) // parsed: SIZE (0..255)
					{
						stringstream convert2(result_size_min_max[1]);
						id = result_size_min_max.suffix().str();
						int k;
						convert2 >> k;
						stringstream convert3(result_size_min_max[2]);
						id = result_size_min_max.suffix().str();
						int i;
						convert3 >> i;
						data_type.new_type(line, type_name, k, i, true);
						type = data_type.get_typ(line);
					}
					else if (regex_match(line, result_min_max, regex_pattern_min_max)) // parsed: (0..255)
					{
						stringstream convert2(result_min_max[1]);
						id = result_min_max.suffix().str();
						int k;
						convert2 >> k;
						stringstream convert3(result_min_max[2]);
						id = result_min_max.suffix().str();
						int i;
						convert3 >> i;
						data_type.new_type(line, type_name, k, i, false);
						type = data_type.get_typ(line);
					}
					else  if (regex_match(line, result_size_amount, regex_size_amount)) // parsed: SIZE(5)
					{
						stringstream convert2(result_size_amount[1]);
						id = result_size_amount.suffix().str();
						int k;
						convert2 >> k;
						data_type.new_type(line, type_name, k);
						type = data_type.get_typ(line);
					}
					else // nie wchodzi tu
					{
						data_type.new_type(line, type_name);
						type = data_type.get_typ(line);
					}
				}
			}
			syntax = true;
		}
		if (regex_match(line_from_mib, result_third, regex_pattern_acces) && !acces)
		{
			parsed.push_back(result_third[1]);
			acces = true;
		}
		if (regex_match(line_from_mib, result_third_beta, regex_pattern_status) && !status)
		{
			parsed.push_back(result_third_beta[1]);
			status = true;
		}

		if (description)
		{
			if (regex_match(line_from_mib, result_end, regex_pattern_end))
			{
				parsed.push_back(description_text);
				description_text = "";
				parsed.push_back(result_end[1]);
				parsed.push_back(result_end[2]);
				stringstream convert(parsed[7]);
				int j;
				convert >> j;
				tree.add_node(parsed[0], parsed[6], j, parsed, type);
				parsed.clear();
				type = nullptr;
				obj_type = false;
				syntax = false;
				acces = false;
				status = false;
				description = false;
				}
			else
			{
				description_text.append(line_from_mib);
				description_text += '\n';
			}
		}
		if (regex_match(line_from_mib, result_description, regex_pattern_description) && !description)
		{
			parsed.push_back(result_description[1]);
			description = true;
		}

		if (fil->eof())
		{
			iterator_files = iterator_files - 1;
			if (iterator_files < 0)
				files_to_search = false;
			else
				fil = files[iterator_files];
		}
	}

}


void ParsingMIBfile::data_type_find() // bez SEQUENCE OF < >  // bez Size (roz min, roz max)
{
	vector <fstream*> files2;
	copy(files.begin(), files.end(), back_inserter(files2));
	fstream* fil = files.back();
	string pattern_data_type_name = " *([A-Za-z-]+) ::= *";
	regex regex_pattern_data_type_name(pattern_data_type_name);
	string pattern_bracket = " *\\[([A-Za-z]+) ([0-9]+)\\].*";
	regex regex_pattern_bracket(pattern_bracket);
	smatch result_bracket;
	string pattern_im_ex_primitive = " *([A-Za-z-]+) ([A-Za-z]+).*";
	regex regex_pattern_im_ex_primitive(pattern_im_ex_primitive);
	smatch result_im_ex_primitive;
	string pattern_primitive_2 = " *[A-Za-z]+ [A-Za-z]+ ([A-Za-z]+).*";
	regex regex_pattern_primitive_2(pattern_primitive_2);
	smatch result_primitive_2;
	string pattern_size = ".*\\((SIZE).*";
	regex regex_pattern_size(pattern_size);
	smatch result_size;
	string pattern_size_amount = ".*\\(SIZE \\(([0-9]+)\\).*";
	regex regex_pattern_size_amount(pattern_size_amount);
	smatch result_size_amount;
	string pattern_min_max = ".*\\(([0-9]+)..([0-9]+)\\).*";
	regex regex_pattern_min_max(pattern_min_max);
	smatch result_min_max;
	string pattern_sequence = ".*SEQUENCE.*";
	regex regex_pattern_sequence(pattern_sequence);
	smatch result_sequence;
	string pattern_sequence_variable = " *([A-Za-z]+-?[A-Za-z]+) ?([A-Za-z]+)? ?(.*)?,? *";
	regex regex_pattern_sequence_variable(pattern_sequence_variable);
	smatch result_sequence_variable;
	string pattern_sequence_end = " *(\\}) *";
	regex regex_pattern_sequence_end(pattern_sequence_end);
	smatch result_sequence_end;
	string pattern_typ_short = " *([A-Za-z]+) ?([A-Za-z]+)? *";
	regex regex_pattern_typ_short(pattern_typ_short);
	smatch result_typ_short;
	string pattern_size_ogr = ".*\\(SIZE \\(([0-9]+)\\).*";
	regex regex_pattern_size_ogr(pattern_size_ogr);
	smatch result_size_ogr;
	string pattern_minmax_ogr = ".*\\(([0-9]+)..([0-9]+)\\).*";
	regex regex_pattern_minmax_ogr(pattern_minmax_ogr);
	smatch result_minmax_ogr;
	string pattern_choice = " *CHOICE \\{.*";
	regex regex_pattern_choice(pattern_choice);
	smatch result_choice;
	bool files_to_search = true;
	while (files_to_search)
	{
		getline(*fil, line_from_mib);
		if (regex_match(line_from_mib, result, regex_pattern_data_type_name))
		{
			string type_name = result[1];
			getline(*fil, line_from_mib);
			if (regex_match(line_from_mib, result_bracket, regex_pattern_bracket)) // parsed: TimeTicks ::=
																					//[APPLICATION 3]
																					//IMPLICIT INTEGER(0..4294967295)
			{
				string visibility = result_bracket[1];
				string id = result_bracket[2];
				stringstream convert(result_bracket[2]);
				id = result_bracket.suffix().str();
				int j;
				convert >> j;
				getline(*fil, line_from_mib);
				if (regex_match(line_from_mib, result_im_ex_primitive, regex_pattern_im_ex_primitive))
				{
					string im_ex = result_im_ex_primitive[1];
					string prim_1 = result_im_ex_primitive[2];
					if (result_im_ex_primitive[2] == "OBJECT" || result_im_ex_primitive[2] == "OCTET")
					{
						if (regex_match(line_from_mib, result_primitive_2, regex_pattern_primitive_2))
						{
							prim_1.append(" ");
							prim_1.append(result_primitive_2[1]);
						}
					}
					if (regex_match(line_from_mib, result_size_amount, regex_pattern_size_amount))
					{
						stringstream convert5(result_size_amount[1]);
						id = result_size_amount.suffix().str();
						int w;
						convert5 >> w;
						data_type.new_type(type_name, prim_1, visibility, j, im_ex,w);
					}
					else if (regex_match(line_from_mib, result_min_max, regex_pattern_min_max))
					{
						stringstream convert2(result_min_max[1]);
						id = result_min_max.suffix().str();
						int k;
						convert2 >> k;
						stringstream convert3(result_min_max[2]);
						id = result_min_max.suffix().str();
						int i;
						convert3 >> i;
						data_type.new_type(type_name, prim_1, visibility, j, im_ex,j,i);
					}
					else
					{
						data_type.new_type(type_name, prim_1, visibility,j, im_ex);
					}
				}

			}
			else if (regex_match(line_from_mib, result_sequence, regex_pattern_sequence))
			{
				typ* seq = data_type.new_type_sequence(type_name);
				getline(*fil, line_from_mib);
				bool end_of_seq = false;
				while (!end_of_seq)
				{
					string type_var_name;
					if (regex_match(line_from_mib, result_sequence_variable, regex_pattern_sequence_variable))
					{
						type_var_name = result_sequence_variable[1];
					}
					getline(*fil, line_from_mib);
					if (regex_match(line_from_mib, result_sequence_variable, regex_pattern_sequence_variable))
					{
						string type = result_sequence_variable[1];
						if (result_sequence_variable[2] == "IDENTIFIER" || result_sequence_variable[2] == "STRING")
						{
							type.append(" ");
							type.append(result_sequence_variable[2]);
						}
						if (regex_match(line_from_mib, result_size_ogr, regex_pattern_size_ogr))
						{
							string size = result_size_ogr[1];
							stringstream convert4(result_size_ogr[1]);
							size = result_bracket.suffix().str();
							int z;
							convert4 >> z;
							data_type.new_type(seq, type_var_name, type, z);
						}
						else if (regex_match(line_from_mib, result_minmax_ogr, regex_pattern_minmax_ogr))
						{
							string size2 = result_minmax_ogr[1];
							stringstream convert6(result_minmax_ogr[1]);
							size2 = result_bracket.suffix().str();
							int x;
							convert6 >> x;
							string size = result_minmax_ogr[2];
							stringstream convert7(result_minmax_ogr[2]);
							size = result_bracket.suffix().str();
							int c;
							convert7 >> c;
							data_type.new_type(seq, type_var_name, type, x,c);
						}
						else
							data_type.new_type(seq, type_var_name, type);
					}
					getline(*fil, line_from_mib);
					if (regex_match(line_from_mib, result_sequence_end, regex_pattern_sequence_end))
					{
						end_of_seq = true;
					}
				}
			}
			else if (regex_match(line_from_mib, result_choice, regex_pattern_choice))
			{
				typ* choice = data_type.new_type_choice(type_name);
				bool end_of_choice = false;
				while (!end_of_choice)
				{
					getline(*fil, line_from_mib);
					string type_var_name;
					if (regex_match(line_from_mib, result_sequence_variable, regex_pattern_sequence_variable))
					{
						type_var_name = result_sequence_variable[1];
						getline(*fil, line_from_mib);
					}
					if (regex_match(line_from_mib, result_sequence_variable, regex_pattern_sequence_variable))
					{
						string type = result_sequence_variable[1];
						if (result_sequence_variable[2] == "IDENTIFIER" || result_sequence_variable[2] == "STRING")
						{
							type.append(" ");
							type.append(result_sequence_variable[2]);
						}
						if (regex_match(line_from_mib, result_size_ogr, regex_pattern_size_ogr))
						{
							string size = result_size_ogr[1];
							stringstream convert4(result_size_ogr[1]);
							size = result_bracket.suffix().str();
							int z;
							convert4 >> z;
							data_type.new_type(choice, type_var_name, type, z);
						}
						else if (regex_match(line_from_mib, result_minmax_ogr, regex_pattern_minmax_ogr))
						{
							string size2 = result_minmax_ogr[1];
							stringstream convert6(result_minmax_ogr[1]);
							size2 = result_bracket.suffix().str();
							int x;
							convert6 >> x;
							string size = result_minmax_ogr[2];
							stringstream convert7(result_minmax_ogr[2]);
							size = result_bracket.suffix().str();
							int c;
							convert7 >> c;
							data_type.new_type(choice, type_var_name, type, x, c);
						}
						else
						{
							data_type.new_type(choice, type_var_name, type);
						}
					}
					if (regex_match(line_from_mib, result_sequence_end, regex_pattern_sequence_end))
					{
						end_of_choice = true;
					}
				}
			}
			else
			{
				if (regex_match(line_from_mib, result_typ_short, regex_pattern_typ_short))
				{
					string primitive = result_typ_short[1];
					string part_two_name = result_typ_short[2];
					if (part_two_name != "")
					{
						primitive.append(" ");
						primitive.append(result_typ_short[2]);
					}
					data_type.new_type(type_name,primitive);
				}
			}
		}
		if (fil->eof())
		{
			fil->clear();
			fil->seekg(0, ios::beg);
			files.pop_back();
			if (!files.empty())
				fil = files.back();
			else
				files_to_search = false;
		}
	}
	copy(files2.begin(), files2.end(), back_inserter(files));
}


ParsingMIBfile::ParsingMIBfile()
{
	TreeMIBfile tree;
}


ParsingMIBfile::~ParsingMIBfile()
{
	while (!files.empty())
	{
		files.back()->close();
		files.pop_back();
		// delete fstream
	}
}
