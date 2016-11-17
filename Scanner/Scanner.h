#ifndef __SCANER 
//#define __SCANER 
#include "defs.h"

class TScaner {
private:
	TypeMod t; // �������� ����� 
	int uk; // ��������� ������� ������� � �������� ������
	int line;

public:
	void	SetUK(int i); 
	int		GetUK(void); 
	void	SetL(int i);
	int		GetL(void);

	void	PrintError(char *, char *); 
	int		Scaner(TypeLex l); 
	void	GetData(char *); 

	TScaner(char *); 
	~TScaner() {}
};

#endif