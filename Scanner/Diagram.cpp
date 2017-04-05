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
		if (t1 == Tconst)
			O();
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
	int FlInt_local = -1;
	Node *newfun = NULL;

	t1 = sc->Scaner(l);

	int line = sc->GetL();
	uk = sc->GetUK();
	t1 = sc->Scaner(l);

	newfun = tree->AddId(l, TYPE_FUNCT, false);

 	sc->SetL(line);
	sc->SetUK(uk);

	N();

	if (t1 != Tmain)
	{
		//если не главная ПП, запомнили флаг 
		FlInt_local = FlInt;
		FlInt = 0;
	}


	t1 = sc->Scaner(l);
	if (t1 != Topenbracket) sc->PrintError("Ожидалось \"(\"", "");

	t1 = sc->Scaner(l);
	if (t1 != Tclosebracket) sc->PrintError("Ожидалось \")\"", "");

	//сохранили uk
	newfun->value->DataAsLongInt = sc->GetUK();

	B();

	if (FlInt_local != -1)
	{
		//если флаг был инициализирован, значит не главная ПП, восстановили флаг
		FlInt = FlInt_local;
	}
}

//блок
void Diagram::B()
{
	TypeLex l;
	int t1;

	tree->GetPosition();
	t1 = sc->Scaner(l);
	if (t1 != Topenblock) sc->PrintError("Ожидалось \"{\"", "");

	oldFlintForReturn = FlInt;

	J();
	t1 = sc->Scaner(l);
	if (t1 != Tcloseblock) sc->PrintError("Ожидалось \"}\"", "");

	FlInt = oldFlintForReturn;

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
	Node *node = new Node();
	TData* data = new TData();
	TData* data1 = new TData();
	int t, uk, line;
	t = sc->Scaner(l);

	if (t == Tconst)
	{
		t = sc->Scaner(l);
		if (!((t == Tlong) || (t == Tshort)))
			sc->PrintError("Ожидалось short или long", "");
		else
		{	//memtype
			if (t == Tlong) data->type = TYPE_LONG_INTEGER;
			if (t == Tshort) data->type = TYPE_SHORT_INTEGER;
		}

		t = sc->Scaner(l);
		if (t != Tint) 
			sc->PrintError("(F) Ожидался символ int", l);

		t = sc->Scaner(l);
		if (t != Tid) 
			sc->PrintError("(F) Ожидался идентификатор", l);
		else 
			node = tree->AddId(l, data->type, true);

		t = sc->Scaner(l);
		if (t != Tequal) sc->PrintError("(F) Ожидался символ '='", l);
		t = sc->Scaner(l);
		if (t != Tconst10 && t != Tconst16) sc->PrintError("(F) Ожидалась константа", l);
		
		data1->type = TYPE_SHORT_INTEGER;
		
		if (FlInt)
		{
			switch (data->type)
			{
			case TYPE_LONG_INTEGER:
				data->value->DataAsLongInt = stof(l); break;
				break;
			case TYPE_SHORT_INTEGER:
				data->value->DataAsShortInt = stoi(l); break;
				break;
			default:
				sc->PrintError("Невозможно присвоить", "");
				break;
			}
			tree->SetValue(data->value, node);
			cout << "Константа: " << node->name << " = ";
			if (data->type == TYPE_LONG_INTEGER) cout << data->value->DataAsLongInt << endl;
			if (data->type == TYPE_SHORT_INTEGER) cout << data->value->DataAsShortInt << endl;
		}
		t = sc->Scaner(l);
		if (t != Tsemicolon) sc->PrintError("(F) Ожидался символ ';'", l);
	}

	else if ((t == Tlong) || (t == Tshort))
	{
		if (t == Tlong) data->type = TYPE_LONG_INTEGER;
		if (t == Tshort) data->type = TYPE_SHORT_INTEGER;

		t = sc->Scaner(l);
		if (t != Tint)
			sc->PrintError("Ожидалось int", "");

		do {
			t = sc->Scaner(l);

			if (t != Tid)
				sc->PrintError("Ожидался идентификатор", "");
			else
				node = tree->AddId(l, data->type, false);	//занесём идентификатор			
												//запомним адрес в node
			line = sc->GetL();
			uk = sc->GetUK();

			t = sc->Scaner(l);

			if (t == Tequal)
			{
				data1 = P1();
				if (FlInt)
				{
					switch (data->type)
					{
					case TYPE_LONG_INTEGER:
						switch (data1->type)
						{
						case TYPE_LONG_INTEGER: data->value->DataAsLongInt = data1->value->DataAsLongInt; break;
						case TYPE_SHORT_INTEGER: data->value->DataAsLongInt = data1->value->DataAsShortInt; break;
						default:
							sc->PrintError("Невозможно присвоить", "");
							break;
						}
						break;
					case TYPE_SHORT_INTEGER:
						switch (data1->type)
						{
						case TYPE_LONG_INTEGER: data->value->DataAsShortInt = data1->value->DataAsLongInt; break;
						case TYPE_SHORT_INTEGER: data->value->DataAsShortInt = data1->value->DataAsShortInt; break;
						default:
							sc->PrintError("Невозможно присвоить", "");
							break;
						}
						break;
					default:
						sc->PrintError("Невозможно присвоить", "");
						break;
					}
					cout << "Присвоение: " << node->name << " = ";
					if (data->type == TYPE_LONG_INTEGER) cout << data->value->DataAsLongInt << endl;
					if (data->type == TYPE_SHORT_INTEGER) cout << data->value->DataAsShortInt << endl;
					tree->SetValue(data->value, node);
				}

			}
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
		TData * data = P1();
		if (LocalFlInt)
		{
			if (data->type == TYPE_SHORT_INTEGER)
				FlInt = data->value->DataAsShortInt;
			if (data->type == TYPE_LONG_INTEGER)
				FlInt = data->value->DataAsLongInt;
		}
		if (data->type == TYPE_FUNCT)
			sc->PrintError("Аргументом if не может быть void функция", "");
	}

	t = sc->Scaner(l);
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

	//TODO::
	//return
	//a();

	Type temptype;
	TypeLex l;
	int t, uk, line;
	Node *node = new Node();
	TData * data = new TData();
	TData * data1 = new TData();

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
		data->type = node->type;
		memafterid = sc->GetUK();
		temptype = tree->SemGetType(l);
		t = sc->Scaner(l);
		if (t == Tequal)
		{
			//sc->SetUK(uk);
//			sc->SetL(line);
			data1 = P1();
			if (FlInt)
			{
				switch (data->type)
				{
				case TYPE_LONG_INTEGER:
					switch (data1->type)
					{
					case TYPE_LONG_INTEGER: data->value->DataAsLongInt = data1->value->DataAsLongInt; break;
					case TYPE_SHORT_INTEGER: data->value->DataAsLongInt = data1->value->DataAsShortInt; break;
					default:
						sc->PrintError("Невозможно присвоить", "");
						break;
					}
					break;
				case TYPE_SHORT_INTEGER:
					switch (data1->type)
					{
					case TYPE_LONG_INTEGER: data->value->DataAsShortInt = data1->value->DataAsLongInt; break;
					case TYPE_SHORT_INTEGER: data->value->DataAsShortInt = data1->value->DataAsShortInt; break;
					default:
						sc->PrintError("Невозможно присвоить", "");
						break;
					}
					break;
				default:
					sc->PrintError("Невозможно присвоить", "");
					break;
				}
				cout << "Присвоение: " << node->name << " = ";
				if (data->type == TYPE_LONG_INTEGER) cout << data->value->DataAsLongInt << endl;
				if (data->type == TYPE_SHORT_INTEGER) cout << data->value->DataAsShortInt << endl;
				tree->SetValue(data->value, node);
			}
			//tree->CheckDataTypes(temptype, P1());	//проверка на приведение типов
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

			int saveUK = sc->GetUK();				//сохранить uk
			sc->SetUK(node->value->DataAsLongInt);	//uk на начало описанной функции

			B();

			sc->SetUK(saveUK);

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
		FlInt = 0;
	}

	else if (t != Tsemicolon)
	{
		sc->PrintError("Ожидалось ;", "");
	}
}


TData* Diagram::P1()
{
	TypeLex l;
	int t, uk, line;
	TData* data = new TData(), *data1 = new TData();
	Node* node = NULL;

	data = P2();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == Tand) && t != Tend)
	{
		int t_local = t;
		data1 = P2();
		data->type = tree->SemGetResultType(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				data->value->DataAsLongInt = data->value->DataAsLongInt || data1->value->DataAsLongInt;
				break;
			case TYPE_SHORT_INTEGER:
				data->value->DataAsLongInt = data->value->DataAsLongInt || data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию конъюнкции", "");
				break;
			}
			break;
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				data->value->DataAsShortInt = data->value->DataAsShortInt || data1->value->DataAsLongInt;
				break;
			case TYPE_SHORT_INTEGER:
				data->value->DataAsShortInt = data->value->DataAsShortInt || data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию конъюнкции", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию конъюнкции", "");
			break;
		}
		uk = sc->GetUK();
		line = sc->GetL();
		t = sc->Scaner(l);//нужна ли тут тонна операторов указателя на строку и сканер?
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData* Diagram::P2()
{
	TypeLex l;
	int t, uk, line;
	TData* data = new TData(), *data1 = new TData();
	Node* node = NULL;

	data = P3();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == Tand) && t != Tend)
	{
		int t_local = t;
		data1 = P3();
		data->type = tree->SemGetResultType(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				data->value->DataAsLongInt = data->value->DataAsLongInt && data1->value->DataAsLongInt;
				break;
			case TYPE_SHORT_INTEGER:
				data->value->DataAsLongInt = data->value->DataAsLongInt && data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию конъюнкции", "");
				break;
			}
			break;
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				data->value->DataAsShortInt = data->value->DataAsShortInt && data1->value->DataAsLongInt;
				break;
			case TYPE_SHORT_INTEGER:
				data->value->DataAsShortInt = data->value->DataAsShortInt && data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию конъюнкции", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию конъюнкции", "");
			break;
		}
		uk = sc->GetUK();
		line = sc->GetL();
		t = sc->Scaner(l);//нужна ли тут тонна операторов указателя на строку и сканер?
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}


