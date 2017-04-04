#pragma once
#include "defs.h"
#include "Scanner.h"

#include <stack>
#include <string>

using namespace std;

enum DATA_TYPE {
	EMPTY = -1, 
	TYPE_UNKNOWN = 1, 
	TYPE_SHORT_INTEGER,
	TYPE_LONG_INTEGER, 
	TYPE_FUNCT
};

struct Node {
	TypeLex name;
	DATA_TYPE type;
	DATA_TYPE ReturnType;

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

	static TScaner* sc;
	
	void GetPosition();	//��������� ������ �����, ������� ������ �������, ���������� cur � ����
	void SetPosition();	//��������� ������ ����� � cur
	void CheckMain();	//���������� ��� main
	void ShowTree();
	void PaintError(TypeLex a, string str);
	void SetSc(TScaner *a) { sc = a; };

	bool CheckVar(TypeLex a);

	Node* AddId(TypeLex a, DATA_TYPE t);						//���������� ���������� � �����, ���������� ������ �� ��� ����������
	Node* FindId(TypeLex a);									//������� ���������� �� ��������

	DATA_TYPE TypeAnalis(int a);								//�������� ��� ������
	
	DATA_TYPE CheckDataTypes(DATA_TYPE type1, DATA_TYPE type2);	//�������� �������������� �����. ���������� ��� ����� ����������

	DATA_TYPE SemGetResultType(DATA_TYPE a, DATA_TYPE b);
	DATA_TYPE SemGetType(TypeLex a);							// ����� � ������� ���������� � ������ a � ������� ���
};
	
