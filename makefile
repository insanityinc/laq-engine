CXX = g++-7.2
CXXFLAGS = -std=c++14 -O2
#-pedantic-errors -Wall -Wextra -Wpedantic 
#-Werror
LDFLAGS =

LAS_FOLDER = las-interface

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
		-o $(LAS_FOLDER)/bin/$@ -lfl $(LDFLAGS)

dir:
	mkdir -p $(LAS_FOLDER)/build $(LAS_FOLDER)/bin

las-bison: $(LAS_FOLDER)/src/las-parser.yy
	bison -o $(LAS_FOLDER)/build/las-parser.cc -d $<

las-flex: $(LAS_FOLDER)/src/las-scanner.ll
	flex -o $(LAS_FOLDER)/build/lex.yy.cc $<


.PHONY: all clean
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c 