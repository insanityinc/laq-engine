#include "laq.tab.h"
#include <iostream>

extern FILE *yyin;
FILE *fp;

int main(int argc, char* argv[]) {

	++argv; --argc;
	
	if (argc == 2) {
		yyin = fopen(argv[0], "r");
		fp = fopen(argv[1], "w");
	}
	else if (argc == 1 ) {
		yyin = fopen(argv[0], "r");
		fp = stdout;
	}
	else {
		yyin = stdin;
		fp = stdout;
	}
	
	yyparse();

	return 0;
}

