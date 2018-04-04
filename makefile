CXX = g++
CXXFLAGS = -O2 -std=c++11 -Wall -Wextra -Wshadow -Wconversion -pedantic -Werror
#-Wno-unused-parameter

DBGEN_DATA = -D data/dbgen
LA_DATA = -D data/la

LAQ_DIR = laq_driver
ENGINE_DIR = engine
LINTER = lib/styleguide/cpplint/cpplint.py
GTEST_DIR = lib/googletest/googletest/include
CEREAL = lib/cereal/include

ENGINE_OBJ = $(ENGINE_DIR)/build/block.o \
			 $(ENGINE_DIR)/build/dot.o \
			 $(ENGINE_DIR)/build/filter.o \
			 $(ENGINE_DIR)/build/fold.o \
			 $(ENGINE_DIR)/build/krao.o \
			 $(ENGINE_DIR)/build/lift.o \
			 $(ENGINE_DIR)/build/matrix.o \
			 $(ENGINE_DIR)/build/database.o

.PHONY: all clean delete linter test count

all: $(ENGINE_DIR)/build $(ENGINE_DIR)/bin/q6 $(ENGINE_DIR)/bin/load

#all: $(LAQ_DIR)/build \
	 $(ENGINE_DIR)/build \
	 $(LAQ_DIR)/bin/test_laq \
	 $(ENGINE_DIR)/bin/q6

clean:
	rm -fr $(LAQ_DIR)/build $(ENGINE_DIR)/build

delete: clean
	rm -fr $(LAQ_DIR)/bin $(ENGINE_DIR)/bin data/la/*

linter: $(LINTER)
	@$< --extensions=hpp,cpp \
		$(LAQ_DIR)/*/*.cpp \
		$(LAQ_DIR)/*/*.hpp \
		$(ENGINE_DIR)/*/*.cpp \
		$(ENGINE_DIR)/*/*.hpp \
		queries/cpp/*.cpp \
		data/*.cpp

test:
	$(LAQ_DIR)/bin/test_laq

count:
	cloc engine laq_driver sql_driver data queries makefile


$(LAQ_DIR)/bin/test_laq: $(LAQ_DIR)/test/test_laq.cpp \
						 $(LAQ_DIR)/build/laq_parser.o \
						 $(LAQ_DIR)/build/lex.yy.o \
						 $(LAQ_DIR)/build/laq_driver.o \
						 $(LAQ_DIR)/build/parsing_tree.o \
						 $(LAQ_DIR)/build/laq_statement.o \
						 libgtest.a
	$(CXX) $(CXXFLAGS) -isystem $(GTEST_DIR) -pthread $^ -o $@ -I $(LAQ_DIR)



$(ENGINE_DIR)/bin/q6: queries/cpp/6.cpp $(ENGINE_OBJ)
	$(CXX) $(CXXFLAGS) -I $(ENGINE_DIR) -o $@ $^

$(ENGINE_DIR)/bin/load: data/tpch_createDB.cpp $(ENGINE_OBJ)
	$(CXX) $(CXXFLAGS) -I $(ENGINE_DIR) -o $@ $^


########## Needed directories ##########

$(LAQ_DIR)/build:
	mkdir -p $@ $(LAQ_DIR)/bin

$(ENGINE_DIR)/build:
	mkdir -p $@ $(ENGINE_DIR)/bin


########## LAQ parser objects ##########

$(LAQ_DIR)/build/laq_parser.o: $(LAQ_DIR)/build/laq_parser.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -I $(LAQ_DIR)/build -o $@

$(LAQ_DIR)/build/lex.yy.o: $(LAQ_DIR)/build/lex.yy.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@

$(LAQ_DIR)/build/laq_driver.o: $(LAQ_DIR)/src/laq_driver.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@

$(LAQ_DIR)/build/parsing_tree.o: $(LAQ_DIR)/src/parsing_tree.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@

$(LAQ_DIR)/build/laq_statement.o: $(LAQ_DIR)/src/laq_statement.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(LAQ_DIR) -o $@


########## LAQ parser flex and bison ##########

$(LAQ_DIR)/build/laq_parser.cpp: $(LAQ_DIR)/src/laq_parser.yy
	bison -o $@ -d $<

$(LAQ_DIR)/build/lex.yy.cpp: $(LAQ_DIR)/src/laq_scanner.ll
	flex -o $@ $<


########## Engine objects ##########

$(ENGINE_DIR)/build/block.o: $(ENGINE_DIR)/src/block.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@

$(ENGINE_DIR)/build/dot.o: $(ENGINE_DIR)/src/dot.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@

$(ENGINE_DIR)/build/filter.o: $(ENGINE_DIR)/src/filter.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@

$(ENGINE_DIR)/build/fold.o: $(ENGINE_DIR)/src/fold.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@

$(ENGINE_DIR)/build/krao.o: $(ENGINE_DIR)/src/krao.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@

$(ENGINE_DIR)/build/lift.o: $(ENGINE_DIR)/src/lift.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@

$(ENGINE_DIR)/build/matrix.o: $(ENGINE_DIR)/src/matrix.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@

$(ENGINE_DIR)/build/database.o: $(ENGINE_DIR)/src/database.cpp
	$(CXX) $(CXXFLAGS) -c $< -I $(ENGINE_DIR) -I $(CEREAL) -o $@


#a: b c
#   gcc -o $@ $^
#$@ = a
#$< = b
#$^ = b c
