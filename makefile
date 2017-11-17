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

laq: $(LAQ_FOLDER)/build $(LAQ_FOLDER)/build/laq-parser.cc $(LAQ_FOLDER)/build/lex.yy.cc
	$(CXX) $(CXXFLAGS) \
		$(LAQ_FOLDER)/build/laq-parser.cc \
		$(LAQ_FOLDER)/build/lex.yy.cc \
		$(LAQ_FOLDER)/src/laq-statement.cc \
		$(LAQ_FOLDER)/src/parsing-tree.cc \
		$(LAQ_FOLDER)/src/laq-driver.cc \
		$(LAQ_FOLDER)/test/laq.cc \
		-I $(LAQ_FOLDER) \
		-o $(LAQ_FOLDER)/bin/$@ -lfl

$(LAQ_FOLDER)/build/laq-parser.cc: $(LAQ_FOLDER)/src/laq-parser.yy $(LAQ_FOLDER)/build
	bison -o $@ -d $<

$(LAQ_FOLDER)/build/lex.yy.cc: $(LAQ_FOLDER)/src/laq-scanner.ll $(LAQ_FOLDER)/build
	flex -o $@ $<

$(LAQ_FOLDER)/build:
	mkdir -p $@ $(LAQ_FOLDER)/bin

linter:
	$(LINTER) $(LAQ_FOLDER)/src/*.cc
	$(LINTER) $(LAQ_FOLDER)/*/*.h
	$(LINTER) $(LAQ_FOLDER)/test/*.cc
	$(LINTER) $(ENGINE_FOLDER)/src/*.cc
	$(LINTER) $(ENGINE_FOLDER)/*/*.h

test: $(LAQ_FOLDER)/bin/laq
	@echo "\n========== Parsing TPC-H Query 06 ==========\n"
	$(LAQ_FOLDER)/bin/laq queries/laq/6.laq
	@echo "\n========== Parsing TPC-H Query 14 ==========\n"
	$(LAQ_FOLDER)/bin/laq queries/laq/14.laq

.PHONY: all clean
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c