CXX = g++-7.2

CXXFLAGS = -O2 -std=c++14 -pedantic -Wall -Wextra -Wshadow -Wconversion -Werror


LAQ_FOLDER = laq-interface
LINTER = lib/cpplint/cpplint.py

all: laq

clean:
	rm -fr $(LAQ_FOLDER)/build

delete: clean
	rm -fr $(LAQ_FOLDER)/bin

laq: dir laq-bison laq-flex
	$(CXX) $(CXXFLAGS) \
		$(LAQ_FOLDER)/build/laq-parser.cc \
		$(LAQ_FOLDER)/build/lex.yy.cc \
		$(LAQ_FOLDER)/src/laq-driver.cc \
		$(LAQ_FOLDER)/test/laq.cc \
		-I $(LAQ_FOLDER)/include \
		-I $(LAQ_FOLDER)/build \
		-o $(LAQ_FOLDER)/bin/$@ -lfl

dir:
	mkdir -p $(LAQ_FOLDER)/build $(LAQ_FOLDER)/bin

laq-bison: $(LAQ_FOLDER)/src/laq-parser.yy
	bison -o $(LAQ_FOLDER)/build/laq-parser.cc -d $<

laq-flex: $(LAQ_FOLDER)/src/laq-scanner.ll
	flex -o $(LAQ_FOLDER)/build/lex.yy.cc $<

linter:
	$(LINTER) $(LAQ_FOLDER)/src/laq-driver.cc
	$(LINTER) $(LAQ_FOLDER)/include/laq-driver.hh

test: $(LAQ_FOLDER)/bin/laq
	$(LAQ_FOLDER)/bin/laq $(LAQ_FOLDER)/test/Q14.laq

.PHONY: all clean
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c 