
#include "defs.h"
#include "stdafx.h"
#include "Tree.h"


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
		//�������, ����� ����������� �� ������ �����, �� ������ ������ ��� ��� ���� ������� � ��� �� ���������
		while (strcmp(a, node->name) != 0 &&
			node->parent != NULL &&
			node->parent != points.top())
		{
			node = node->parent;
		}
	}
	return strcmp(a, node->name) != 0;
}

//���������� ���������� � �����
//���������� ������ �� ��� ����������
Node* Tree::AddId(TypeLex a, DATA_TYPE t)
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
	//ShowTree();
	//system("pause");
	exit(0);
}


//�������� ��� ������
DATA_TYPE Tree::TypeAnalis(int a)
{
	if (a == Tlong) return TYPE_LONG_INTEGER;
	if (a == Tshort) return TYPE_SHORT_INTEGER;
	PaintError("", "����������� ���");
	return TYPE_UNKNOWN;
}

DATA_TYPE Tree::ConvertTypes(DATA_TYPE type1, DATA_TYPE type2)
{
	if (type1 == TYPE_LONG_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_SHORT_INTEGER) return TYPE_SHORT_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	PaintError("", "���������� �������� ����");
	return TYPE_UNKNOWN;
}

//��������� ������ �����, ������� ������ �������, ���������� cur � ����
void Tree::GetPosition()
{
	points.push(cur);
	Node* node = new Node();
	node->parent = cur;
	cur->right = node;
	cur = node;
}

//��������� ������ ����� � cur
void Tree::SetPosition()
{
	cur = points.top();
	points.pop();
}

//���������� ��� main
void Tree::CheckMain()
{
	Node* node = new Node();
	node->parent = cur;
	cur->left = node;
	cur = node;
}


//������� ���������� �� ��������
Node* Tree::FindId(TypeLex a)
{
	Node* node = cur;
	//�������, ����� ����� �� ����� ��� ����� ���� � ����� ������ � �� ���������
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

// ������������� ������������
//Tree* Tree::Cur = (Tree*)NULL;
TScaner* Tree::sc = (TScaner*)NULL;

// ����� � ������� ���������� � ������ a
// � ������� ���
DATA_TYPE Tree::SemGetType(TypeLex a) {
	Node * v = FindId(a);
	if (v == NULL)
	{
		return TYPE_UNKNOWN;
	}

	return v->type;
}

//�������� �������������� �����
DATA_TYPE Tree::CheckDataTypes(DATA_TYPE type1, DATA_TYPE type2) {
	if (type1 == TYPE_LONG_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_SHORT_INTEGER) return TYPE_SHORT_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	sc->PrintError("���������� ����� ���������� ", "");
	return TYPE_UNKNOWN;
}

DATA_TYPE Tree::SemGetResultType(DATA_TYPE type1, DATA_TYPE type2)
{
	if (type1 == TYPE_LONG_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_SHORT_INTEGER) return TYPE_SHORT_INTEGER;
	if (type1 == TYPE_LONG_INTEGER && type2 == TYPE_SHORT_INTEGER) return TYPE_LONG_INTEGER;
	if (type1 == TYPE_SHORT_INTEGER && type2 == TYPE_LONG_INTEGER) return TYPE_LONG_INTEGER;
	return TYPE_UNKNOWN;
}