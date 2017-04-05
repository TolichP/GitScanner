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


void TScaner::SetUK(int i)	//������������ ���������
{
	uk = i;
}


int TScaner::GetUK(void)			// ��������� ���������
{
	return uk;
}


void TScaner::PrintError(char * err, char * a) // ������ ��������� �� ������ 
{
	if (a[0] == '\0')
		printf("������ (line: %d) : %s %s\n", line, err, a);
	else printf("������ : %s. �������� ������ %s\n", err, a);
	system("pause");
	exit(0);
}


int TScaner::Scaner(TypeLex l)
{
	//int typ;										// ��� ������� 
	int i;											// ������� ����� ������� 
	for (i = 0; i < MAX_LEX; i++) l[i] = 0;			//�������� ���� ������� 
	i = 0;											// ������� ����������� � ������� i 

start:
	while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t'))
	{
		if (t[uk] == '\n') line++;
		uk++; // ������� ���������� ��������� 
	}

	if (t[uk] == '/')
	{
		if (t[uk + 1] == '/')
		{
			// ������� �����������, ���� ���������� ����� �� �\n� 
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
				else	//���� 0�, � ����� �� ����� 16��
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
			while ((t[uk] <= '9') && (t[uk] >= '0')) // ��������� N1 
				l[i++] = t[uk++];

			return Tconst10;
		}

		else if (((t[uk] >= 'a') && (t[uk] <= 'z'))|| ((t[uk] >= 'A') && (t[uk] <= 'Z')))
		{
			l[i++] = t[uk++];
			while ((t[uk] <= '9') && (t[uk] >= '0') || // ��������� N2 
				((t[uk] >= 'a') && (t[uk] <= 'z')) || ((t[uk] >= 'A') && (t[uk] <= 'Z'))) l[i++] = t[uk++];

			//int j; // �������� �� �������� �����: 
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
			//PrintError(10); // ������ 
			return Terror;
		}
	}
} // ����� Scaner 

void TScaner::GetData(char * FileName)
{ // ���� ����� FileName, ������� �������� ����� ��������� ������ 
	char aa;
	FILE * in = fopen(FileName, "r");
	if (in == NULL)
	{
		PrintError("����������� ������� ����", "");
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
			PrintError("������� ������� ������ ��������� ������", "");
			break;
		}
	}
	t[i] = '\0'; // ����������� ���� �\0� � ����� ������ 
	fclose(in);
}

void TScaner::SetL(int i) {
	line = i;
}

int TScaner::GetL() {
	return line;
}

