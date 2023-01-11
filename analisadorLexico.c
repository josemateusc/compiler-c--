#define _GNU_SOURCE // necessário porque getline() é extensão GNU
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>

int i = 0; // variável global para percorrer a linha
FILE *docLex;
FILE *file;
char linha[2000];//buffer arbitrário
bool firstSaved;

void gravar_token(char *token, char *lexema){
    char buffer[strlen(token)+strlen(lexema)+2];
    if(!firstSaved){
        strcpy(buffer, "");
        firstSaved = true;
    }else{
        strcpy(buffer,"\n");
    }
    strcat(buffer,token);
    strcat(buffer,"\t");
    strcat(buffer,lexema);
    printf("TOKEN\tLEXEMA\n%s\n", buffer);
    fwrite(buffer, sizeof(char), strlen(buffer), docLex);
    
}

char prox_char(){
    if(strlen(linha) == 0){
        fgets(linha, 2000, file);
    }
    char c = linha[i];//c != '\n' &&
    if(c != '\0'){ //linha[i] != '\n' || linha[i] == '\0'
        i++;
    }
    // else if(feof(file) && c == '\0'){;}
    else{
        i = 0;
        strcpy(linha, "");
        // c = '\n';
    }
    return c;
}

void salvaLexema(int *j, char *lexema, char *c){
    lexema[*j] = *c;
    *j+=1;
    lexema[*j] = '\0';
}

void avanca(int *j, char *lexema, char *c){
    salvaLexema(j,lexema,c);
    *c = prox_char();
}

int estado0(char c){ //função que simula estado 0
    int estado;
    if(isalpha(c)){estado = 1;}
    else if(isdigit(c)){estado = 3;}
    else if(c == '"'){estado = 8;}
    else if(c == '\''){estado = 10;}
    else if(c == '/'){estado = 13;}
    else if(c == '+'){estado = 20;}
    else if(c == '-'){estado = 24;}
    else if(c == '*'){estado = 29;}
    else if(c == '%'){estado = 32;}
    else if(c == '='){estado = 35;}
    else if(c == '!'){estado = 38;}
    else if(c == '>'){estado = 41;}
    else if(c == '<'){estado = 44;}
    else if(c == '<'){estado = 44;}
    else if(c == '|'){estado = 47;}
    else if(c == '&'){estado = 49;}
    else if(c == ','){estado = 52;}
    else if(c == ';'){estado = 53;}
    else if(c == '.'){estado = 54;}
    else if(c == '('){estado = 55;}
    else if(c == ')'){estado = 56;}
    else if(c == ']'){estado = 57;}
    else if(c == '['){estado = 58;}
    else if(c == '{'){estado = 59;}
    else if(c == '}'){estado = 60;}
    else if(c == ':'){estado = 61;}
    else if(c == '?'){estado = 62;}
    else if(c == '\n' || c == ' ' || c == '\t'){estado=0;}
    else if(c == '\0' && feof(file)){estado=99;}
    else if(c == '\0'){estado=0;}
    else{
        gravar_token("ERRO simbolo desconhecido", &c);
        exit(0);
    }
    return estado;
}

char* palavraReservada(char *lexema){
    if(strcmp("void", lexema) == 0){
        return "VOID";
    }
    else if(strcmp("int", lexema) == 0){
        return "INT";
    }
    else if(strcmp("float", lexema) == 0){
        return "FLOAT";
    }
    else if(strcmp("char", lexema) == 0){
        return "CHAR";
    }
    else if(strcmp("bool", lexema) == 0){
        return "BOOL";
    }
    else if(strcmp("if", lexema) == 0){
        return "IF";
    }
    else if(strcmp("else", lexema) == 0){
        return "ELSE";
    }
    else if(strcmp("for", lexema) == 0){
        return "FOR";
    }
    else if(strcmp("while", lexema) == 0){
        return "WHILE";
    }
    else if(strcmp("do", lexema) == 0){
        return "DO";
    }
    else if(strcmp("return", lexema) == 0){
        return "RETURN";
    }
    else if(strcmp("break", lexema) == 0){
        return "BREAK";
    }
    else if(strcmp("continue", lexema) == 0){
        return "CONTINUE";
    }
    else if(strcmp("goto", lexema) == 0){
        return "goto";
    }
    else if(strcmp("true", lexema) == 0){
        return "TRUE";
    }
    else if(strcmp("false", lexema) == 0){
        return "FALSE";
    }
    else{
        return "ID";
    }
}

