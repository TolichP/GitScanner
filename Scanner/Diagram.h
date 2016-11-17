#pragma once
#include "Scanner.h"
class Diagram
{
private:
	TScaner *sc;
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
	void P1();	//приоритеты	
	void P2();
	void P3();
	void P4();
	void P5();
	void P6();
	void P7();

	Diagram(TScaner *s) { sc = s; };
	~Diagram() {};
};