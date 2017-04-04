
#include "defs.h"
#include "stdafx.h"
#include "Tree.h"

#include <iostream>

#define max(a,b) a<b ? b : a

Tree::Tree(void) {
	root = new Node();
	cur = root;
}

bool Tree::CheckVar(TypeLex a)
{
	Node* node = cur;

	if (points.size() > 0)
	{
		//�������, ����� ����������� �� ������ �����, �� ������ ������ ��� ��� ���� �������
		while (strcmp(a, node->name) != 0 &&
			node->parent != NULL &&
			node->parent != points.top())
		{
			node = node->parent;
		}
	}
	return strcmp(a, node->name) != 0;
}

void Tree::SetValue(DataValue* _value, Node* node)
{
	node->value = _value;
}

Node* Tree::AddId(TypeLex a, Type t)
{
	if (CheckVar(a))
	{
		Node* node = new Node(a, t, cur);
		cur->left = node;
		cur = node;
		return node;
	}
	else
	{
		PaintError(a, "���������� � ����� ������ ��� �������");
		return NULL;
	}
}

void Tree::PaintError(TypeLex a, string str)
{
	if (a[0] == '\0')
		cout << "������: " << str << endl;
	else
		cout << "������: " << str << ". �������� ������: " << a << endl;
	system("pause");
	exit(0);
}

Type Tree::TypeAnalis(int a)
{
	if (a == Tlong) return TYPE_LONG_INTEGER;
	if (a == Tshort) return TYPE_SHORT_INTEGER;
	PaintError("", "����������� ���");
	return TYPE_UNKNOWN;
}

void Tree::GetPosition()
{
	points.push(cur);
	Node* node = new Node();
	node->parent = cur;
	cur->right = node;
	cur = node;
}


void Tree::SetPosition()
{
	cur = points.top();
	points.pop();
}

void Tree::CheckMain()
{
	Node* node = new Node();
	node->parent = cur;
	cur->left = node;
	cur = node;
}

Node* Tree::FindId(TypeLex a)
{
	Node* node = cur;
	//�������, ����� ����� �� ����� ��� ����� ���� � ����� ������
	while (node->parent != NULL && strcmp(a, node->name) != 0)
	{
		node = node->parent;
	}
	if (strcmp(a, node->name) == 0) return node;
	else
	{
		PaintError(a, "�� ������� ����������");
		return NULL;
	}
}

void Tree::ShowTree()
{
	Node* node = root;
	stack<Node*> st;
	st.push(node);
	cout << "������" << endl;
	while (!st.empty())
	{
		node = st.top();
		st.pop();
		cout << node->name << ": ";
		if (node->left != NULL) { cout << node->left->name << " | "; st.push(node->left); }
		else cout << " - | ";
		if (node->right != NULL) { cout << node->right->name << endl; st.push(node->right); }
		else cout << " -" << endl;
	}
}

TScaner* Tree::sc = (TScaner*)NULL;

Type Tree::SemGetType(TypeLex a) {
	Node * v = FindId(a);
	if (v == NULL)
	{
		return TYPE_UNKNOWN;
	}

	return v->type;
}


Type Tree::CheckDataTypes(Type type1, Type type2) {
	if (type1 == TYPE_LONG_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_SHORT_INTEGER) return TYPE_SHORT_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	sc->PrintError("���������� ����� ���������� ", "");
	return TYPE_UNKNOWN;
}

Type Tree::SemGetResultType(Type type1, Type type2)
{
	if (type1 == TYPE_LONG_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_SHORT_INTEGER) return TYPE_SHORT_INTEGER;
	if (type1 == TYPE_LONG_INTEGER && type2 == TYPE_SHORT_INTEGER) return TYPE_LONG_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	return TYPE_UNKNOWN;
}

//Type Tree::ConstType(int a)
//{
//	if (a == Tconst10) return TYPE_SHORT_INTEGER;
//	if (a == tlon) return TypeDouble;
//	PaintError("", "����������� ��� ���������");
//	return TypeUnknown;
//}