bool analex(char *token, char *lexema){ //classificador de token
    char c = prox_char();
    int estado = 0;
    int j = 0;
    while(1){
        switch (estado){
            case 0:
                estado = estado0(c);
                if(estado == 0){c = prox_char();}
                else{avanca(&j, lexema, &c);}
                break;
            case 1:
                if(isdigit(c) || isalpha(c)){avanca(&j, lexema, &c);}
                else if(!(isdigit(c) || isalpha(c))){estado = 2;}
                break;
            case 2:
                // lexema[j-1] = '\0';
                strcpy(token, palavraReservada(lexema)); //verificar se é identificador, palavra reservada, booleano ou ID
                i--;
                return true;
                break;
            case 3:
                if(isdigit(c)){avanca(&j, lexema, &c);}
                else if(isalpha(c)){
                    gravar_token("ERRO no numero", lexema);
                    exit(0);
                }
                else if(c == '.'){estado = 5;avanca(&j, lexema, &c);}
                else{estado = 4;}
                
                break;
            case 4:
                // lexema[j-1] = '\0';
                strcpy(token, "NUM_INT");
                i--;
                return true;
                break;
            case 5:
                if(isdigit(c)){estado = 6;avanca(&j, lexema, &c);}
                else{
                    gravar_token("ERRO numero real incompleto", lexema);
                    exit(0);
                }
                break;
            case 6:
                if(isdigit(c)){avanca(&j, lexema, &c);}
                else if(isalpha(c)){
                    gravar_token("ERRO no numero real", lexema);
                    exit(0);
                }
                else{estado = 7;}
                // avanca(&j, lexema, &c);
                break;
            case 7:
                // lexema[j-1] = '\0';
                strcpy(token,"NUM_REAL");
                i--;
                return true;
                break;
            case 8:
                //if(c == '\n'){erro();}
                if(c == '"'){estado = 9;}
                else{;}
                avanca(&j, lexema, &c);
                break;
            case 9:
                strcpy(token,"STRING");
                i--;
                return true;
                break;
            case 10:
                if(c == '\''){estado = 12;}
                else{estado = 11;}
                avanca(&j, lexema, &c);
                break;
            case 11:
                if(c == '\''){estado = 12;}
                else{
                    gravar_token("ERRO no char", lexema);
                    exit(0);
                }
                avanca(&j, lexema, &c);
                break;
            case 12:
                strcpy(token,"CHAR");
                i--;
                return true;
                break;
            case 13:
                if(c == '/'){estado=14;}
                else if(c == '='){estado=19;}
                else if(c == '*'){estado=16;}
                else{estado=18;}
                break;
            case 14:
                avanca(&j, lexema, &c);
                if(c == '\n' || c == '\0'){strcpy(lexema,"");return true;}
                else if(feof(file)){
                    strcpy(token,"FIM_DO_ARQUIVO");
                }
                else{avanca(&j, lexema, &c);}
                break;
            case 16:
                avanca(&j, lexema, &c);
                if(c == '*'){estado=17;}
                else{;}
                break;
            case 17:
                avanca(&j, lexema, &c);
                if(c == '/'){return true;}
                else{
                    gravar_token("ERRO no comentário", lexema);
                    exit(0);
                }
            case 18:
                strcpy(token,"OP_DIV");// /
                i--;
                return true;
                break;
            case 19:
                salvaLexema(&j, lexema, &c);
                strcpy(token,"OP_DIV_REC");// /=
                return true;
                break;
            case 20:
                if(c == '+'){estado=21;}
                else if(c == '='){estado=23;}
                else{estado=22;}
                break;
            case 21:
                salvaLexema(&j, lexema, &c);
                strcpy(token,"OP_INC"); //++
                return true;
                break;
            case 22:
                strcpy(token,"OP_SOMA"); // +
                i--;
                return true;
                break;
            case 23:
                salvaLexema(&j, lexema, &c);
                // i--;
                strcpy(token,"OP_SOMA_REC"); //+=
                return true;
                break;
            case 24:
                if(c == '-'){estado=26;}
                else if(c == '='){estado=27;}
                else if(c == '>'){estado=28;}
                else{estado=25;}
                break;
            case 25:
                strcpy(token,"OP_SUB"); // -
                i--;
                return true;
                break;
            case 26:
                salvaLexema(&j, lexema, &c);
                // i--;
                strcpy(token,"OP_DEC"); // --
                return true;
                break;
            case 27:
                salvaLexema(&j, lexema, &c);
                // i--;
                strcpy(token,"OP_SUB_REC"); // -=
                return true;
                break;
            case 28:
                salvaLexema(&j, lexema, &c);
                // i--;
                strcpy(token,"SETA"); // ->
                return true;
                break;
            case 29:
                if(c == '='){estado=30;}
                else{estado=31;}
                break;
            case 30:
                salvaLexema(&j, lexema, &c);
                // i--;
                strcpy(token,"OP_MULT_REC"); // *=
                return true;
                break;
            case 31:
                strcpy(token,"OP_MULT"); // *
                i--;
                return true;
                break;
            case 32:
                if(c == '='){estado=34;}
                else{estado=33;}
                break;
            case 33:
                strcpy(token,"OP_RESTO"); // %
                i--;
                return true;
                break;
            case 34:
                salvaLexema(&j, lexema, &c);
                // i--;
                strcpy(token,"OP_RESTO_REC"); // %=
                return true;
                break;
            case 35:
                if(c == '='){estado=36;}
                else{estado=37;}
                break;
            case 36:
                salvaLexema(&j, lexema, &c);
                // i--;
                strcpy(token,"COMP_IGUAL"); // ==
                return true;
                break;
            case 37:
                strcpy(token,"OP_ATRIB"); // =
                i--;
                return true;
                break;
            case 38:
                if(c == '='){estado=39;}
                else{estado=40;}
                break;
            case 39:
                salvaLexema(&j, lexema, &c);
                strcpy(token,"COMP_DIF"); // !=
                return true;
                break;
            case 40:
                strcpy(token,"OP_NOT"); // !
                i--;
                return true;
                break;
            case 41:
                if(c == '='){estado=42;}
                else{estado=43;}
                break;
            case 42:
                salvaLexema(&j, lexema, &c);
                strcpy(token,"MAIOR_IGUAL"); // >=
                return true;
                break;
            case 43:
                strcpy(token,"MAIOR"); // >
                i--;
                return true;
                break;
            case 44:
                if(c == '='){estado=46;}
                else{estado=45;}
                break;
            case 45:
                strcpy(token,"MENOR"); // <
                i--;
                return true;
                break;
            case 46:
                salvaLexema(&j, lexema, &c);
                strcpy(token,"MENOR_IGUAL"); // <=
                return true;
                break;
            case 47:
                if(c == '|'){estado=48;}
                else{
                    gravar_token("ERRO de simbolo incompleto", lexema);
                    exit(0);
                }
                break;
            case 48:
                salvaLexema(&j, lexema, &c);
                strcpy(token,"OU"); // ||
                return true;
                break;
            case 49:
                if(c == '&'){estado=50;}
                else{estado=51;}
                break;
            case 50:
                salvaLexema(&j, lexema, &c);
                strcpy(token,"E"); // &&
                return true;
                break;
            case 51:
                strcpy(token,"ENDERECO"); // &
                i--;
                return true;
                break;
            case 52:
                strcpy(token,"VIRG"); // ,
                i--;
                return true;
                break;
            case 53:
                strcpy(token,"PV"); // ;
                i--;
                return true;
                break;
            case 54:
                strcpy(token,"PONTO"); // .
                i--;
                return true;
                break;
            case 55:
                strcpy(token,"ABRE_PAREN"); // (
                i--;
                return true;
                break;
            case 56:
                strcpy(token,"FECHA_PAREN"); // )
                i--;
                return true;
                break;
            case 57:
                strcpy(token,"FECHA_COLC"); // ]
                i--;
                return true;
                break;
            case 58:
                strcpy(token,"ABRE_COLC"); // [
                i--;
                return true;
                break;
            case 59:
                strcpy(token,"ABRE_CHAV"); // {
                i--;
                return true;
                break;
            case 60:
                strcpy(token,"FECHA_CHAV"); // }
                i--;
                return true;
                break;
            case 61:
                strcpy(token,"OP_DOIS_PONTOS"); // :
                i--;
                return true;
                break;
            case 62:
                strcpy(token,"OP_SELEC"); // ?
                i--;
                return true;
                break;
            case 99: //fim do arquivo
                strcpy(token,"FIM_DO_ARQUIVO");
                return true;
                break;
        }
    }
}

int main(int agrc, char *argv[]){

    file = fopen(argv[1], "rb"); //abertura do arquivo
    if(file == NULL){
        printf("ERRO AO ABRIR O CODIGO!");
    }
    
    docLex = fopen("docLex.txt","wb");
    strcpy(linha,"");
    firstSaved = false;
    char token[15];
    char *lexema = malloc(100000 * sizeof(char));
    //Enquanto o documento ainda não tiver acabado rodará o while
    while(1){
        strcpy(token,"");
        strcpy(lexema,"");
        analex(token, lexema);
        if(!strcmp(token,"FIM_DO_ARQUIVO")){printf("\nFim da Analise léxica\n\n");break;}
        else if(strcmp(token,"")){gravar_token(token, lexema);}
    }

    free(lexema);
    fclose(file);
    fclose(docLex);
    return 0;
}