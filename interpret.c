#include<stdio.h>
#include "common.h"

// assign the variable
void assign_var(int index){


    return;
}


// interpret the pcode one by one
int interpret(){
    int i, ind, tmp, base, bit_ind;
    Opt opt;
    top = 0;
    for(i = 0; i < pcode_len; i++){
        opt = pcodes[i].opt;
        ind = pcodes[i].ind;
        switch(opt){
            case opt_push_const:
                stacks[top++] = ind;
                break;
            case opt_push_var:
                stacks[top++] = varTabs[ind].val;
                break;
            case opt_neg:
                stacks[top - 1] = 1 - stacks[top -1];
                break;
            case opt_conjunc:
                tmp = stacks[--top];
                stacks[top - 1] = (stacks[top - 1] + tmp > 1);
                break;
            case opt_disjunc:
                tmp = stacks[--top];
                stacks[top - 1] = (stacks[top - 1] + tmp > 0);
                break;
            case opt_equival:
                tmp = stacks[--top];
                stacks[top - 1] = (stacks[top -1] == tmp);
                break;
            case opt_xor:
                tmp = stacks[--top];
                stacks[top - 1] = (stacks[top - 1] != tmp);
                break;
            case opt_implica:
                tmp = stacks[--top];
                stacks[top - 1] = (stacks[top - 1] <= tmp);
                break;
            case opt_call_func:
                base = 1;
                bit_ind = 0;
                for(i = 0; i < funcTabs[ind].args_count; i++){
                    tmp = stacks[--top];
                    bit_ind += base * tmp;
                    base = base << 2;
                }
                stacks[top++] = funcTabs[ind].bits[bit_ind];
                break;
        }
    }
    return stacks[--top];
}
