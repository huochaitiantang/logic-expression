#include<stdio.h>
#include<wchar.h>
#include<locale.h>
#include "common.h"

void print_error(char * s){
    int i;
    error++;
    printf("Error[%d] in line [%d] column [%d]:\n", error, row_ind, col_ind);
    setlocale(LC_CTYPE, "");
    wprintf(L"\t%ls", row_val);
    //setlocale(LC_CTYPE, "C");
    //for(i = 0; i < col_ind - token_len - 1; i++)
    //    wprintf(L" ");
    printf("\t%s\n", s);
    return;
}
