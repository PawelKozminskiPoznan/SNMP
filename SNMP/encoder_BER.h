#pragma once
#include <cstdint>
#include <string>
#include "Data_types.h"
#include <list>
#include "ParsingMIBfile.h"


using namespace std;

class encoder_BER
{
public:
	encoder_BER();
	~encoder_BER();
	uint8_t identyficator(typ* typ);
	uint8_t visibility_encoder(visibility vis);
	uint8_t tag_number(typ* typ);
	uint8_t length(typ* typ, string text);
	uint8_t length(typ* typ, int amount);
	uint8_t length();
	int string_length(string text);
	int int_length(int amount);
	list <uint8_t> content(string text);
	list <uint8_t> content(int, int);
	list <uint8_t> content_oid(string oid);
	void show_list(list<uint8_t> list);
	list <uint8_t> encoder_simple(ParsingMIBfile, string);
	list <uint8_t> encoder_simple(ParsingMIBfile, string, string);
	list <uint8_t> encoder_simple(ParsingMIBfile, string, int);
	string get_primitive_type(primitive);
	list <uint8_t> encoder_complex(ParsingMIBfile, string);
};

