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
	DATA_TYPE P1();	//����������	
	DATA_TYPE P2();
	DATA_TYPE P3();
	DATA_TYPE P4();
	DATA_TYPE P5();
	DATA_TYPE P6();
	DATA_TYPE P7();

	Diagram(TScaner *s) 
	{ 
		this->sc = s; 
		this->tree = new Tree();
		tree->SetSc(sc);
	};
	~Diagram() {};
};