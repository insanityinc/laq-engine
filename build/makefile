CXX = g++-7.2
CXXFLAGS =
#-std=c++14 -O2 -pedantic-errors -Wall -Wextra -Wpedantic 
#-Werror
LDFLAGS =

BUILD_FOLDER = dbms
LAQ_PARSER_FOLDER = $(BUILD_FOLDER)/la-parser

LAQ_PARSER_SRC = ../dbms/engine/src/new

all: laq-parser

clean:
	rm -fr	$(BUILD_FOLDER)
	rm -f	$(LAQ_PARSER_SRC)/laq-parser.cc \
		$(LAQ_PARSER_SRC)/laq-parser.hh \
		$(LAQ_PARSER_SRC)/lex.yy.c \
		$(LAQ_PARSER_SRC)/location.hh \
		$(LAQ_PARSER_SRC)/position.hh \
		$(LAQ_PARSER_SRC)/stack.hh

laq-parser: dir laq-bison laq-flex
	$(CXX) $(CXXFLAGS) $(LAQ_PARSER_SRC)/laq-parser.cc $(LAQ_PARSER_SRC)/lex.yy.c $(LAQ_PARSER_SRC)/laq-driver.cc $(LAQ_PARSER_SRC)/laq.cc -o $@ -lfl $(LDFLAGS)

dir:
	mkdir -p $(LAQ_PARSER_FOLDER)

laq-bison: $(LAQ_PARSER_SRC)/laq-parser.yy
	bison -o $(LAQ_PARSER_SRC)/laq-parser.cc -d $<

laq-flex: $(LAQ_PARSER_SRC)/laq-scanner.ll
	flex -o $(LAQ_PARSER_SRC)/lex.yy.c $<


.PHONY: all clean
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c 