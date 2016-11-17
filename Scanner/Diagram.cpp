#include "stdafx.h"
#include "diagram.h"
#include "defs.h"

//программа
void Diagram::P()
{
	TypeLex l;
	int t1, t2, uk;
	int line;
	line = sc->GetL();
	uk = sc->GetUK(); t1 = sc->Scaner(l); t2 = sc->Scaner(l);  sc->SetUK(uk);
	sc->SetL(line);

	do
	{
		if (t1 == Tvoid)
			F();
		if ((t1 == Tlong) && (t2 == Tint))
			O();
		if ((t1 == Tshort) && (t2 == Tint))
			O();

		line = sc->GetL();
		uk = sc->GetUK(); t1 = sc->Scaner(l); t2 = sc->Scaner(l);  sc->SetUK(uk);
		sc->SetL(line);
	} while ((t1 == Tvoid) || ((t1 == Tlong) || (t1 == Tshort)));
}

//описание функции
void Diagram::F()
{
	TypeLex l;
	int t1;

	t1 = sc->Scaner(l);

	N();

	t1 = sc->Scaner(l);
	if (t1 != Topenbracket) sc->PrintError("ќжидалось \"(\"", "");

	t1 = sc->Scaner(l);
	if (t1 != Tclosebracket) sc->PrintError("ќжидалось \")\"", "");

	B();
}

//блок
void Diagram::B()
{
	TypeLex l;
	int t1;

	t1 = sc->Scaner(l);
	if (t1 != Topenblock) sc->PrintError("ќжидалось \"{\"", "");
	J();
	t1 = sc->Scaner(l);
	if (t1 != Tcloseblock) sc->PrintError("ќжидалось \"}\"", "");
}

//операторы
void Diagram::J()
{
	TypeLex l;
	int t1,uk;
	int line;
	line = sc->GetL();
	sc->SetL(line);

	do {
		line = sc->GetL();
		uk = sc->GetUK();
		t1 = sc->Scaner(l);
		

		if ((t1 == Tlong) || (t1 == Tshort) || (t1 == Tconst))
		{
			sc->SetUK(uk);
			sc->SetL(line);
			O();
		}

		else if ((t1 == Tif) || (t1 == Tid) || (t1 == Topenblock) || (t1 == Treturn) || (t1 == Tsemicolon))
		{
			sc->SetUK(uk);
			sc->SetL(line);
			L();
		}

		if (t1 == Tend) sc->PrintError("Ћишн€€ {", "");
	} while (t1 != Tcloseblock);

	sc->SetUK(uk);
	sc->SetL(line);
}

//им€ функции
void Diagram::N()
{
	TypeLex l;
	int t;
	t = sc->Scaner(l);
	if ((t != Tid) && (t != Tmain)) sc->PrintError("ќжидалс€ идентификатор или main", "");
}

//описание переменных
void Diagram::O()
{
	TypeLex l;
	int t, uk, line;
	t = sc->Scaner(l);


	if (t == Tconst)
	{
		t = sc->Scaner(l);
		if (!((t == Tlong) || (t == Tshort)))
			sc->PrintError("ќжидалось short или long","");

		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("ќжидалось int", "");

		t = sc->Scaner(l);
		if (t != Tid)
			sc->PrintError("ќжидалс€ идентификатор", "");

		t = sc->Scaner(l);
		if (t != Tequal)
			sc->PrintError("ќжидалось =", "");

		t = sc->Scaner(l);
		if ((t != Tconst10) && (t != Tconst16))
			sc->PrintError("ќжидалась константа", "");
	}

	else if ((t == Tlong) || (t == Tshort))
	{

		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("ќжидалось int", "");

		do {
			t = sc->Scaner(l);

			if (t != Tid)
				sc->PrintError("ќжидалс€ идентификатор", "");

			line = sc->GetL();
			uk = sc->GetUK();

			t = sc->Scaner(l);

			if (t == Tequal)
				P1();
			else
			{
				sc->SetUK(uk);
				sc->SetL(line);
			}

			t = sc->Scaner(l);

		} while (t == Tcomma);
	}

	else 
		sc->PrintError("ќшибка в описании переменных", "");

	if (t != Tsemicolon)
		sc->PrintError("ќжидалось ;", "");
}

//if
void Diagram::I()
{
	TypeLex l;
	int t, uk, line;

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if (t != Topenbracket)
		sc->PrintError("ќжидалось (", "");

	else P1();

	if (t != Tclosebracket)
		sc->PrintError("ќжидалось )", "");

	L();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	if (t == Telse)
	{
		L();
	}
	else
	{
		sc->SetUK(uk);
		sc->SetL(line);
	}
}

