#include<stdio.h>
#include<wchar.h>
#include "common.h"

int main(){
    int i, cnt, res;
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
        printf("Interpret...\n");
        cnt = (int)pow(2, varTab_len);
        for(i = 0; i < cnt; i++){
            assign_var(i);
            print_var_tab();
            res = interpret();
            printf("Res:%d\n", res);
        }
    }
    return 0;
}
