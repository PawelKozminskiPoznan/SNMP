#include "pch.h"
#include "encoder_BER.h"
#include <bitset>


encoder_BER::encoder_BER()
{
}


encoder_BER::~encoder_BER()
{
}

uint8_t encoder_BER::identyficator(typ* typ)
{
	uint8_t vis = visibility_encoder(typ->type_visibility);
	uint8_t pri_con = 0;
	if (typ->sequence.size() > 0 || typ->type_evidence == evidence::explicitt)
	{
		pri_con = 32;
	}
	 // primitive=0 vs constructed=32 tz. czy prosty czy z³o¿ony (sekwencja, string?)
	// dla octet_string mo¿e byæ z³o¿one jako kodowanie o nieznanej d³ugoœci
	uint8_t tag = tag_number(typ);
	uint8_t byte = vis + pri_con + tag;
	return byte;
}

uint8_t encoder_BER::visibility_encoder(visibility vis)
{
	uint8_t result = 0;
	if (vis == visibility::application)
		result = 64;
	else if (vis == visibility::context_specific)
		result = 128;
	else if (vis == visibility::privatee)
		result = 192;
	else if (vis == visibility::undeclarated) 
		result = 128; //jako context_specific ?
	return result;
}

uint8_t encoder_BER::tag_number(typ * typ)
{
	uint8_t result;
	if (typ->id_type != -1)
		result = typ->id_type;
	else
	{
		if (typ->primitive_type == primitive::integer)
			result = 2;
		else if (typ->primitive_type == primitive::octet_string)
			result = 4;
		else if (typ->primitive_type == primitive::null)
			result = 5;
		else if (typ->primitive_type == primitive::object_identifier)
			result = 6;
		else if (typ->primitive_type == primitive::undeclarated3) // sekwencje
			result = 16;
	}
	return result;
}

uint8_t encoder_BER::length(typ * typ, string text)
{
	uint8_t length_enc;
	length_enc = string_length(text); // jeœli string mniejszy/równy od 127 
	length_enc = length_enc + 0; // okreœlona d³ugoœæ=0, nieokreœlona d³ugoœæ=128
	return length_enc;
}

int encoder_BER::string_length(string text)
{
	int size = text.length();
	return size;
}

uint8_t encoder_BER::length(typ * typ, int amount)
{
	uint8_t length_enc;
	length_enc = int_length(amount); // jeœli string mniejszy/równy od 127 bajtów
	length_enc = length_enc + 0; // okreœlona d³ugoœæ=0, nieokreœlona d³ugoœæ=128
	return length_enc;
}


int encoder_BER::int_length(int amount)//ma znaczenie czy int czy unsigned int
{
	unsigned int value;
	int byte = 1;
	int max_amoutn_of_byte = 5; // dla jednego liczby ogólnie w okreœlonej d³ugoœci mo¿e byæ do 127 bajtów
	if (amount >= 0)
	{
		for (int i = 7; i <= max_amoutn_of_byte * 8; i = i + 8)
		{
			value = pow(2, i) - 1;
			if (amount <= value)
				return byte;
			byte++;
		}
	}
	else
	{
		for (int i = 7; i <= max_amoutn_of_byte * 8; i = i + 8)
		{
			value = pow(2, i);
			if (abs(amount) <= value)
				return byte;
			byte++;
		}
	}
}

uint8_t encoder_BER::length()
{
	uint8_t length_enc = 0; // okreœlona d³ugoœæ=0, nieokreœlona d³ugoœæ=128
	//null = 0
	return length_enc;
}

list<uint8_t> encoder_BER::content(string text)
{
	list <uint8_t> content;
	for (int i = 0; i < text.size(); i++)
	{
		uint8_t sing = text[i];
		content.push_back(sing);
	}
	return content;
}

