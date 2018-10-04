#include<stdio.h>
#include<wchar.h>
#include "common.h"

int main(){
    setup_file();
    /*do{
        fetch_token(2);
        print_token(token);
    }while(token != token_end);
    */
    scan_passage();
    print_var_tab();
    print_func_tab();
    print_pcodes();
    printf("Error: %d\n", error);
    if(error == 0){
        printf("Interpret...");
    }
    return 0;
}
