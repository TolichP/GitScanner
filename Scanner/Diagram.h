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
	TData * P1();	//приоритеты	
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