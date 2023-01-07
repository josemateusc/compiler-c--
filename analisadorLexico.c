#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>

int i = 0; // variável global para percorrer a linha
char prox_char(char *linha){
    if(linha[i] != '\n'){
        char c = linha[i];
        i++;
        return c;
    }
    else{
        i = 0;
        return;
    }
}

char sinais_pontuacao_token(char c){ //verificador de token para sinais de pontuação
    int estado = 0;
    while(1){
        switch (estado){
        case 0:
            if(c == ','){estado = 1;}
            else if(c == ';'){estado = 2;}
            else if(c == '('){estado = 3;}
            else if(c == ')'){estado = 4;}
            else if(c == '['){estado = 5;}
            else if(c == ']'){estado = 6;}
            else if(c == '{'){estado = 7;}
            else if(c == '}'){estado = 8;}
            /*else{} o que fazer quando nenhum caminho servir ?*/
            break;
        case 1:
            return ',';
            break;
        case 2:
            return ';';
            break;
        case 3:
            return '(';
            break;
        case 4:
            return ')';
            break;
        case 5:
            return '[';
            break;
        case 6:
            return ']';
            break;
        case 7:
            return '{';
            break;
        case 8:
            return '}';
            break;
        }
        
    }
}

int main(int agrc, char *argv[]){

    FILE *teste = fopen(argv[1], "rb"); //abertura do arquivo
    if(teste == NULL){
        printf("ERRO!");
    }

    char str[200];
    fgets(str, 200, teste);
    for(int j = 0; j <11; j++){printf("%c", prox_char(str));}
    printf("\n");

    fgets(str, 200, teste);
    for(int j = 0; j <11; j++){printf("%c", prox_char(str));}
    printf("\n");
    
    fclose(teste);
    
}
