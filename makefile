CXX = g++-7.2
CXXFLAGS = -O2 -std=c++14 -pedantic -Wall -Wextra -Wshadow -Wconversion -Werror

LAQ_FOLDER = laq-driver
ENGINE_FOLDER = engine
LINTER = lib/styleguide/cpplint/cpplint.py
GTEST_DIR =  lib/googletest/googletest/include

all: $(LAQ_FOLDER)/bin/test-laq $(ENGINE_FOLDER)/bin/test-krao

clean:
	rm -fr $(LAQ_FOLDER)/build $(ENGINE_FOLDER)/build

delete: clean
	rm -fr $(LAQ_FOLDER)/bin $(ENGINE_FOLDER)/bin

test: $(LAQ_FOLDER)/bin/test-laq $(ENGINE_FOLDER)/bin/test-krao
	$<
	$(ENGINE_FOLDER)/bin/test-krao

$(ENGINE_FOLDER)/bin/test-krao: $(ENGINE_FOLDER)/test/test-krao.cc \
								$(ENGINE_FOLDER)/build/block.o \
								$(ENGINE_FOLDER)/build/krao.o
	$(CXX) -g -isystem $(GTEST_DIR) -pthread $^ libgtest.a -o $@ -I $(ENGINE_FOLDER)

$(LAQ_FOLDER)/bin/test-laq: $(LAQ_FOLDER)/test/test-laq.cc \
							$(LAQ_FOLDER)/build/laq-parser.o \
							$(LAQ_FOLDER)/build/lex.yy.o \
							$(LAQ_FOLDER)/build/laq-driver.o \
							$(LAQ_FOLDER)/build/parsing-tree.o \
							$(LAQ_FOLDER)/build/laq-statement.o
	$(CXX) -g -isystem $(GTEST_DIR) -pthread $^ libgtest.a -o $@ -I $(LAQ_FOLDER)

$(LAQ_FOLDER)/build/laq-parser.o: $(LAQ_FOLDER)/build/laq-parser.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_FOLDER) -I $(LAQ_FOLDER)/build -o $@

$(LAQ_FOLDER)/build/lex.yy.o: $(LAQ_FOLDER)/build/lex.yy.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_FOLDER) -o $@

$(LAQ_FOLDER)/build/laq-driver.o: $(LAQ_FOLDER)/src/laq-driver.cc $(LAQ_FOLDER)/build
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_FOLDER) -o $@

$(LAQ_FOLDER)/build/parsing-tree.o: $(LAQ_FOLDER)/src/parsing-tree.cc $(LAQ_FOLDER)/build
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_FOLDER) -o $@

$(LAQ_FOLDER)/build/laq-statement.o: $(LAQ_FOLDER)/src/laq-statement.cc $(LAQ_FOLDER)/build
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_FOLDER) -o $@

$(LAQ_FOLDER)/build/laq-parser.cc: $(LAQ_FOLDER)/src/laq-parser.yy $(LAQ_FOLDER)/build
	bison -o $@ -d $<

$(LAQ_FOLDER)/build/lex.yy.cc: $(LAQ_FOLDER)/src/laq-scanner.ll $(LAQ_FOLDER)/build
	flex -o $@ $<

$(LAQ_FOLDER)/build:
	mkdir -p $@ $(LAQ_FOLDER)/bin

$(ENGINE_FOLDER)/bin/engine: $(ENGINE_FOLDER)/build/krao.o
	$(CXX) $(CXXFLAGS) $< -o $@

$(ENGINE_FOLDER)/build/krao.o: $(ENGINE_FOLDER)/src/krao.cc $(ENGINE_FOLDER)/build
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_FOLDER) -o $@

$(ENGINE_FOLDER)/build/block.o: $(ENGINE_FOLDER)/src/block.cc $(ENGINE_FOLDER)/build
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_FOLDER) -o $@

$(ENGINE_FOLDER)/build:
	mkdir -p $@ $(ENGINE_FOLDER)/bin


linter: $(LINTER)
	$(LINTER) $(LAQ_FOLDER)/src/*.cc
	$(LINTER) $(LAQ_FOLDER)/*/*.h
	$(LINTER) $(LAQ_FOLDER)/test/*.cc
	$(LINTER) $(ENGINE_FOLDER)/src/*.cc
	$(LINTER) $(ENGINE_FOLDER)/*/*.h
	$(LINTER) $(ENGINE_FOLDER)/test/*.cc
	$(LINTER) queries/cpp/*.cc

.PHONY: all clean delete linter test laq engine
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c