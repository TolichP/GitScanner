#pragma once
#include "Scanner.h"
class Diagram
{
private:
	TScaner *sc;
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
	void P1();	//����������	
	void P2();
	void P3();
	void P4();
	void P5();
	void P6();
	void P7();

	Diagram(TScaner *s) { sc = s; };
	~Diagram() {};
};