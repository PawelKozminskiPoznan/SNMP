#include "pch.h"
#include <iostream>
#include "ParsingMIBfile.h"  
#include "TreeMIBfile.h"
#include <bitset>
#include "encoder_BER.h"
#include <queue>

using namespace std;

int main()
{
	ParsingMIBfile parser;
	parser.load_file("mibs/RFC1213-MIB.txt");
	parser.import_find(); // import additional file defined in RFC1213-MIB.txt
	cout <<"Parsing..."<< endl;
	parser.object_identifier_find();//find and add object identifiers to tree
	parser.data_type.primitive_type();//add primitive types to tree
	parser.data_type_find();// find and add data types to tree
	parser.object_type_find();// find and add object types to tree
	cout << "Press any key to start display parsed object types" << endl;
	cin.ignore();
	parser.data_type.show_types();//display parsed data types
	cout << "Press any key to start display MIB tree" << endl;
	cin.ignore();
	parser.tree.show_tree(parser.tree.getISO());//display MIB tree
	cout << "Press any key to start unite tests" << endl;
	cin.ignore();
	/*node* a = parser.get_OID("1.3.6.1.2.1.2");
	if (a)
		cout <<endl<< a->name<<"  "<< a->OID_nr << endl;
	else
		cout << "node = nullptr" << endl;

	a = parser.tree.search_node_name("tcp");
	if (a)
		cout << a->name << "  " << a->OID_nr << endl;
	else
		cout << "node = nullptr" << endl;
	*/
	encoder_BER encoder;
	encoder.encoder_simple(parser, "INTEGER", -129);
	encoder.encoder_simple(parser, "INTEGER", -128);
	encoder.encoder_simple(parser, "INTEGER", 127);
	encoder.encoder_simple(parser, "INTEGER", 128);
	encoder.encoder_simple(parser, "INTEGER", 2000000000);
	encoder.encoder_simple(parser, "INTEGER (0..65535)", 65533);
	encoder.encoder_simple(parser, "INTEGER (0..65535)", -12);
	encoder.encoder_simple(parser, "DisplayString (SIZE (5))", "abcde");
	encoder.encoder_simple(parser, "DisplayString (SIZE (5))", "abcdezzzzzzz");
	encoder.encoder_simple(parser, "Counter", 12345);//EXPLICIT
	string oid = parser.tree.get_node_OID("snmp");
	encoder.encoder_simple(parser, "OBJECT IDENTIFIER", oid);
	encoder.encoder_simple(parser, "OBJECT IDENTIFIER", "1.3.127");
	encoder.encoder_complex(parser, "INTEGER {up");
	encoder.encoder_complex(parser, "IpAddrEntry");
}