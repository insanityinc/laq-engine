CXX = g++
CXXFLAGS = -O2 -std=c++11

all: build bin/test_laq

clean:
	rm -fr build bin

build:
	mkdir -p $@ bin

bin/test_laq: test/test_sql.cpp \
			  build/sql_parser.o \
			  build/lex.yy.o \
			  build/sql_driver.o \
			  ../libgtest.a
	$(CXX) $(CXXFLAGS) -isystem $(GTEST_DIR) -pthread $^ -o $@ -I $(LAQ_DIR) -I $(ENGINE_DIR)

build/sql_parser.o: build/sql_parser.cpp
	$(CXX) $(CXXFLAGS) -c $< -I . -Ibuild -o $@

build/lex.yy.o: build/lex.yy.cpp
	$(CXX) $(CXXFLAGS) -c $< -I. -o $@

build/sql_driver.o: src/sql_driver.cpp
	$(CXX) $(CXXFLAGS) -c $< -I. -o $@

build/sql_parser.cpp: src/sql_parser.yy
	bison -o $@ -d $<

build/lex.yy.cpp: src/sql_scanner.ll
	flex -o $@ $<