list<uint8_t> encoder_BER::content(int amount_bytes,int amount)// przy z³o¿onym b³edna wartoœæ amount_bytes
{
	list <uint8_t> content;
	uint8_t byte;
	for (int i = 8 * amount_bytes - 8; i >= 0; i = i - 8)
	{
		int value = amount;
		value >>= i;
		byte = value;
		content.push_back(value);
	}
	return content;
}

void encoder_BER::show_list(list<uint8_t> list_byte)
{
	list <uint8_t>::iterator i;
	for (i = list_byte.begin(); i != list_byte.end(); i++)
	{
		cout << bitset<8>(*i) << endl;
	}
	cout << endl;
}

list<uint8_t> encoder_BER::encoder_simple(ParsingMIBfile parser, string type) //null
{
	list <uint8_t> content_byte;
	typ* typ = parser.data_type.get_typ(type);
	if (typ)
	{
		uint8_t tag = identyficator(typ);
		uint8_t length_enc = length();
		cout << "Value: null" << endl;
		cout << "Tag: " << bitset<8>(tag) << endl;
		cout << "length: " << bitset<8>(length_enc) << endl;
		content_byte.push_front(length_enc);
		content_byte.push_front(tag);
		return content_byte;
	}
	else
	{
		cout << "ERROR TYPE" << endl;
		return content_byte;
	}
}

list<uint8_t> encoder_BER::encoder_simple(ParsingMIBfile parser, string type, string value) //string
{
	list <uint8_t> content_byte;
	typ* typ = parser.data_type.get_typ(type);
	if (typ)
	{
		if (type == "OBJECT IDENTIFIER" )
		{
			uint8_t tag = identyficator(typ);
			list<uint8_t> content_byte = content_oid(value);
			uint8_t length = content_byte.size();
			cout << "Value: " << value << endl;
			cout << "Tag: " << bitset<8>(tag) << endl;
			cout << "length: " << bitset<8>(length) << endl;
			cout << "content: " << endl;
			show_list(content_byte);
			content_byte.push_front(length);
			content_byte.push_front(tag);
			return content_byte;
		}
		else
		{
			bool restricted = false;
			if (typ->max_size != -1 && typ->min_size != -1 || typ->size != -1)
				restricted = true;
			if (value.size() <= typ->max_size && value.size() >= typ->min_size || value.size() <= typ->size || !restricted)
			{
				uint8_t tag = identyficator(typ);
				uint8_t length_enc = length(typ, value);
				content_byte = content(value);//explicit zrobiæ
				cout << "Value: " << value << endl;
				cout << "Tag: " << bitset<8>(tag) << endl;
				cout << "length: " << bitset<8>(length_enc) << endl;
				cout << "content: " << endl;
				show_list(content_byte);
				content_byte.push_front(length_enc);
				content_byte.push_front(tag);
				return content_byte;
			}
			else
			{
				cout << "Niepoprawne dane" << endl<<endl;
				return content_byte;
			}
		}
	}
	else
	{
		cout << "ERROR TYPE" << endl<<endl;
		return content_byte;
	}
}

list<uint8_t> encoder_BER::content_oid(string oid)
{
	list <uint8_t> content;
	uint8_t byte;
	string num = "";
	int sing = 0;
		for (int i = 0; i <= oid.size(); i++)
		{
			if (oid[i] != '.')
			{
				string temp = to_string(oid[i]);
				num.append(temp);
			}
			else
			{
				if (i == 1)
				{
					stringstream geek(num);
					geek >> sing;
					sing = sing * 40;
				}
				else if (i == 3)
				{
					sing += 3;
					byte = sing;
					content.push_back(byte);
					sing = 0;
					num = "";
				}
				else
				{
					stringstream geek(num);
					geek >> sing;
					byte = sing;
					content.push_back(byte);
					num = "";
				}
			}
			if (i == oid.size())
			{
				stringstream geek(num);
				geek >> sing;
				byte = sing;
				content.push_back(byte);
			}
		}
	return content;
}

