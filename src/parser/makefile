FLEX = flex
BISON = bison

all: scanner.cpp parser.cpp

scanner.cpp: sp.l
	$(FLEX) sp.l

parser.cpp: sp.y
	$(BISON) -v -d -t sp.y
