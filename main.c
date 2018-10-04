#include<stdio.h>
#include<wchar.h>
#include "common.h"

int main(){
    int i, j, cnt, tmp, all0 = 1, all1 = 1;
    int* res;
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
        res = (int*)malloc(sizeof(int) * cnt);
        for(i = 0; i < varTab_len; i++)
            printf("%s ", varTabs[i].ident);
        printf(" Res\n");
        for(i = 0; i < cnt; i++){
            assign_var(i);
            //print_var_tab();
            tmp = interpret();
            for(j = 0; j < varTab_len; j++)
                printf("%d ", varTabs[j].val);
            printf(" %d\n", tmp);
            if(tmp == 0) all1 = 0;
            if(tmp == 1) all0 = 0;
            res[i] = tmp;
        }
        if(all0)
            printf("Always False!\n");
        else if(all1)
            printf("Always True!\n");
        /*
        for(i = 0; i < varTab_len; i++)
            printf("%s ", varTabs[i].ident);
        printf(" Res\n");
        for(i = 0; i < cnt; i++){
            assign_var(i);
            for(j = 0; j < varTab_len; j++)
                printf("%d ", varTabs[j].val);
            printf(" %d\n", res[i]);
        }
        */
        tmp = check_complete();

    }
    return 0;
}
