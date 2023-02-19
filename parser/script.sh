#!/bin/bash

gcc -o analex analisadorLexico.c

./analex code.txt

bison parser.y

g++ parser.tab.c -std=c++17 -o parser

./parser docLex.txt

rm parser parser.tab.c analex Tabela_de_simbolos.txt


clear
cat docParser.txt