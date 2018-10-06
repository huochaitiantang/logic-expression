#include<stdio.h>
#include<wchar.h>
#include "common.h"

// assign the variable
void assign_var(int index){
    // index >> (varTab_len - 1,....0) & 1
    int i, j;
    j = varTab_len - 1;
    for(i = 0; j >= 0; i++, j--){
        varTabs[j].val = index & 1;    // get the last bit
        index = index >> 1 ; //
    }
    return;
}


// check if some function not defined
void check_undefined(){
    int i;
    for(i = 0; i < pcode_len; i++){
        if(pcodes[i].opt == opt_call_func){
            if(funcTabs[pcodes[i].ind].bits == NULL){
                sprintf(error_info, "Semantics-Error: Undefined function: %s",
                     funcTabs[pcodes[i].ind].ident);
                print_error(error_info);
            }
        }
    }
}



// interpret the pcode one by one
int interpret(){
    int i, j, ind, tmp, base, bit_ind;
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
                for(j = 0; j < funcTabs[ind].args_count; j++){
                    tmp = stacks[--top];
                    bit_ind += base * tmp;
                    //printf("Base:%d tmp:%d bit_ind:%d\n", base, tmp, bit_ind);
                    base = base << 1;
                }

                stacks[top++] = funcTabs[ind].bits[bit_ind];
                break;
        }
        //printf("TOP: %d: %d\n", top, stacks[top - 1]);
    }

    return stacks[--top];
}
