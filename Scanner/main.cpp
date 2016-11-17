#include "stdafx.h" 
#include "defs.h" 
//#include "Scanner.h"
#include "Diagram.h"

#include <locale.h>
#include <stdio.h> 
#include <string.h>

int main(int argc, char * argv[]) {
	
	setlocale(LC_ALL, "");

	TScaner * sc;
	int type; TypeLex l;

	if (argc <= 1) sc = new TScaner("input.txt");// файл по умолчанию 
	else sc = new TScaner(argv[1]); // заданный файл 

									
//do {
//		type = sc->Scaner(l);
//		printf("%s - type %d \n", l, type);

	//} while (type != Tend);


	Diagram * dg = new Diagram(sc);

	dg->P();

	type = sc->Scaner(l);
	if (type == Tend)
		printf("Синтаксических ошибок не обнаружено. \n");
	else
		sc->PrintError("Лишний текст в конце программы.", "");
	system("pause");
	return 0;
}