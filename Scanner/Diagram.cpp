#include "stdafx.h"
#include "diagram.h"
#include "defs.h"

//���������
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

//�������� �������
void Diagram::F()
{
	TypeLex l;
	int t1;

	t1 = sc->Scaner(l);

	N();

	t1 = sc->Scaner(l);
	if (t1 != Topenbracket) sc->PrintError("��������� \"(\"", "");

	t1 = sc->Scaner(l);
	if (t1 != Tclosebracket) sc->PrintError("��������� \")\"", "");

	B();
}

//����
void Diagram::B()
{
	TypeLex l;
	int t1;

	t1 = sc->Scaner(l);
	if (t1 != Topenblock) sc->PrintError("��������� \"{\"", "");
	J();
	t1 = sc->Scaner(l);
	if (t1 != Tcloseblock) sc->PrintError("��������� \"}\"", "");
}

//���������
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

		if (t1 == Tend) sc->PrintError("������ {", "");
	} while (t1 != Tcloseblock);

	sc->SetUK(uk);
	sc->SetL(line);
}

//��� �������
void Diagram::N()
{
	TypeLex l;
	int t;
	t = sc->Scaner(l);
	if ((t != Tid) && (t != Tmain)) sc->PrintError("�������� ������������� ��� main", "");
}

//�������� ����������
void Diagram::O()
{
	TypeLex l;
	int t, uk, line;
	t = sc->Scaner(l);


	if (t == Tconst)
	{
		t = sc->Scaner(l);
		if (!((t == Tlong) || (t == Tshort)))
			sc->PrintError("��������� short ��� long","");

		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("��������� int", "");

		t = sc->Scaner(l);
		if (t != Tid)
			sc->PrintError("�������� �������������", "");

		t = sc->Scaner(l);
		if (t != Tequal)
			sc->PrintError("��������� =", "");

		t = sc->Scaner(l);
		if ((t != Tconst10) && (t != Tconst16))
			sc->PrintError("��������� ���������", "");
	}

	else if ((t == Tlong) || (t == Tshort))
	{

		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("��������� int", "");

		do {
			t = sc->Scaner(l);

			if (t != Tid)
				sc->PrintError("�������� �������������", "");

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
		sc->PrintError("������ � �������� ����������", "");

	if (t != Tsemicolon)
		sc->PrintError("��������� ;", "");
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
		sc->PrintError("��������� (", "");

	else P1();

	if (t != Tclosebracket)
		sc->PrintError("��������� )", "");

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

//��������
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
				sc->PrintError("��������� )", "");
		}

		else (sc->PrintError("������", ""));

		t = sc->Scaner(l);
		if (t != Tsemicolon)
			sc->PrintError("��������� ;", "");
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
			sc->PrintError("��������� ;", "");
	}

	else if (t != Tsemicolon)
	{
		sc->PrintError("��������� ;", "");
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
		sc->PrintError("������ ������", "");
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
		if (t != Tclosebracket) sc->PrintError("��������� )", "");
	}

	else sc->PrintError("�������� �������������", "");

	//��� ������� ��������� - ������� ��������� �� ������, �������� ������ ����� ������.
	//������ ��������� �� ������ ���� ������������. 
	//����������������� ��� ���� - ��������� �� ������ �����������, �� � ��� ������� ����� �������� ���������
	//��� ���� - ��������� ���������, �� ������ ������� �����



	/*
	if ((t != Tid) && (t != Tconst10) && (t != Tconst16))
	{
		if (t == Topenbracket)
		{
			P1();
		}
		else sc->PrintError("��������� (", "");

		if (t != Tclosebracket)
			sc->PrintError("��������� )", "");
	}*/
}


