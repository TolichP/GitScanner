#pragma once
#include "Scanner.h"
#include "Tree.h"

class Diagram
{
private:
	TScaner *sc;
	Tree *tree;
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
	Type P1();	//����������	
	Type P2();
	Type P3();
	Type P4();
	Type P5();
	Type P6();
	Type P7();

	Diagram(TScaner *s) 
	{ 
		this->sc = s; 
		this->tree = new Tree();
		tree->SetSc(sc);
	};
	~Diagram() {};
};