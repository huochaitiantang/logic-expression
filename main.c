#include<stdio.h>
#include "common.h"

int main(){
    setup_file(); 
    do{
        token = fetch_token();
        print_token(token) 
    }while(token != token_end);

    return 0;
}