TData* Diagram::P3()
{
	TypeLex l;
	int t, uk, line;
	TData* data = new TData(), *data1 = new TData();
	Node* node = NULL;

	data = P4();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == Tless || t == Tequalless || t == Tmore ||
		t == Tequalmore || t == Tequalequal || t == TnotEqual) && t != Tend)
	{
		int t_local = t;
		data1 = P4();
		data->type = tree->SemGetResultType(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tless)
					data->value->DataAsLongInt = data->value->DataAsLongInt < data1->value->DataAsLongInt;
				if (t_local == Tequalless)
					data->value->DataAsLongInt = data->value->DataAsLongInt <= data1->value->DataAsLongInt;
				if (t_local == Tmore)
					data->value->DataAsLongInt = data->value->DataAsLongInt > data1->value->DataAsLongInt;
				if (t_local == Tequalmore)
					data->value->DataAsLongInt = data->value->DataAsLongInt >= data1->value->DataAsLongInt;
				if (t_local == Tequalequal)
					data->value->DataAsLongInt = data->value->DataAsLongInt == data1->value->DataAsLongInt;
				if (t_local == TnotEqual)
					data->value->DataAsLongInt = data->value->DataAsLongInt != data1->value->DataAsLongInt;
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tless)
					data->value->DataAsLongInt = data->value->DataAsLongInt < data1->value->DataAsShortInt;
				if (t_local == Tequalless)
					data->value->DataAsLongInt = data->value->DataAsLongInt <= data1->value->DataAsShortInt;
				if (t_local == Tmore)
					data->value->DataAsLongInt = data->value->DataAsLongInt > data1->value->DataAsShortInt;
				if (t_local == Tequalmore)
					data->value->DataAsLongInt = data->value->DataAsLongInt >= data1->value->DataAsShortInt;
				if (t_local == Tequalequal)
					data->value->DataAsLongInt = data->value->DataAsLongInt == data1->value->DataAsShortInt;
				if (t_local == TnotEqual)
					data->value->DataAsLongInt = data->value->DataAsLongInt != data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сравнения", "");
				break;
			}
			break;
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
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
			default:
				sc->PrintError("Невозможно выполнить операцию сравнения", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию сравнения", "");
			break;
		}
		uk = sc->GetUK();
		line = sc->GetL();
		t = sc->Scaner(l);//нужна ли тут тонна операторов указателя на строку и сканер?
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData* Diagram::P4()
{
	TypeLex l;
	int t, uk, line;
	TData* data = new TData(), *data1 = new TData();
	Node* node = NULL;

	data = P5();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == Tshiftl || t == Tshiftr) && t != Tend)
	{
		int t_local = t;
		data1 = P5();
		data->type = tree->SemGetResultType(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt << data1->value->DataAsLongInt;
				if (t_local == Tshiftr && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt >> data1->value->DataAsLongInt;
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt << data1->value->DataAsShortInt;
				if (t_local == Tshiftr && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt >> data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/вычитания", "");
				break;
			}
			break;
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt << data1->value->DataAsLongInt;
				if (t_local == Tshiftr)
					data->value->DataAsShortInt = data->value->DataAsShortInt >> data1->value->DataAsLongInt;
				data->type = TYPE_LONG_INTEGER;
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tshiftl && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt << data1->value->DataAsShortInt;
				if (t_local == Tshiftr && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt >> data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/вычитания", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию сложения/вычитания", "");
			break;
		}
		uk = sc->GetUK();
		line = sc->GetL();
		t = sc->Scaner(l);//нужна ли тут тонна операторов указателя на строку и сканер?
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData* Diagram::P5()
{
	TypeLex l;
	int t, uk, line;
	TData* data = new TData(), *data1 = new TData();
	Node* node = NULL;

	data = P6();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == Tplus || t == Tminus) && t != Tend)
	{
		int t_local = t;
		data1 = P6();
		data->type = tree->SemGetResultType(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt + data1->value->DataAsLongInt;
				if (t_local == Tminus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt - data1->value->DataAsLongInt;
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt + data1->value->DataAsShortInt;
				if (t_local == Tminus && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt - data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/вычитания", "");
				break;
			}
			break;
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt + data1->value->DataAsLongInt;
				if (t_local == Tminus)
					data->value->DataAsShortInt = data->value->DataAsShortInt - data1->value->DataAsLongInt;
				data->type = TYPE_LONG_INTEGER;
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tplus && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt + data1->value->DataAsShortInt;
				if (t_local == Tminus && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt - data1->value->DataAsShortInt;
				break;
			default:
				sc->PrintError("Невозможно выполнить операцию сложения/вычитания", "");
				break;
			}
			break;
		default:
			sc->PrintError("Невозможно выполнить операцию сложения/вычитания", "");
			break;
		}
		uk = sc->GetUK();
		line = sc->GetL();
		t = sc->Scaner(l);//нужна ли тут тонна операторов указателя на строку и сканер?
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData* Diagram::P6()
{
	TypeLex l;
	int t, uk, line;
	TData* data = new TData(), *data1 = new TData();
	Node* node = NULL;

	data = P7();

	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	while ((t == Tmul || t == Tdiv || t == Tpercent) && t != Tend)
	{
		int t_local = t;
		data1 = P7();
		data->type = tree->SemGetResultType(data->type, data1->type);
		switch (data->type)
		{
		case TYPE_LONG_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tmul && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt * data1->value->DataAsLongInt;
				if (t_local == Tdiv && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt / data1->value->DataAsLongInt;
				if (t_local == Tpercent && FlInt)
					data->value->DataAsLongInt = data->value->DataAsLongInt % data1->value->DataAsLongInt;
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
		case TYPE_SHORT_INTEGER:
			switch (data1->type)
			{
			case TYPE_LONG_INTEGER:
				if (t_local == Tmul && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt * data1->value->DataAsLongInt;
				if (t_local == Tdiv)
					data->value->DataAsShortInt = data->value->DataAsShortInt / data1->value->DataAsLongInt;
				if (t_local == Tpercent && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt % data1->value->DataAsLongInt;
				data->type = TYPE_LONG_INTEGER;
				break;
			case TYPE_SHORT_INTEGER:
				if (t_local == Tmul && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt * data1->value->DataAsShortInt;
				if (t_local == Tdiv && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt / data1->value->DataAsShortInt;
				if (t_local == Tpercent && FlInt)
					data->value->DataAsShortInt = data->value->DataAsShortInt % data1->value->DataAsShortInt;
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
		uk = sc->GetUK();
		line = sc->GetL();
		t = sc->Scaner(l);//нужна ли тут тонна операторов указателя на строку и сканер?
	}
	sc->SetUK(uk);
	sc->SetL(line);
	return data;
}

TData* Diagram::P7()
{
	TypeLex l;
	int t, uk, line;
	TData* data = new TData();
	Node* node = NULL;


	line = sc->GetL();
	uk = sc->GetUK();
	t = sc->Scaner(l);

	if ((t == Tconst10) || (t == Tconst16))
	{
		data->type = TYPE_SHORT_INTEGER;
		data->value->DataAsShortInt = atoi(l);
	}
	else if (t == Topenbracket)
	{
		data = P1();
		t = sc->Scaner(l);
		if (t != Tclosebracket)
			sc->PrintError("(P7) Ожидался символ ')'", l);
	}
	else if (t == Tid)
	{
		node = tree->FindId(l);
		data->type = node->type;

		uk = sc->GetUK();
		line = sc->GetL();
		t = sc->Scaner(l);

		data->value->DataAsShortInt = node->value->DataAsShortInt;
		data->value->DataAsLongInt = node->value->DataAsLongInt;

		sc->SetUK(uk);
		sc->SetL(line);
	}
	else sc->PrintError("Ожидался идентификатор", "");
	return data;
}


