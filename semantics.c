#include<stdio.h>
#include<string.h>
#include<math.h>
#include<wchar.h>
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
    VarTab tmp;
    tmp.val = 0;
    tmp.ident = (char*)malloc(sizeof(ident));
    strcpy(tmp.ident, ident);
    varTabs[varTab_len] = tmp;
    varTab_len++;
    return varTab_len - 1;
}


// insert the function into the variable tables
int insert_func_tab(char* ident, int args_num){
    FuncTab tmp;
    tmp.args_count = args_num;
    tmp.ident = (char*)malloc(sizeof(ident));
    tmp.bits = NULL;
    tmp.bits_count = 0;
    strcpy(tmp.ident, ident);
    funcTabs[funcTab_len] = tmp;
    funcTab_len++;
    return funcTab_len - 1;
}


//complete the func table when declared
void complete_func_tab(int ind, int* bits, int bit_len){
    int i;
    funcTabs[ind].bits = (int *)malloc(sizeof(int) * bit_len);
    funcTabs[ind].bits_count = bit_len;
    for(i = 0; i < bit_len; i++)
        funcTabs[ind].bits[i] = bits[i];
    return;
}


// generate the p-code
void gen_pcode(Opt opt, int ind){
    PCode tmp;
    tmp.opt = opt;
    tmp.ind = ind;
    pcodes[pcode_len] = tmp;
    pcode_len++;
    return;
}


// print the VarTabs
void print_var_tab(){
    int i;
    printf("-----------varTabs-------------\n");
    for(i = 0; i < varTab_len; i++)
        printf("%d %s %d\n", i, varTabs[i].ident, varTabs[i].val);
    printf("-------------------------------\n");
    return;
}


// print the FuncTabs
void print_func_tab(){
    int i, j, args_cnt, bits_cnt;
    printf("-----------funcTabs------------\n");
    for(i = 0; i < funcTab_len; i++){
        args_cnt = funcTabs[i].args_count;
        bits_cnt = funcTabs[i].bits_count;
        printf("%d %s %d ", i, funcTabs[i].ident, args_cnt);
        for(j = 0; j < bits_cnt; j++)
            printf("%d", funcTabs[i].bits[j]);
        printf("\n");
    }
    printf("-------------------------------\n");
    return;
}


// print the PCodes
void print_pcodes(){
    int i;
    char* s[] = {"pushc","pushv", "call", "neg", "conjunc", "disjunc", "xor", "equival", "implica"};
    printf("------------pcode--------------\n");
    for(i =0; i < pcode_len; i++){
        printf("%d %s ", i, s[pcodes[i].opt]);
        if(pcodes[i].opt == opt_push_var && pcodes[i].ind >= 0)
            printf(" %s [ind=%d]", varTabs[pcodes[i].ind].ident, pcodes[i].ind);
        if(pcodes[i].opt == opt_call_func && pcodes[i].ind >=0)
            printf(" %s [ind=%d]", funcTabs[pcodes[i].ind].ident, pcodes[i].ind);
        printf("\n");
    }
    printf("-------------------------------\n");
    return;
}
