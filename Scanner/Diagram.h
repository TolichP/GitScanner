#pragma once
#include "Scanner.h"
#include "Tree.h"

class Diagram
{
private:
	TScaner *sc;
	Tree *tree;
public:

	void P();	//программа
	void O();	//описание переменных
	void S();	//список переменных
	void W();	//элемент списка переменных
	void T();	//тип
	void F();	//описание функции
	void N();	//имя функции
	void B();	//блок
	void J();	//операторы
	void L();	//оператор
	void I();	//иф
	void Y();	//присваивание
	void U();	//вызов функции
	void V();	//выражение
	Type P1();	//приоритеты	
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