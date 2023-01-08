#define _GNU_SOURCE // necessário porque getline() é extensão GNU
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int i = 0; // variável global para percorrer a linha
FILE *docLex;
FILE *file;
char linha[20000] = "";
 //buffer arbitrário

void grava_token(char *token, char *lexema){
    char buffer[strlen(token)+strlen(lexema)+2];
    strcat(buffer,token);
    strcat(buffer,"\t");
    strcat(buffer,lexema);
    strcat(buffer,"\n");
    fwrite(buffer, sizeof(buffer), 1, docLex);
}

char prox_char(){
    if(strlen(linha) == 0){
        fgets(linha, 20000, file);
    }
    if(linha[i] != '\n'){
        char c = linha[i];
        i++;
        return c;
    }
    else{
        i = 0;
        strcmp(linha, "");
        return '\n';
    }
}

void avanca(int* j, char* lexema, char* c){
    lexema[*j] = *c;
    *j+=1;
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
    else if(c == ' ' || c == '\t' || c == '\n'){;}
    else{erro();} //"simbolo inválido"
    return estado;
}

char* analex(){ //classificador de token
    char c = prox_char();  
    int estado = 0;
    char token[30]; //,valor arb
    char lexema[100];
    int j = 0;
    while(1){
        switch (estado){
            case 0:
                estado = estado0(c);
                avanca(&j, lexema, &c);
                break;
            case 1:
                if(isdigit(c) || isalpha(c)){;}
                else if(!(isdigit(c) || isalpha(c))){estado = 2;}
                avanca(&j, lexema, &c);
                break;
            case 2:
                // token = verificar se é identificador, palavra reservada e booleano
                lexema[j-1] = '\0';
                i--;
                return token, lexema;
            case 3:
                if(isdigit(c)){;}
                else if(isalpha(c)){erro();}
                else if(c == '.'){estado = 5;}
                else{estado = 4;}
                avanca(&j, lexema, &c);
                break;
            case 4:
                lexema[j-1] = '/0';
                strcmp(token,"NUM_INTEIRO");
                i--;
                return token, lexema;
            case 5:
                if(isdigit(c)){estado = 6;}
                else{erro();}
                avanca(&j, lexema, &c);
                break;
            case 6:
                if(isdigit(c)){;}
                else if(isalpha(c)){erro();}
                else{estado = 7;}
                avanca(&j, lexema, &c);
                break;
            case 7:
                lexema[j-1] = '\0';
                strcmp(token,"NUM_REAL");
                i--;
                return token, lexema;
            case 8:
                if(c == '\n'){erro();}
                else if(c == '\"'){estado = 9;}
                else{;}
                avanca(&j, lexema, &c);
                break;
            case 9:
                strcmp(token,"STRING");
                i--;
                return token, lexema;
            case 10:
                if(c == '\n'){erro();}
                else{estado = 11;}
                avanca(&j, lexema, &c);
                break;
            case 11:
            //verifica char ?
                if(c == '\''){estado = 12;}
                else{erro();}
            case 12:
                strcmp(token,"CHAR");
                i--;
                return token,lexema;
        }
    }
}

void concatenar_letra(char *str, char ch){
    long int size = strlen(str);
    str[size] = ch;
    str[size + 1] = '\0'; //recolocar terminador
}

void main(int agrc, char *argv[]){

    file = fopen(argv[1], "rb"); //abertura do arquivo
    if(file == NULL){
        printf("ERRO AO ABRIR O CODIGO!");
    }

    docLex = fopen("docLex.txt","wb");
    if(strcmp("a","a")){
        printf("OK\n");
    }
    else{
        printf("No\n");
    }
    //Enquanto o documento ainda não tiver acabado rodará o while
    while(!feof(file)){
        char token[50];
        char lexema[20000];
        // token, lexema = analex(); //Acho que vamos ter que passar o token e lexema como parâmetros, não da para receber assim
        gravar_token(token, lexema);
    }

    fclose(file);
    fclose(docLex);  
}