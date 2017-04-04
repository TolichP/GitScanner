#pragma once
#include "defs.h"
#include "Scanner.h"

#include <stack>
#include <string>

using namespace std;

enum Type {
	EMPTY = -1, 
	TYPE_UNKNOWN = 1, 
	TYPE_SHORT_INTEGER,
	TYPE_LONG_INTEGER, 
	TYPE_FUNCT
};

struct DataValue
{
	short int DataAsShortInt;
	long int DataAsLongInt;
	DataValue()
	{
		DataAsShortInt = 0;
		DataAsLongInt = 0;
	}
	DataValue(DataValue* value)
	{
		DataAsLongInt = value->DataAsLongInt;
		DataAsShortInt = value->DataAsShortInt;
	}
};

struct TData
{
	DataValue* value;
	Type type;

	TData()
	{
		value = new DataValue();
		type = TYPE_UNKNOWN;
	}
};

struct Node {
	TypeLex name;
	Type type;
	
	bool FlagConst;
	Node *parent;
	Node *left;
	Node *right;
	DataValue* value;

	Node()
	{
		FlagConst = false;
		parent = NULL;
		left = NULL;
		right = NULL;
		type = TYPE_UNKNOWN;
		value = new DataValue();
	}
	Node(TypeLex l, Type t, Node* p)
	{
		FlagConst = false;
		parent = p;
		left = NULL;
		right = NULL;
		strcpy(name, l);
		type = t;
		value = new DataValue();
	}
	Node(TypeLex l, Node* s, Node* p)
	{
		FlagConst = false;
		parent = p;
		left = NULL;
		right = NULL;
		strcpy(name, l);
	}
	Node(Node* node)
	{
		FlagConst = false;
		strcpy(name, node->name);
		type = node->type;
		parent = NULL;
		left = NULL;
		right = NULL;
		value = new DataValue(node->value);
	}
};

class Tree {
private:
	Node *root;
	Node *cur;
	stack <Node*> points;
public:
	Tree();
	~Tree();

	static TScaner* sc;
	
	void GetPosition();	//��������� ������ �����, ������� ������ �������, ���������� cur � ����
	void SetPosition();	//��������� ������ ����� � cur
	void CheckMain();	//���������� ��� main
	void ShowTree();
	void PaintError(TypeLex a, string str);
	void SetSc(TScaner *a) { sc = a; };
	void SetValue(DataValue* value, Node* node);

	bool CheckVar(TypeLex a);

	Node* AddId(TypeLex a, Type t, bool f);						//���������� ���������� � �����, ���������� ������ �� ��� ����������
	Node* FindId(TypeLex a);									//������� ���������� �� ��������

	Type TypeAnalis(int a);								//�������� ��� ������
	
	Type CheckDataTypes(Type type1, Type type2);	//�������� �������������� �����. ���������� ��� ����� ����������

	Type SemGetResultType(Type a, Type b);
	Type SemGetType(TypeLex a);							// ����� � ������� ���������� � ������ a � ������� ���
};
	
