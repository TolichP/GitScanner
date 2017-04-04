#pragma once
#include "Scanner.h"
#include "Tree.h"

class Diagram
{
private:
	TScaner *sc;
	Tree *tree;
	int FlInt;
public:

	void P();	//���������
	void O();	//�������� ����������
	void S();	//������ ����������
	void W();	//������� ������ ����������
	void T();	//���
	void F();	//�������� �������
	void N();	//��� �������
	void B();	//����
	void J();	//���������
	void L();	//��������
	void I();	//��
	void Y();	//������������
	void U();	//����� �������
	void V();	//���������
	TData * P1();	//����������	
	TData * P2();
	TData * P3();
	TData * P4();
	TData * P5();
	TData * P6();
	TData * P7();

	Diagram(TScaner *s) 
	{ 
		this->sc = s; 
		this->tree = new Tree();
		tree->SetSc(sc);
		FlInt = true;
	};
	~Diagram() {};
};