#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define MAX_TEXT 10000 // максимальная длина текста 
#define MAX_LEX 100 // максимальная длина лексемы 
#define MAX_KEYW 9 // число ключевых слов

typedef char TypeLex[MAX_LEX]; 
typedef char TypeMod[MAX_TEXT];

// ключевые слова 
#define Tid 1

#define Tconst10 21
#define Tconst16 22

#define Tlong 31
#define Tshort 32
#define Tint 33
#define Tif 34
#define Tmain 35
#define Tconst 36
#define Tvoid 37
#define Treturn 38 //доб
#define Telse 39

#define Tequal 41
#define Tequalequal 42
#define TnotEqual 43
#define Tmore 44
#define Tless 45
#define Tequalmore 46
#define Tequalless 47
#define Tplus 48
#define Tminus 49
#define Tmul 50
#define Tdiv 51
#define Tpercent 52
#define Tplusplus 53
#define Tminusminus 54
#define Tshiftr 55
#define Tshiftl 56
#define Tor 57	//доб
#define Tand 58

#define Tcomma 61
#define Tsemicolon 62

#define Topenblock 71
#define Tcloseblock 72
#define Topenbracket 73
#define Tclosebracket 74

#define Tend 1001
#define Terror 1002

