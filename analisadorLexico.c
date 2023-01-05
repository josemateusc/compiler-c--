#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char prox_char(FILE *f){
    return fgetc(f);
}

int main(int agrc, char *argv[]){

    FILE *teste = fopen(argv[1], "rb");
    if(teste == NULL){
        printf("ERRO!");
    }
    
    
    fclose(teste);
    
}