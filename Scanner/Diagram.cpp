#include "stdafx.h"
#include "diagram.h"
#include "defs.h"
#include <iostream>

using namespace std;

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
	if (t1 != Topenbracket) sc->PrintError("Ожидалось \"(\"", "");

	t1 = sc->Scaner(l);
	if (t1 != Tclosebracket) sc->PrintError("Ожидалось \")\"", "");

	B();

}

//блок
void Diagram::B()
{
	TypeLex l;
	int t1;

	tree->GetPosition();
	t1 = sc->Scaner(l);
	if (t1 != Topenblock) sc->PrintError("Ожидалось \"{\"", "");

	J();
	t1 = sc->Scaner(l);
	if (t1 != Tcloseblock) sc->PrintError("Ожидалось \"}\"", "");
	tree->SetPosition();
}

//операторы
void Diagram::J()
{
	TypeLex l;
	int t1, uk;
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

		if (t1 == Tend) sc->PrintError("Лишняя {", "");
	} while (t1 != Tcloseblock);

	sc->SetUK(uk);
	sc->SetL(line);
}

//имя функции
void Diagram::N()
{
	TypeLex l;
	int t;
	t = sc->Scaner(l);
	if ((t != Tid) && (t != Tmain)) sc->PrintError("Ожидался идентификатор или main", "");
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
			sc->PrintError("Ожидалось short или long", "");
		else
		{
			if (t == Tlong) type = TYPE_LONG_INTEGER;
			if (t == Tshort) type = TYPE_SHORT_INTEGER;
		}

		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("Ожидалось int", "");

		t = sc->Scaner(l);
		if (t != Tid)
			sc->PrintError("Ожидался идентификатор", "");

		t = sc->Scaner(l);
		if (t != Tequal)
			sc->PrintError("Ожидалось =", "");

		t = sc->Scaner(l);
		if ((t != Tconst10) && (t != Tconst16))
			sc->PrintError("Ожидалась константа", "");
	}

	else if ((t == Tlong) || (t == Tshort))
	{
		if (t == Tlong) type = TYPE_LONG_INTEGER;
		if (t == Tshort) type = TYPE_SHORT_INTEGER;

		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("Ожидалось int", "");

		do {
			t = sc->Scaner(l);

			if (t != Tid)
				sc->PrintError("Ожидался идентификатор", "");
			else
				tree->AddId(l, type);			//занесём идентификатор			

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
		sc->PrintError("Ошибка в описании переменных", "");

	if (t != Tsemicolon)
		sc->PrintError("Ожидалось ;", "");
}

//if
void Diagram::I()
{
	Type temptype;
	TypeLex l;
	int t, uk, line;
	int LocalFlInt = FlInt;

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if (t != Topenbracket)
		sc->PrintError("Ожидалось (", "");
	else
	{
		TData* data = P1();
		if (temptype == TYPE_FUNCT)
			sc->PrintError("Аргументом if не может быть void функция", "");

		if (LocalFlInt)
		{
			if (data->type == TYPE_SHORT_INTEGER)
				FlInt = data->value->DataAsShortInt;
			if (data->type == TYPE_LONG_INTEGER)
				FlInt = data->value->DataAsLongInt;
		}
	}

	if (t != Tclosebracket)
		sc->PrintError("Ожидалось )", "");

	L();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	if (LocalFlInt) FlInt = !FlInt;
	if (t == Telse)
	{
		L();
	}
	else
	{
		sc->SetUK(uk);
		sc->SetL(line);
	}
	FlInt = LocalFlInt;
}

//оператор
void Diagram::L()
{
	Type temptype;
	TypeLex l;
	int t, uk, line;
	TData* data = new TData(), *data1 = new TData();
	Node* node = NULL;

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
		node = tree->FindId(l);

		memafterid = sc->GetUK();
		temptype = tree->SemGetType(l);
		t = sc->Scaner(l);
		if (t == Tequal)
		{
			sc->SetUK(uk);
			sc->SetL(line);
			data1 = P1();
			tree->CheckDataTypes(temptype, data1->type);	//проверка на приведение типов
			if (FlInt)
			{
				switch (data->type)
				{
				case TYPE_LONG_INTEGER:
					switch (data1->type)
					{
					case TYPE_SHORT_INTEGER: data->value->DataAsLongInt = data1->value->DataAsShortInt; break;
					case TYPE_LONG_INTEGER: data->value->DataAsLongInt = data1->value->DataAsLongInt; break;
					default:
						sc->PrintError("Невозможно присвоить", "");
						break;
					}
					break;
				case TYPE_SHORT_INTEGER:
					switch (data1->type)
					{
					case TYPE_SHORT_INTEGER: data->value->DataAsShortInt = data1->value->DataAsShortInt; break;
					case TYPE_LONG_INTEGER: data->value->DataAsShortInt = data1->value->DataAsLongInt; break;
					default:
						sc->PrintError("Невозможно присвоить", "");
						break;
					}
					break;
				default:
					sc->PrintError("Невозможно присвоить", "");
					break;
				}
				cout << "Присвоение: " << l << " = ";
				if (data->type == TYPE_LONG_INTEGER) cout << data->value->DataAsLongInt << endl;
				if (data->type == TYPE_SHORT_INTEGER) cout << data->value->DataAsShortInt << endl;
				tree->SetValue(data->value, node);
			}
		}
		else if (t == Topenbracket)
		{
			memafterbracket = sc->GetUK();
			sc->SetUK(membeforeid);
			sc->Scaner(l);

			if (tree->SemGetType(l) != TYPE_FUNCT)
				sc->PrintError("Идентификатор не является идентификатором функции", "");

			sc->SetUK(memafterbracket);
			t = sc->Scaner(l);
			if (t != Tclosebracket)
				sc->PrintError("Ожидалось )", "");
		}

		else (sc->PrintError("Ошибка", ""));

		t = sc->Scaner(l);
		if (t != Tsemicolon)
			sc->PrintError("Ожидалось ;", "");
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
			sc->PrintError("Ожидалось ;", "");
	}

	else if (t != Tsemicolon)
	{
		sc->PrintError("Ожидалось ;", "");
	}
}


