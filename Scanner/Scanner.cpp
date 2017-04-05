#include "stdafx.h"
#include "string.h"
#include "defs.h" 
#include "Scanner.h"

TScaner::TScaner(char * FileName)
{
	GetData(FileName);
	SetUK(0);
	line = 1;
}

TypeLex Keyword[MAX_KEYW] = { "long", "short", "int", "if", "main", "const", "void", "return", "else"};
int IndexKeyword[MAX_KEYW] = { Tlong, Tshort, Tint, Tif, Tmain, Tconst, Tvoid, Treturn, Telse};


void TScaner::SetUK(int i)	//восстановить указатель
{
	uk = i;
}


int TScaner::GetUK(void)			// запомнить указатель
{
	return uk;
}


void TScaner::PrintError(char * err, char * a) // выдать сообщение об ошибке 
{
	if (a[0] == '\0')
		printf("ќшибка (line: %d) : %s %s\n", line, err, a);
	else printf("ќшибка : %s. Ќеверный символ %s\n", err, a);
	system("pause");
	exit(0);
}


int TScaner::Scaner(TypeLex l)
{
	//int typ;										// тип лексемы 
	int i;											// текуща€ длина лексемы 
	for (i = 0; i < MAX_LEX; i++) l[i] = 0;			//очистили поле лексемы 
	i = 0;											// лексема заполн€етс€ с позиции i 

start:
	while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t'))
	{
		if (t[uk] == '\n') line++;
		uk++; // пропуск незначащих элементов 
	}

	if (t[uk] == '/')
	{
		if (t[uk + 1] == '/')
		{
			// началс€ комментарий, надо пропустить текст до Т\nТ 
			uk = uk + 2;
			while (t[uk] != '\n') uk++;
			//line++;
			goto start;
		}
		if (t[uk + 1] == '*')
		{
			uk = uk + 2;

			while (1)
			{
				if (t[uk] == '\n')
					line++;

				if ((t[uk] == '*') && (t[uk + 1] == '/'))
				{
					uk += 2;
					break;
				}

				if (t[uk] == '\0')
					return Tend;

				uk++;
			}

		
			goto start;
		}
	}

	if (t[uk] == '\0')
	{
		l[0] = '\0';
		return Tend;
	}

	else
	{
	N0:
		//constx
		if (t[uk] == '0')
		{
			l[i++] = t[uk++];

			//const16
			if (t[uk] == 'x')
			{
				l[i++] = t[uk++];

				if ((((t[uk] <= '9') && (t[uk] >= '0')) || ((t[uk] <= 'F') && (t[uk] >= 'A'))))
				{
					while (((t[uk] <= '9') && (t[uk] >= '0')) || ((t[uk] <= 'F') && (t[uk] >= 'A')))
						l[i++] = t[uk++];
					return Tconst16;
				}
				else	//если 0х, а потом не цифры 16сс
					return Terror;
			}
			i--;
			uk--;
		}


		//const10
		if ((t[uk] <= '9') && (t[uk] >= '0'))
		{
			l[i++] = t[uk++];

			if ((t[uk] >= 'a') && (t[uk] <= 'z'))
			{

				while ((t[uk] != ' ') && (t[uk] != '\n') && (t[uk] != '\t'))
					l[i++] = t[uk++];

				return Terror;
			}
			while ((t[uk] <= '9') && (t[uk] >= '0')) // состо€ние N1 
				l[i++] = t[uk++];

			return Tconst10;
		}

		else if (((t[uk] >= 'a') && (t[uk] <= 'z'))|| ((t[uk] >= 'A') && (t[uk] <= 'Z')))
		{
			l[i++] = t[uk++];
			while ((t[uk] <= '9') && (t[uk] >= '0') || // состо€ние N2 
				((t[uk] >= 'a') && (t[uk] <= 'z')) || ((t[uk] >= 'A') && (t[uk] <= 'Z'))) l[i++] = t[uk++];

			//int j; // проверка на ключевое слово: 
			for (int j = 0; j < MAX_KEYW; j++)
			{
				if (strcmp(l, Keyword[j]) == 0) return IndexKeyword[j];
			}

			return Tid;
		}

		else if (t[uk] == '=')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++];
				return Tequalequal;
			}

			return Tequal;
		}

		else if (t[uk] == '!')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++];
				return TnotEqual;
			}
		}

		else if (t[uk] == '>')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++];
				return Tequalmore;
			}

			if (t[uk] == '>')
			{
				l[i++] = t[uk++];
				return Tshiftr;
			}
			return Tmore;
		}

		else if (t[uk] == '<')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=')
			{
				l[i++] = t[uk++];
				return Tequalless;
			}

			if (t[uk] == '<')
			{
				l[i++] = t[uk++];
				return Tshiftl;
			}
			return Tless;
		}


		else if (t[uk] == '+')
		{
			l[i++] = t[uk++];
			if (t[uk] == '+')
			{
				l[i++] = t[uk++];
				return Tplusplus;
			}
			return Tplus;
		}

		else if (t[uk] == '-')
		{
			l[i++] = t[uk++];
			if (t[uk] == '-')
			{
				l[i++] = t[uk++];
				return Tminusminus;
			}
			return Tminus;
		}

		else if (t[uk] == '&')
		{
			l[i++] = t[uk++];
			if (t[uk] == '&')
			{
				l[i++] = t[uk++];
				return Tand;
			}
			return Tand;
		}

		else if (t[uk] == '|')
		{
			l[i++] = t[uk++];
			if (t[uk] == '|')
			{
				l[i++] = t[uk++];
				return Tor;
			}
			return Tor;
		}


		else if (t[uk] == '*')
		{
			l[i++] = t[uk++];
			return Tmul;
		}

		else if (t[uk] == '/')
		{
			l[i++] = t[uk++];
			return Tdiv;
		}

		else if (t[uk] == '%')
		{
			l[i++] = t[uk++];
			return Tpercent;
		}

		else if (t[uk] == ',')
		{
			l[i++] = t[uk++];
			return Tcomma;
		}

		else if (t[uk] == ';')
		{
			l[i++] = t[uk++];
			return Tsemicolon;
		}

		else if (t[uk] == '{')
		{
			l[i++] = t[uk++];
			return Topenblock;
		}

		else if (t[uk] == '}')
		{
			l[i++] = t[uk++];
			return Tcloseblock;
		}

		else if (t[uk] == ')')
		{
			l[i++] = t[uk++];
			return Tclosebracket;
		}

		else if (t[uk] == '(')
		{
			l[i++] = t[uk++];
			return Topenbracket;
		}

		else {
			//PrintError(10); // ошибка 
			return Terror;
		}
	}
} // конец Scaner 

void TScaner::GetData(char * FileName)
{ // ввод файла FileName, который содержит текст исходного модул€ 
	char aa;
	FILE * in = fopen(FileName, "r");
	if (in == NULL)
	{
		PrintError("ќтсутствует входной файл", "");
		exit(1);
	}
	int i = 0;
	while (!feof(in))
	{
		fscanf(in, "%c", &aa);
		if (!feof(in))
			t[i++] = aa;
		if (i >= MAX_TEXT - 1)
		{
			PrintError("—лишком большой размер исходного модул€", "");
			break;
		}
	}
	t[i] = '\0'; // приписываем знак Т\0Т в конец текста 
	fclose(in);
}

void TScaner::SetL(int i) {
	line = i;
}

int TScaner::GetL() {
	return line;
}

