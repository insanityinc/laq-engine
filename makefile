CXX = g++-7.2

CXXFLAGS = -O2 -std=c++14 -pedantic -Wall -Wextra -Wshadow -Wconversion -Werror


LAS_FOLDER = las-interface
LINTER = lib/cpplint/cpplint.py

all: las

clean:
	rm -fr $(LAS_FOLDER)/build

delete: clean
	rm -fr $(LAS_FOLDER)/bin

las: dir las-bison las-flex
	$(CXX) $(CXXFLAGS) \
		$(LAS_FOLDER)/build/las-parser.cc \
		$(LAS_FOLDER)/build/lex.yy.cc \
		$(LAS_FOLDER)/src/las-driver.cc \
		$(LAS_FOLDER)/test/las.cc \
		-I $(LAS_FOLDER)/include \
		-I $(LAS_FOLDER)/build \
		-o $(LAS_FOLDER)/bin/$@ -lfl

dir:
	mkdir -p $(LAS_FOLDER)/build $(LAS_FOLDER)/bin

las-bison: $(LAS_FOLDER)/src/las-parser.yy
	bison -o $(LAS_FOLDER)/build/las-parser.cc -d $<

las-flex: $(LAS_FOLDER)/src/las-scanner.ll
	flex -o $(LAS_FOLDER)/build/lex.yy.cc $<

linter:
	$(LINTER) $(LAS_FOLDER)/src/las-driver.cc
	$(LINTER) $(LAS_FOLDER)/include/las-driver.hh

.PHONY: all clean
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c 