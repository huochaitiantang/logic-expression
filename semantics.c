#include<stdio.h>
#include<string.h>
#include "common.h"


// find the variable index in the variable table
int lookup_var_tab(char* ident){
    int i;
    for(i = 0; i < varTab_len; i++)
        if(strcmp(varTabs[i].ident, ident)==0)
            return i;
    return -1;
}


// find the function index in the function table
int lookup_func_tab(char* ident){
    int i;
    for(i = 0; i < funcTab_len; i++)
        if(strcmp(funcTabs[i].ident, ident) == 0)
            return i;
    return -1;
}


// insert the variable into the variable tables
int insert_var_tab(char* ident){

}




