#pragma once
#include "defs.h"
#include "Scanner.h"
#include <stack>
#include <string>
#include <iostream>

using namespace std;

enum DATA_TYPE {
	EMPTY = -1, 
	TYPE_UNKNOWN = 1, 
	TYPE_SHORT_INTEGER,
	TYPE_LONG_INTEGER, 
	TYPE_FUNCT
};

struct Node {
	TypeLex name;				// идентификатор переменной
	DATA_TYPE type;				// тип значения
	DATA_TYPE ReturnType;		// тип возвращаемого значения

	Node *parent;
	Node *left;
	Node *right;
	
	short int DataShort;
	long int DataLong;

	Node()
	{
		type = TYPE_UNKNOWN;
		parent = NULL;
		left = NULL;
		right = NULL;
		DataShort = NULL;
		DataLong = NULL;
	}

	Node(TypeLex l, DATA_TYPE t, Node* p)
	{
		parent = p;
		left = NULL;
		right = NULL;
		strcpy(name, l);
		type = t;
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

	static TScaner* sc; //?
	bool CheckVar(TypeLex a);
	Node* AddId(TypeLex a, DATA_TYPE t);

	void PaintError(TypeLex a, string str);

	DATA_TYPE TypeAnalis(int a);
	DATA_TYPE ConvertTypes(DATA_TYPE t1, DATA_TYPE t2);
	Node* FindId(TypeLex a);
	void GetPosition();
	void SetPosition();
	void CheckMain();
	void ShowTree();

	// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
	// установить текущий узел дерева
	void SetSc(TScaner *a) { sc = a; };
	DATA_TYPE CheckDataTypes(DATA_TYPE type1, DATA_TYPE type2);
	DATA_TYPE SemGetResultType(DATA_TYPE a, DATA_TYPE b);
	DATA_TYPE SemGetType(TypeLex a);
};