TData * Diagram::P1()
{
	TData* data = new TData(), *data1 = new TData();
	TypeLex l;
	int t, uk;
	int line;

	data = P2();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while (t == Tand)
	{
		int t_local = t;
		data1 = P2();
		data->type = tree->CheckDataTypes(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				data->value->DataAsShortInt = data->value->DataAsShortInt || data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				data->value->DataAsShortInt = data->value->DataAsShortInt || data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				data->value->DataAsLongInt = data->value->DataAsLongInt || data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				data->value->DataAsLongInt = data->value->DataAsLongInt || data1->value->DataAsLongInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
			break;
		}
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData * Diagram::P2()
{
	TData* data = new TData(), *data1 = new TData();
	TypeLex l;
	int t, uk;
	int line;

	data = P3();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while (t == Tand)
	{
		int t_local = t;
		data1 = P3();
		data->type = tree->CheckDataTypes(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
					data->value->DataAsShortInt = data->value->DataAsShortInt && data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
					data->value->DataAsShortInt = data->value->DataAsShortInt && data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
					data->value->DataAsLongInt = data->value->DataAsLongInt && data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
					data->value->DataAsLongInt = data->value->DataAsLongInt && data1->value->DataAsLongInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
			break;
		}
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}


TData * Diagram::P3()
{
	TData* data = new TData(), *data1 = new TData();
	TypeLex l;
	int t, uk;
	int line;

	data = P5();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == TnotEqual) || (t == Tmore) ||
		(t == Tless) || (t == Tequalmore) ||
		(t == Tequalless) || (t == Tequal))
	{
		int t_local = t;
		data1 = P4();
		data->type = tree->CheckDataTypes(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				if (t_local == Tless && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt < data1->value->DataAsShortInt;
				if (t_local == Tequalless && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt <= data1->value->DataAsShortInt;
				if (t_local == Tmore && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt > data1->value->DataAsShortInt;
				if (t_local == Tequalmore && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt >= data1->value->DataAsShortInt;
				if (t_local == Tequalequal && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt == data1->value->DataAsShortInt;
				if (t_local == TnotEqual && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt != data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				if (t_local == Tless && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt < data1->value->DataAsLongInt;
				if (t_local == Tequalless && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt <= data1->value->DataAsLongInt;
				if (t_local == Tmore && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt > data1->value->DataAsLongInt;
				if (t_local == Tequalmore && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt >= data1->value->DataAsLongInt;
				if (t_local == Tequalequal && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt == data1->value->DataAsLongInt;
				if (t_local == TnotEqual && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt != data1->value->DataAsLongInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				if (t_local == Tless && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt < data1->value->DataAsShortInt;
				if (t_local == Tequalless && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt <= data1->value->DataAsShortInt;
				if (t_local == Tmore && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt > data1->value->DataAsShortInt;
				if (t_local == Tequalmore && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt >= data1->value->DataAsShortInt;
				if (t_local == Tequalequal && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt == data1->value->DataAsShortInt;
				if (t_local == TnotEqual && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt != data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				if (t_local == Tless && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt < data1->value->DataAsLongInt;
				if (t_local == Tequalless && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt <= data1->value->DataAsLongInt;
				if (t_local == Tmore && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt > data1->value->DataAsLongInt;
				if (t_local == Tequalmore && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt >= data1->value->DataAsLongInt;
				if (t_local == Tequalequal && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt == data1->value->DataAsLongInt;
				if (t_local == TnotEqual && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt != data1->value->DataAsLongInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			//data = tree->SemGetResultType(temptype, P5());
			line = sc->GetL();
			uk = sc->GetUK();
			t = sc->Scaner(l);
		}
		sc->SetUK(uk);
		sc->SetL(line);
		return data;
	}
}

TData * Diagram::P4()
{
	TData* data = new TData(), *data1 = new TData();
	TypeLex l;
	int t, uk;
	int line;

	data = P5();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while ((t == Tshiftl) || (t == Tshiftr))
	{
		int t_local = t;
		data1 = P5();
		data->type = tree->CheckDataTypes(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt << data1->value->DataAsShortInt;
				if (t_local == Tshiftr && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt >> data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt << data1->value->DataAsShortInt;
				if (t_local == Tshiftr && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt >> data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt << data1->value->DataAsShortInt;
				if (t_local == Tshiftr && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt >> data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt << data1->value->DataAsLongInt;
				if (t_local == Tshiftr && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt >> data1->value->DataAsLongInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
			break;
		}
		//data = tree->SemGetResultType(temptype, P5());
		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData * Diagram::P5()
{
	//Type temptype;
	TData* data = new TData(), *data1 = new TData();
	TypeLex l;
	int t, uk;
	int line;

	data = P6();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == Tplus) || (t == Tminus))
	{
		int t_local = t;
		data1 = P6();
		data->type = tree->CheckDataTypes(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt + data1->value->DataAsShortInt;
				if (t_local == Tminus && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt - data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt + data1->value->DataAsShortInt;
				if (t_local == Tminus && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt - data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_SHORT_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt + data1->value->DataAsShortInt;
				if (t_local == Tminus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt - data1->value->DataAsShortInt;
				break;
			case TYPE_LONG_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt + data1->value->DataAsLongInt;
				if (t_local == Tminus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt - data1->value->DataAsLongInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию сложения/разности", "");
			break;
		}

		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData * Diagram::P6()
{
	TypeLex l;
	int t, uk;
	int line;

	TData* data = new TData(), *data1 = new TData();
	data = P7();
	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);
	while ((t == Tmul) || (t == Tdiv) || (t == Tpercent))
	{
		int t_local = t;
		data1 = P7();
		data->type = tree->CheckDataTypes(data->type, data1->type);

		switch (data->type)
		{
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tmul && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt * data1->value->DataAsLongInt;
				if (t_local == Tdiv)
					data->value->DataAsShortInt = data->value->DataAsShortInt / data1->value->DataAsLongInt;
				if (t_local == Tpercent && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt % (data1->value->DataAsLongInt);
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tmul && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt * data1->value->DataAsLongInt;
				if (t_local == Tdiv && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt / data1->value->DataAsLongInt;
				if (t_local == Tpercent && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt % data1->value->DataAsLongInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию умножения/деления/остатка", "");
				break;
			}
			break;
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tmul && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt * data1->value->DataAsLongInt;
				if (t_local == Tdiv)
					data->value->DataAsLongInt = data->value->DataAsLongInt / data1->value->DataAsLongInt;
				if (t_local == Tpercent && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt % (data1->value->DataAsLongInt);
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tmul && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt * data1->value->DataAsShortInt;
				if (t_local == Tdiv && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt / data1->value->DataAsShortInt;
				if (t_local == Tpercent && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt % data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию умножения/деления/остатка", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию умножения/деления/остатка", "");
			break;
		}

		line = sc->GetL();
		uk = sc->GetUK();
		t = sc->Scaner(l);
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData * Diagram::P7()
{
	TypeLex l;
	TData* data = new TData();
	Node* node = NULL;
	int t, uk1, line1;

	line1 = sc->GetL();
	uk1 = sc->GetUK();
	t = sc->Scaner(l);

	if ((t == Tconst10) || (t == Tconst16))
	{
		data->type = TYPE_LONG_INTEGER;
		data->value->DataAsLongInt = atoi(l);
		return data;
	}
	else if (t == Topenbracket)
	{
		data = P1();
		t = sc->Scaner(l);
		if (t != Tclosebracket) sc->PrintError("(V) Ожидался символ ')'", l);
	}
	else if (t == Tid)
	{
		node = tree->FindId(l);
		data->type = node->type;
		uk1 = sc->GetUK();
		line1 = sc->GetL();
		t = sc->Scaner(l);

		data->type = node->type;
		data->value->DataAsShortInt = node->value->DataAsShortInt;
		data->value->DataAsLongInt = node->value->DataAsLongInt;
		sc->SetUK(uk1);
		sc->SetL(line1);
	}

	else sc->PrintError("Ожидался идентификатор", "");
	return data;
}