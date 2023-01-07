#define _GNU_SOURCE // necessário porque getline() é extensão GNU
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>

int i = 0; // variável global para percorrer a linha

FILE *docLex;

void grava_token(char *token, char *lexema){
    char buffer[strlen(token)+strlen(lexema)+2];
    strcat(buffer,token);
    strcat(buffer,"\t");
    strcat(buffer,lexema);
    strcat(buffer,"\n");
    fwrite(buffer, sizeof(buffer), 1, docLex);
}

char prox_char(char *linha){
    char c = linha[i];
    i++;
    return c;
}


// char* analex(char *c){ //verificador de token para sinais de pontuação
//     int estado = 0;
//     while(1){
//         switch (estado){
//         case 0:
//             if(c == ','){estado = 1;}
//             else if(c == ';'){estado = 2;}
//             else if(c == '('){estado = 3;}
//             else if(c == ')'){estado = 4;}
//             else if(c == '['){estado = 5;}
//             else if(c == ']'){estado = 6;}
//             else if(c == '{'){estado = 7;}
//             else if(c == '}'){estado = 8;}
//             /*else{} o que fazer quando nenhum caminho servir ?*/
//             break;
//         case 1:
//             return ',';
//             break;
//         case 2:
//             return ';';
//             break;
//         case 3:
//             return '(';
//             break;
//         case 4:
//             return ')';
//             break;
//         case 5:
//             return '[';
//             break;
//         case 6:
//             return ']';
//             break;
//         case 7:
//             return '{';
//             break;
//         case 8:
//             return '}';
//             break;
//         }
        
//     }
// }

void concatenar_letra(char *str, char ch){
    long int size = strlen(str);
    str[size] = ch;
    str[size + 1] = '\0'; //recolocar terminador
}

void analexLine(char *str){
    while(i < strlen(str)){
        char token[200] = ""; //Verificar o tamanho do maior token possivel +1
        char lexema[200] = "";
        char ch;
        while(token != ""){
            ch = prox_char(str);
            concatenar_letra(lexema,ch);
            // strcpy(token,analex(lexema));
        }
        grava_token(token,lexema);
    }
    i = 0;
}

void main(int agrc, char *argv[]){

    FILE *file = fopen(argv[1], "rb"); //abertura do arquivo
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

        //Recebe cada linha do arquivo
        char line[20000]; //buffer arbitrário
        fgets(line, 20000, file);

        // analexLine(line);
    }

    fclose(file);
    fclose(docLex);  
}
