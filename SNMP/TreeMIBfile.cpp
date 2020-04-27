#include "pch.h"
#include "TreeMIBfile.h"


using namespace std;

void TreeMIBfile::add_node(string name,string parent_name,int OID, vector <string> parsed, typ* type)
{
	node* new_node = new node;
	new_node->OID_nr = OID;
	new_node->name = name;
	node* parent = search_node_name(parent_name);
	if (parsed.size() > 0)
	{
		new_node->syntax = parsed[1];
		new_node->access = parsed[2];
		new_node->status = parsed[3];
		new_node->description = parsed[5];
	}
	if (type)
		new_node->type = type;
	if (parent)
	{
		new_node->parent = parent;
		parent->children.push_back(new_node);
	}
	else
	{
		cout << "Node: "<< parent_name <<" do not exist. " << endl;
		// tworzenie noda na podstawie TYLKO nazy ??
		//i powi¹zanie go z nowym nodem
		//add_node(parent_name);//only name
	}
}


node* TreeMIBfile::search_node_name(string name)
{
	queue<node*> MIBqueue;
	MIBqueue.push(&root_ISO);
	node* n;
	while (!MIBqueue.empty())
	{
		n = MIBqueue.front();
		MIBqueue.pop();
		if (n->name == name) return n;
		for (int i = 0; i < n->children.size(); i++)
			MIBqueue.push(n->children[i]);
	}
	return nullptr;
}

string TreeMIBfile::get_node_OID(string name)
{
	queue<node*> MIBqueue;
	MIBqueue.push(&root_ISO);
	node* n = nullptr;
	string result = "";
	while (!MIBqueue.empty())
	{
		n = MIBqueue.front();
		MIBqueue.pop();
		if (n->name == name) break;
		for (int i = 0; i < n->children.size(); i++)
			MIBqueue.push(n->children[i]);
	}
	bool parent = true;
	while (parent)
	{
		if (n->parent)
		{
			if (n->OID_nr > 9)
			{
				string nr = to_string(n->OID_nr);
				swap(nr[0], nr[1]);
				result.append(nr);
				result.append(".");
				n = n->parent;
			}
			else
			{
				result.append(to_string(n->OID_nr));
				result.append(".");
				n = n->parent;
			}
		}
		else
			parent = false;
	}
	result.append(to_string(root_ISO.OID_nr));
	int t = result.length();
	for (int i = 0; i < t / 2; i++)
		swap(result[i], result[t - i - 1]);
	cout << name << " has OID nr: " << result << endl;
	return result;
}


node* TreeMIBfile::search_node_OID(queue <int> numbers_OID,string node_oid)
{
	queue<node*> MIBqueue;
	node* n = &root_ISO;
	MIBqueue.push(n);
	int nr;
	while (!numbers_OID.empty()) //	dopóki s¹ sparsowane liczby
	{
		nr = numbers_OID.front();
		numbers_OID.pop();
		bool find = false;
		while (!MIBqueue.empty() && !find) // dopóki s¹ nody do sprawdzenia // i  // n nod nie by³ sprawdzany
		{
			n = MIBqueue.front();
			MIBqueue.pop();
			if (n->OID_nr == nr) // jeœli to ten nod
			{
				if (!MIBqueue.empty()) //  to usuwam go i jego braci jesli sa node do przeszukania
				{
					int size = MIBqueue.size();
					for (int i = 0; i < size; i++)
						MIBqueue.pop();
				}
				int size = n->children.size();
				if (numbers_OID.empty())//jesli nie ma sparsowanych liczb
					return n;
				else
					for (int i = 0; i < size; i++) //dodaje jego dzieci
						MIBqueue.push(n->children[i]);
				find = true;
				
			}
			else if ( numbers_OID.empty() && MIBqueue.empty())//czy sprawdzany nod nie ma dzieci// i  //czy sparsowanych liczb nie ma
			{
				cout << "Node: " << node_oid << " do not exist. " << endl;
				return nullptr;
			}
		}
	}
	cout << "Node: " << node_oid << " do not exist. " << endl;
	return nullptr;
	
}


bool TreeMIBfile::check_exsistance_node(string name)//nie potrzebne ?
{
	queue<node*> MIBqueue;
	node* n = &root_ISO;
	MIBqueue.push(n);
	while (!MIBqueue.empty())
	{
		n = MIBqueue.front();
		MIBqueue.pop();
		if (n->name == name)
			return true;
		for (int i = 0; i < n->children.size(); i++)
			MIBqueue.push(n->children[i]);
	}
	return false;
}


void TreeMIBfile::show_tree(node* n,int level)
{
	queue<node*> MIBqueue;
	MIBqueue.push(n);
	stringstream ss;
	ss << n->OID_nr;
	string OID = ss.str();
	while (!MIBqueue.empty())
	{
		n = MIBqueue.front();
		MIBqueue.pop();
		for (int i = 0; i < level; i++)
			cout << "  ";
		cout << n->name << "  " << OID << endl;
		if (!n->children.empty())
		{
			level++;
			for (int i = 0; i < n->children.size(); i++)
			{
				if (i > 0)
					OID.erase(OID.end()-2,OID.end());
				OID = OID + "." + to_string(n->children[i]->OID_nr);
				show_tree(n->children[i],level);
			}
		}
	}
}

node* TreeMIBfile::getISO()///??? potrzebne
{
	return &root_ISO;
}

TreeMIBfile::TreeMIBfile()
{
	root_ISO.parent = NULL;
	root_ISO.OID_nr = 1;
	root_ISO.children; // DEKLARACJA?
	root_ISO.name = "iso";
}


TreeMIBfile::~TreeMIBfile()
{
}
