# Compiladores
CPPC=g++
YACC=bison

# Dependências
all: parser

parser: parser.tab.c
	$(CPPC) parser.tab.c -std=c++17 -o parser

parser.tab.c: parser.y
	$(YACC) parser.y

clean:
	rm parser parser.tab.c