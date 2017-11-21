CXX = g++-7.2
CXXFLAGS = 
#-O2 -std=c++14 -pedantic -Wall -Wextra -Wshadow -Wconversion -Werror

LAQ_DIR = laq-driver
ENGINE_DIR = engine
LINTER = lib/styleguide/cpplint/cpplint.py
GTEST_DIR =  lib/googletest/googletest/include

all: $(LAQ_DIR)/build \
	 $(ENGINE_DIR)/build \
	 $(LAQ_DIR)/bin/test-laq \
	 $(ENGINE_DIR)/bin/test-krao #\
	 $(ENGINE_DIR)/bin/test-io

clean:
	rm -fr $(LAQ_DIR)/build $(ENGINE_DIR)/build $(ENGINE_DIR)/src/*.pb.*

delete: clean
	rm -fr $(LAQ_DIR)/bin $(ENGINE_DIR)/bin

test:
	$(LAQ_DIR)/bin/test-laq
	$(ENGINE_DIR)/bin/test-krao

$(ENGINE_DIR)/bin/test-krao: $(ENGINE_DIR)/test/test-krao.cc \
							 $(ENGINE_DIR)/build/block.pb.o \
							 $(ENGINE_DIR)/build/krao.o
	$(CXX) -g -isystem $(GTEST_DIR) -pthread $^ libgtest.a -o $@ -I $(ENGINE_DIR) -lprotobuf

$(LAQ_DIR)/bin/test-laq: $(LAQ_DIR)/test/test-laq.cc \
						 $(LAQ_DIR)/build/laq-parser.o \
						 $(LAQ_DIR)/build/lex.yy.o \
						 $(LAQ_DIR)/build/laq-driver.o \
						 $(LAQ_DIR)/build/parsing-tree.o \
						 $(LAQ_DIR)/build/laq-statement.o
	$(CXX) -g -isystem $(GTEST_DIR) -pthread $^ libgtest.a -o $@ -I $(LAQ_DIR)

#$(ENGINE_DIR)/bin/test-io: $(ENGINE_DIR)/test/test-io.cc \
							  $(ENGINE_DIR)/build/block.o \
							  $(ENGINE_DIR)/build/matrix.o

$(LAQ_DIR)/build/laq-parser.o: $(LAQ_DIR)/build/laq-parser.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -I $(LAQ_DIR)/build -o $@

$(LAQ_DIR)/build/lex.yy.o: $(LAQ_DIR)/build/lex.yy.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@

$(LAQ_DIR)/build/laq-driver.o: $(LAQ_DIR)/src/laq-driver.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@

$(LAQ_DIR)/build/parsing-tree.o: $(LAQ_DIR)/src/parsing-tree.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@

$(LAQ_DIR)/build/laq-statement.o: $(LAQ_DIR)/src/laq-statement.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@

$(LAQ_DIR)/build/laq-parser.cc: $(LAQ_DIR)/src/laq-parser.yy
	bison -o $@ -d $<

$(LAQ_DIR)/build/lex.yy.cc: $(LAQ_DIR)/src/laq-scanner.ll
	flex -o $@ $<

$(LAQ_DIR)/build:
	mkdir -p $@ $(LAQ_DIR)/bin

$(ENGINE_DIR)/bin/engine: $(ENGINE_DIR)/build/krao.o
	$(CXX) $(CXXFLAGS) $< -o $@

$(ENGINE_DIR)/build/krao.o: $(ENGINE_DIR)/src/krao.cc
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -o $@

$(ENGINE_DIR)/build/block.pb.o: $(ENGINE_DIR)/src/block.pb.cc
	$(CXX) $(CXXFLAGS) -c $< -I . -o $@

$(ENGINE_DIR)/src/block.pb.cc: $(ENGINE_DIR)/src/block.proto
	protoc --cpp_out=. $<

$(ENGINE_DIR)/build:
	mkdir -p $@ $(ENGINE_DIR)/bin


linter: $(LINTER)
	$(LINTER) $(LAQ_DIR)/src/*.cc
	$(LINTER) $(LAQ_DIR)/*/*.h
	$(LINTER) $(LAQ_DIR)/test/*.cc
	$(LINTER) $(ENGINE_DIR)/src/*.cc
	$(LINTER) $(ENGINE_DIR)/*/*.h
	$(LINTER) $(ENGINE_DIR)/test/*.cc
	$(LINTER) queries/cpp/*.cc

.PHONY: all clean delete linter test
	
	
#a: b c
#	gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c