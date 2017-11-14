CXX = g++-7.2

CXXFLAGS = -O2 -std=c++14 -pedantic -Wall -Wextra -Wshadow -Wconversion -Werror


LAQ_FOLDER = laq-driver
ENGINE_FOLDER = engine
LINTER = lib/styleguide/cpplint/cpplint.py

all: laq

clean:
	rm -fr $(LAQ_FOLDER)/build

delete: clean
	rm -fr $(LAQ_FOLDER)/bin

laq: dir bison flex
	$(CXX) $(CXXFLAGS) \
		$(LAQ_FOLDER)/build/laq-parser.cc \
		$(LAQ_FOLDER)/build/lex.yy.cc \
		$(LAQ_FOLDER)/src/laq-statement.cc \
		$(LAQ_FOLDER)/src/parsing-tree.cc \
		$(LAQ_FOLDER)/src/laq-driver.cc \
		$(LAQ_FOLDER)/test/laq.cc \
		-I $(LAQ_FOLDER) \
		-o $(LAQ_FOLDER)/bin/$@ -lfl

dir:
	mkdir -p $(LAQ_FOLDER)/build $(LAQ_FOLDER)/bin

bison: $(LAQ_FOLDER)/src/laq-parser.yy
	bison -o $(LAQ_FOLDER)/build/laq-parser.cc -d $<

flex: $(LAQ_FOLDER)/src/laq-scanner.ll
	flex -o $(LAQ_FOLDER)/build/lex.yy.cc $<

linter:
	$(LINTER) $(LAQ_FOLDER)/src/*.cc
	$(LINTER) $(LAQ_FOLDER)/*/*.h
	$(LINTER) $(LAQ_FOLDER)/test/*.cc
	$(LINTER) $(ENGINE_FOLDER)/src/*.cc
	$(LINTER) $(ENGINE_FOLDER)/*/*.h

test: $(LAQ_FOLDER)/bin/laq
	$(LAQ_FOLDER)/bin/laq $(LAQ_FOLDER)/test/Q14.laq

.PHONY: all clean
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c