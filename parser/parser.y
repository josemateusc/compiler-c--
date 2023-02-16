%{
#include <iostream>
#include <cctype>
using namespace std;

int yylex(void);
int yyparse(void);
void yyerror(const char *);
%}


%token VOID
%token INT
%token FLOAT
%token CHAR_t //Tipo
%token BOOL
%token IF
%token ELSE
%token FOR
%token WHILE
%token DO
%token RETURN
%token BREAK
%token CONTINUE
%token GOTO
%token TRUE
%token FALSE
%token ID
%token CHAR //Elemento
%token NUM_INT
%token NUM_REAL
%token STRING
%token OP_DIV
%token OP_DIV_REC
%token OP_INC
%token OP_SOMA
%token OP_SOMA_REC
%token OP_SUB
%token OP_DEC
%token OP_SUB_REC
%token SETA
%token OP_MULT_REC
%token OP_MULT
%token OP_RESTO
%token OP_RESTO_REC
%token COMP_IGUAL
%token OP_ATRIB
%token COMP_DIF
%token OP_NOT
%token MAIOR_IGUAL
%token MAIOR
%token MENOR
%token MENOR_IGUAL
%token OU
%token E
%token ENDERECO
%token VIRG
%token PV
%token PONTO
%token ABRE_PAREN
%token FECHA_PAREN
%token FECHA_COLC
%token ABRE_COLC
%token ABRE_CHAV
%token FECHA_CHAV
%token OP_DOIS_PONTOS
%token OP_SELEC

%%

//grammar
expression: MAIOR E 
            | /* vazio*/
            ;

%%

int yylex() {
	;
}

void yyerror(const char * s){
    cout << "Erro!\n";
   	exit(0);
}

int main(int argc, char ** argv){
    if (argc > 1){
		FILE *file;
		file = fopen(argv[1], "r");
		if (!file){
			cout << "Arquivo " << argv[1] << " não encontrado!\n";
			exit(1);
		}
		
		/* entrada ajustada para ler do arquivo */
		yyin = file;
	}

	yyparse();
}