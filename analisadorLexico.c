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

void gravar_token(char *token, char *lexema){
    char buffer[strlen(token)+strlen(lexema)+2];
    strcpy(buffer, "");
    strcat(buffer,token);
    strcat(buffer,"\t");
    strcat(buffer,lexema);
    strcat(buffer,"\n");
    printf("TOKEN\tLEXEMA\n%s\n", buffer);
    fwrite(buffer, sizeof(char), strlen(buffer), docLex);
}

char prox_char(){
    if(strlen(linha) == 0){
        fgets(linha, 2000, file);
    }
    if(linha[i] != '\n' || linha[i] == '\0'){
        char c = linha[i];
        i++;
        return c;
    }
    else{
        i = 0;
        strcpy(linha, "");
        return '\n';
    }
}

void avanca(int *j, char *lexema, char *c){
    lexema[*j] = *c;
    *j+=1;
    lexema[*j] = '\0';
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
    else if(c == '\0'){estado=99;}
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
                if(estado == 0){
                    c = prox_char();
                }
                else{avanca(&j, lexema, &c);}
                break;
            case 1:
                if(isdigit(c) || isalpha(c)){;}
                else if(!(isdigit(c) || isalpha(c))){estado = 2;}
                avanca(&j, lexema, &c);
                break;
            case 2:
                lexema[j-1] = '\0';
                strcpy(token, palavraReservada(lexema)); //verificar se é identificador, palavra reservada, booleano ou ID
                i--;
                return true;
                break;
            case 3:
                if(isdigit(c)){;}
                else if(isalpha(c)){
                    gravar_token("ERRO no numero", lexema);
                    exit(0);
                }
                else if(c == '.'){estado = 5;}
                else{estado = 4;}
                avanca(&j, lexema, &c);
                break;
            case 4:
                lexema[j-1] = '\0';
                strcpy(token, "NUM_INT");
                i--;
                return true;
                break;
            case 5:
                if(isdigit(c)){estado = 6;}
                else{
                    gravar_token("ERRO numero real incompleto", lexema);
                    exit(0);
                }
                avanca(&j, lexema, &c);
                break;
            case 6:
                if(isdigit(c)){;}
                else if(isalpha(c)){
                    gravar_token("ERRO no numero real", lexema);
                    exit(0);
                }
                else{estado = 7;}
                avanca(&j, lexema, &c);
                break;
            case 7:
                lexema[j-1] = '\0';
                strcpy(token,"NUM_REAL");
                i--;
                return true;
                break;
            case 8:
                //if(c == '\n'){erro();}
                if(c == '\"'){estado = 9;}
                else{;}
                avanca(&j, lexema, &c);
                break;
            case 9:
                strcpy(token,"STRING");
                // i--;
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
                // i--;
                return true;
                break;
            case 13:
                if(c == '/'){estado=14;}
                else if(c == '='){estado=19;}
                else if(c == '*'){estado=16;}
                else{estado=18;}
            case 14:
                if(c == '\n'){strcpy(lexema,"");return true;}
                else{avanca(&j, lexema, &c);}
                break;
            case 16:
                if(c == '*'){estado=17;}
                else{;}
                avanca(&j, lexema, &c);
                break;
            case 17:
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
                avanca(&j, lexema, &c);
                i--;
                strcpy(token,"OP_DIV_REC");// /=
                return true;
                break;
            case 20:
                if(c == '+'){estado=21;}
                else if(c == '='){estado=23;}
                else{estado=22;}
                break;
            case 21:
                avanca(&j, lexema, &c);
                i--;
                strcpy(token,"OP_INC"); //++
                return true;
                break;
            case 22:
                strcpy(token,"OP_SOMA"); // +
                i--;
                return true;
                break;
            case 23:
                avanca(&j, lexema, &c);
                i--;
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
                avanca(&j, lexema, &c);
                i--;
                strcpy(token,"OP_DEC"); // --
                return true;
                break;
            case 27:
                avanca(&j, lexema, &c);
                i--;
                strcpy(token,"OP_SUB_REC"); // -=
                return true;
                break;
            case 28:
                avanca(&j, lexema, &c);
                i--;
                strcpy(token,"SETA"); // ->
                return true;
                break;
            case 29:
                if(c == '='){estado=30;}
                else{estado=31;}
                break;
            case 30:
                avanca(&j, lexema, &c);
                i--;
                strcpy(token,"OP_MULT_REC"); // *=
                return true;
                break;
            case 31:
                strcpy(token,"OP_MULT"); // *
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

    //Enquanto o documento ainda não tiver acabado rodará o while
    while(1){
        char token[15] = "";
        char *lexema = malloc(100000 * sizeof(char));
        strcpy(lexema,"");
        analex(token, lexema);
        if(!strcmp(token,"FIM_DO_ARQUIVO")){printf("Fim da Analise léxica\n\n");break;}
        else if(strcmp(token,"")){gravar_token(token, lexema);}
    }

    fclose(file);
    fclose(docLex);
    return 0;
}