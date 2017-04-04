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
	int t1, uk;

	t1 = sc->Scaner(l);
	
	int line = sc->GetL();
	uk = sc->GetUK();
	t1 = sc->Scaner(l);
	
	tree->AddId(l, TYPE_FUNCT);
	
	sc->SetL(line);
	sc->SetUK(uk);

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

	tree->GetPosition();
	t1 = sc->Scaner(l);
	if (t1 != Topenblock) sc->PrintError("ќжидалось \"{\"", "");

	J();
	t1 = sc->Scaner(l);
	if (t1 != Tcloseblock) sc->PrintError("ќжидалось \"}\"", "");
	tree->SetPosition();
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
	Type type;
	TypeLex l;
	int t, uk, line;
	t = sc->Scaner(l);

	if (t == Tconst)
	{
		t = sc->Scaner(l);
		if (!((t == Tlong) || (t == Tshort)))
			sc->PrintError("ќжидалось short или long","");
		else
		{
			if (t == Tlong) type = TYPE_LONG_INTEGER;
			if (t == Tshort) type = TYPE_SHORT_INTEGER;
		}

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
		if (t == Tlong) type = TYPE_LONG_INTEGER;
		if (t == Tshort) type = TYPE_SHORT_INTEGER;
	
		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("ќжидалось int", "");

		do {
			t = sc->Scaner(l);

			if (t != Tid)
				sc->PrintError("ќжидалс€ идентификатор", "");
			else
				tree->AddId(l, type);			//занесЄм идентификатор			

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
	Type temptype;
	TypeLex l;
	int t, uk, line;

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if (t != Topenbracket)
		sc->PrintError("ќжидалось (", "");

	else
	{
		temptype = P1();
		if (temptype == TYPE_FUNCT)
			sc->PrintError("јргументом if не может быть void функци€", "");
	}

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
	Type temptype;
	TypeLex l;
	int t, uk, line;

	int membeforeid, memafterid, memafterbracket;

	line = sc->GetL();
	uk = sc->GetUK();
	
	membeforeid = sc->GetUK();
	t = sc->Scaner(l);

	if (t == Tif)
	{
		I();
	}

	else if (t == Tid)
	{
		memafterid = sc->GetUK();
		temptype = tree->SemGetType(l);
		t = sc->Scaner(l);
		if (t == Tequal)
		{
			sc->SetUK(uk);
			sc->SetL(line);
			tree->CheckDataTypes(temptype,P1());	//проверка на приведение типов
		}
		else if (t == Topenbracket)
		{
			memafterbracket = sc->GetUK();
			sc->SetUK(membeforeid);
			sc->Scaner(l);

			if (tree->SemGetType(l) != TYPE_FUNCT)
				sc->PrintError("»дентификатор не €вл€етс€ идентификатором функции", "");
			
			sc->SetUK(memafterbracket);
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


Type Diagram::P1()
{
	Type temptype;
	TypeLex l;
	int t, uk, line;

	temptype = P2();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while (t == Tor)
	{
		temptype = tree->SemGetResultType(temptype, P2());
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return temptype;
}

Type Diagram::P2()
{
	Type temptype;
	TypeLex l;
	int t, uk, line;

	temptype = P3();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while (t == Tand)
	{
		temptype = tree->SemGetResultType(temptype, P3());
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return temptype;
}


Type Diagram::P3()
{
	Type temptype;
	TypeLex l;
	int t, uk;
	int line;
	temptype = P4();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while ((t == TnotEqual) || (t == Tmore) ||
		(t == Tless) || (t == Tequalmore) || 
		(t == Tequalless) || (t == Tequal))
	{
		temptype = tree->SemGetResultType(temptype, P4());
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return temptype;
}

Type Diagram::P4()
{
	Type temptype;
	TypeLex l;
	int t, uk;
	int line;
	temptype = P5();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while ((t == Tshiftl) || (t == Tshiftr))
	{
		temptype = tree->SemGetResultType(temptype, P5());
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return temptype;
}

Type Diagram::P5()
{
	Type temptype;
	TypeLex l;
	int t, uk;
	int line;
	temptype = P6();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while ((t == Tplus) || (t == Tminus))
	{
		temptype = tree->SemGetResultType(temptype, P6());
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return temptype;
}

Type Diagram::P6()
{
	TypeLex l;
	int t, uk;
	int line;
	Type temptype;
	temptype = P7();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while ((t == Tmul) || (t == Tdiv) || (t == Tpercent))
	{
		temptype = tree->SemGetResultType(temptype, P7());
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return temptype;
}

Type Diagram::P7()
{
	TypeLex l;
	int t, uk, line;

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if ((t == Tconst10) || (t == Tconst16)) 
	{
		return TYPE_SHORT_INTEGER;
	}

	else if (t == Tid) 
	{
		return tree->SemGetType(l);
	}
	else if (t == Topenbracket)
	{
		Type temptype = P1();
			
		t = sc->Scaner(l);
		if (t != Tclosebracket) sc->PrintError("ќжидалось )", "");
		return temptype;
	}
	else sc->PrintError("ќжидалс€ идентификатор", "");
}


