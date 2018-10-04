#include<stdio.h>
#include<math.h>
#include "common.h"


// get max args num in the funcTabs
int max_args_num(){
    int i, tmp = 0;
    for(i = 0; i < funcTab_len; i++)
        if(funcTabs[i].args_count > tmp)
            tmp = funcTabs[i].args_count;
    return tmp;
}


// print combination
void print_combina(unsigned long* combina, long long combina_len, int bit_len){
    int i, j, k, tmp;
    int* tmp_array;
    tmp_array = (int *)malloc(sizeof(int) * bit_len);
    for(i = 0; i <combina_len; i++){
        tmp = combina[i];
        for(j = 0, k = bit_len - 1; j < bit_len; k--, j++){
            tmp_array[k] = tmp & 1;
            tmp = tmp >> 1;
        }
        for(j = 0; j < bit_len; j++)
            printf("%d", tmp_array[j]);
        printf("\n");
    }
    return;
}


// check complete by violence
int check_complete(){
    int i, j, k, tmp, base, val, complete=0;
    int max_args, max_bits_len;
    long long max_combina;
    unsigned char* flag;
    unsigned long* combina;
    long long combina_len = 0;
    max_args = max_args_num();                      // n
    max_bits_len = (int)pow(2, max_args);           // 2^n
    max_combina = (long long)pow(2, max_bits_len);  // 2^(2^n)
    printf("Judge if %d complete set...\n", max_args);
    if(max_args > 0){
        flag = (unsigned char*)malloc(sizeof(unsigned char) * max_combina);
        combina = (unsigned long*)malloc(sizeof(unsigned long) * max_combina);
        for(i = 0; i < max_combina; i++) flag[i] = 0;
        // init with 0, 1: 00000000, 11111111,
        flag[0] = 1;
        flag[max_combina - 1] = 1;
        combina[combina_len++] = 0;
        combina[combina_len++] = max_combina - 1;
        //  init with p, q, r(three): 01010101, 00110011, 00001111
        for(i = 0, tmp = 1; i < max_args; i++){
            for(j = 0, k = 0, base = 1, val = 0; j < max_bits_len; j++){
                val = val + (int)pow(2, j) * base;
                k++;
                if(k >= tmp){
                    k = 0;
                    base = 1 - base;
                }
            }
            flag[val] = 1;
            combina[combina_len++] = val;
            tmp = tmp << 1; // 1, 2, 4, ..
        }
        printf("Init with:\n");
        print_combina(combina, combina_len, max_bits_len);

        // judge
        while(1){
            if(combina_len >= max_combina){
                complete = 1;
                break;
            }
            tmp = 0;  // if new add to the set
            for(i = 0; i < funcTab_len; i++){
                // choice args_num val in the combina

                    // calculate by the function defined


                    break;

            }
            if(tmp == 0) break;
        }
        if(complete == 1){
            printf("Complete\n");
        }
        else{
            printf("Combination set:\n");
            print_combina(combina, combina_len, max_bits_len);
        }

    }
    else{
        printf("No Complete\n");
    }



}
