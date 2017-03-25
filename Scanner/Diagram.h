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
	DATA_TYPE P1();	//приоритеты	
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