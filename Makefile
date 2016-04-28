CC=clang
CXX=g++
BUILDDIR=build

all: mkbuild syntax.o lex.o
	$(CXX) -ggdb -march=native $(BUILDDIR)/lex.o $(BUILDDIR)/syntax.o -lfl -lreadline -o test

mkbuild:
	mkdir -p $(BUILDDIR)

lex.o: $(BUILDDIR)/lex.yy.c
	$(CXX) -std=c++11 -Wall -pedantic $(BUILDDIR)/lex.yy.c -I. -c -o $(BUILDDIR)/lex.o
    
syntax.o: $(BUILDDIR)/syntax.tab.cpp
	$(CXX) -std=c++11 -Wall -pedantic $(BUILDDIR)/syntax.tab.cpp -I. -c -o $(BUILDDIR)/syntax.o
    
$(BUILDDIR)/syntax.tab.cpp: syntax.ypp
	bison -d -t syntax.ypp -o $(BUILDDIR)/syntax.tab.cpp

$(BUILDDIR)/lex.yy.c: lexer.l
	flex --outfile=$(BUILDDIR)/lex.yy.c lexer.l
	
clean: $(BUILDDIR)/lex.o $(BUILDDIR)/syntax.o $(BUILDDIR)/syntax.tab.hpp $(BUILDDIR)/syntax.tab.cpp $(BUILDDIR)/lex.yy.c test
	rm -r $(BUILDDIR)
	rm test