//оператор
void Diagram::L()
{
	TypeLex l;
	int t, uk, line;

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if (t == Tif)
	{
		//sc->SetUK(uk);
		I();
	}

	else if (t == Tid)
	{
		t = sc->Scaner(l);
		if (t == Tequal)
		{
			sc->SetUK(uk);
			sc->SetL(line);
			P1();
		}
		else if (t == Topenbracket)
		{
			//()
			t = sc->Scaner(l);
			if (t != Tclosebracket)
				sc->PrintError("ќжидалось )", "");
		}

		else (sc->PrintError("ќшибка", ""));

		t = sc->Scaner(l);
		if (t != Tsemicolon)
			sc->PrintError("ќжидалось ;", "");
	}

	else if (t == Topenblock) 
	{
		sc->SetUK(uk);
		sc->SetL(line);
		B();
	}

	else if (t == Treturn)
	{
		t = sc->Scaner(l);
		if (t != Tsemicolon)
			sc->PrintError("ќжидалось ;", "");
	}

	else if (t != Tsemicolon)
	{
		sc->PrintError("ќжидалось ;", "");
	}
}


void Diagram::P1()
{
	TypeLex l;
	int t, uk, line;

	P2();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if (t == Tclosebracket)
	{
		sc->PrintError("ѕустые скобки", "");
		return;
	}

	while (t == Tor)
	{
		P2();
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
}

void Diagram::P2()
{
	TypeLex l;
	int t, uk, line;

	P3();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while (t == Tand)
	{
		P3();
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
}


void Diagram::P3()
{
	TypeLex l;
	int t, uk;
	int line;
	P4();
	line = sc->GetL();
	uk = sc->GetUK();
	//line = sc->GetL();
	t = sc->Scaner(l);
	while ((t == TnotEqual) || (t == Tmore) ||
		(t == Tless) || (t == Tequalmore) || 
		(t == Tequalless) || (t == Tequal))
	{
		P4();
		line = sc->GetL();
		uk = sc->GetUK();
		//line = sc->GetL();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
}

void Diagram::P4()
{
	TypeLex l;
	int t, uk;
	int line;
	P5();
	line = sc->GetL();
	uk = sc->GetUK();
	//line = sc->GetL();
	t = sc->Scaner(l);
	while ((t == Tshiftl) || (t == Tshiftr))
	{
		P5();
		line = sc->GetL();
		uk = sc->GetUK();
		//line = sc->GetL();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
}

void Diagram::P5()
{
	TypeLex l;
	int t, uk;
	int line;
	P6();
	line = sc->GetL();
	uk = sc->GetUK();
	//line = sc->GetL();
	t = sc->Scaner(l);
	while ((t == Tplus) || (t == Tminus))
	{
		P6();
		line = sc->GetL();
		uk = sc->GetUK();
		//line = sc->GetL();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
}

void Diagram::P6()
{
	TypeLex l;
	int t, uk;
	int line;
	P7();
	line = sc->GetL();
	uk = sc->GetUK();
	//line = sc->GetL();
	t = sc->Scaner(l);
	while ((t == Tmul) || (t == Tdiv) || (t == Tpercent))
	{
		P7();
		line = sc->GetL();
		uk = sc->GetUK();
		//line = sc->GetL();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
}

void Diagram::P7()
{
	TypeLex l;
	int t, uk, line;

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if ((t == Tconst10) && (t == Tconst16)) {}

	else if (t == Tid) {}
	else if (t == Topenbracket)
	{
		P1();
		//t = sc->Scaner(l);
		if (t != Tclosebracket) sc->PrintError("ќжидалось )", "");
	}

	else sc->PrintError("ќжидалс€ идентификатор", "");

	// од работал правильно - выдавал сообщение об ошибке, указывал верный номер строки.
	//ќднако сообщение об ошибке было некорректным. 
	//«акоментированный код ниже - сообщение об ошибке некорректно, но с ним счетчик строк работает правильно
	// од выше - сообщение корректно, но падает счетчик строк



	/*
	if ((t != Tid) && (t != Tconst10) && (t != Tconst16))
	{
		if (t == Topenbracket)
		{
			P1();
		}
		else sc->PrintError("ќжидалось (", "");

		if (t != Tclosebracket)
			sc->PrintError("ќжидалось )", "");
	}*/
}