list<uint8_t> encoder_BER::encoder_simple(ParsingMIBfile parser, string name_type, int value) //int
{
	list <uint8_t> content_byte;
	typ* type = parser.data_type.get_typ(name_type);
	if (type)
	{
		bool restricted = false;
		if (type->max_range != -1 && type->min_range != -1)
			restricted = true;
		if (value <= type->max_range && value >= type->min_range || !restricted)
		{
			uint8_t length_enc;
			uint8_t tag = identyficator(type);
			if (type->type_evidence == evidence::explicitt)
			{
				content_byte = encoder_simple(parser, get_primitive_type(type->primitive_type), value);
				length_enc = content_byte.size();
			}
			else
			{
				length_enc = length(type, value);
				content_byte = content(length_enc, value);
			}
			cout << "Value: " << value << endl;
			cout << "Tag: " << bitset<8>(tag) << endl;
			cout << "length: " << bitset<8>(length_enc) << endl;
			cout << "content: " << endl;
			show_list(content_byte);
			content_byte.push_front(length_enc);
			content_byte.push_front(tag);
			return content_byte;
		}
		else
		{
			cout << "Niepoprawne dane" << endl<<endl;
			return content_byte;
		}
	}
	else
	{
		cout << "ERROR TYPE" << endl<<endl;
		return content_byte;
	}
}

string encoder_BER::get_primitive_type(primitive prim)
{
	if (prim == primitive::integer)
		return "INTEGER";
	else if (prim == primitive::octet_string)
		return "OCTET STRING";
	else if (prim == primitive::object_identifier)
		return "OBJECT IDENTIFIER";
	else if (prim == primitive::null)
		return "NULL";
	else
		return "";
}

list<uint8_t> encoder_BER::encoder_complex(ParsingMIBfile parser, string name_type)
{
	list <uint8_t> content_byte;
	typ* type = parser.data_type.get_typ(name_type);
	if (type)
	{
		uint8_t length_enc;
		uint8_t tag = identyficator(type);
		if (type->sequence.size() > 0)
		{
			typ* current = nullptr;
			list <typ*>::iterator i;
			for (i = type->sequence.begin(); i != type->sequence.end(); i++)
			{
				current = *i;
				if (current->primitive_type == primitive::undeclarated3)
				{
					current = current->my_type;
				}
				if (current->primitive_type == primitive::integer)
				{
					list <uint8_t> subtype_content;
					int value;
					string name_subtyp = current->name;
					if (!parser.data_type.get_typ(name_subtyp))
						name_subtyp = "INTEGER";
					cout << "Wpisz wartoœæ(int): " << endl;
					cin >> value;
					subtype_content = encoder_simple(parser, name_subtyp, value);
					content_byte.insert(content_byte.end(), subtype_content.begin(), subtype_content.end());
				}
				else if (current->primitive_type == primitive::octet_string)
				{
					list <uint8_t> subtype_content;
					string value;
					string name_subtyp = current->name;
					if (!parser.data_type.get_typ(name_subtyp))
						name_subtyp = "OCTET STRING";
					cout << "Wpisz wartoœæ(string): " << endl;
					cin >> value;
					subtype_content = encoder_simple(parser, name_subtyp, value);
					content_byte.insert(content_byte.end(), subtype_content.begin(), subtype_content.end());
				}
			}
			cout << "Sequence type: " << name_type << endl;
			cout << "Tag: " << bitset<8>(tag) << endl;
			length_enc = content_byte.size();
			cout << "Length: " << bitset<8>(length_enc) << endl;
			show_list(content_byte);
			content_byte.push_front(length_enc);
			content_byte.push_front(tag);
			return content_byte;
		}
		else
			cout << "It is not a sequence!" << endl<<endl;
	}
	else
	{
		cout << "ERROR TYPE" << endl<<endl;
		return content_byte;
	}
	return content_byte;
}


