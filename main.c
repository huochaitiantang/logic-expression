#include<stdio.h>
#include "common.h"

int main(){
    setup_file(); 
    do{
        fetch_token(0);
        print_token(token);
    }while(token != token_end);

    return 0;
}
