#include<stdio.h>
#include "common.h"

void print_error(char * s){
    int i;
    error++;
    printf("Error[%d] in line [%d] column [%d]:\n", error, row_ind, col_ind);
    printf("  %ls  ", row_val);
    for(i = 0; i < col_ind - token_len - 1; i++)
        printf(" ");
    printf("^\n  %s\n", s);
    return;
}
