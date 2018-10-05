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
        printf(" [%d] [%d]\n", i, combina[i]);
    }
    return;
}


// fac
int fac(int n){
    if(n == 1) return 1;
    return n * fac(n - 1);
}




// calculate combine
int combine_cnt(int n, int m){
    int i,sum=1;
    for(i = m + 1; i <= n; i++)
        sum = sum * i;
    sum = sum / fac(n - m);
    return sum;
}


// calculate C(n,m)
void C_n_m(int n, int m, int* res){
    int *p;
    int j, k = 0, ind;
    p = (int*)malloc(sizeof(int)*m);
    ind = 0;
    p[ind] = 0;
    while(1){
        if(p[ind] >= n){
            if(ind == 0) break;
            ind--;
            p[ind]++;
        }
        else if(ind == m - 1){
            // one result c(n,m)
            for(j = 0; j < m; j++){
                res[k++] = p[j];
            }
            p[ind]++;
        }
        else{
            ind++;
            p[ind]=p[ind-1]+1;
        }
    }
    free(p);
    return;
}


// calculate A(n,n)
void A_n_n(int n, int * res){
    int i, j, k = 0, tmp;
    int* p;
    p=(int*)malloc(sizeof(int)*n);
    for(i = 0; i < n; i++) p[i] = i;
    while(1){
        for(i = 0; i < n; i++)
            res[k++] = p[i];
        for(i = n - 1; i > 0 && p[i] < p[i - 1]; i--);
        if(i==0) break;
        for(j = n-1; j > i && p[j] < p[i - 1]; j--);
        tmp = p[i - 1];
        p[i - 1] = p[j];
        p[j]=tmp;
        for(i = i, j = n - 1; i < j; i++, j--){
            tmp=p[i];
            p[i]=p[j];
            p[j]=tmp;
        }
    }
    return;
}


// number(10) to number(n)(m bit)
int to_base_n(int number, int n, int m, int * res){
    int i;
    for(i = 0; i < m ; i++)
        res[i] = 0;
    for(i = m - 1; i >= 0; i--){
        res[i] = number % n;
        number = number / n;
        if(number <= 0)
            break;
    }
    return;
}


// check complete by violence
int check_complete(){
    int i, j, k, l, n, m;
    float cnt;
    int tmp, base, val, ind, complete=0;
    int max_args, max_bits_len;
    int * p;
    int * q;
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
                // choice args_num val in the combina: C(n, m)
                n = combina_len;
                m = funcTabs[i].args_count;
                cnt = pow(n, m);
                p = (int *)malloc(sizeof(int) * m);
                q = (int *)malloc(sizeof(int) * max_bits_len);
                for(j = 0; j < cnt; j++){
                    to_base_n(j, n, m, p);
                    for(k = 0; k < max_bits_len; k++)
                        q[k] = 0;
                    printf("%d: \n", j);

                    // combina is
                    // 00000000
                    // 11111111
                    // 01010101
                    // 00110011
                    // 00001111
                    // p[k] is the choose index in the combina, exp 2, 3, 4
                    // val is
                    //      01010101(85) base=4
                    //      00110011(51) base=2
                    //      00001111(15) base=1
                    // q is 04261537
                    print_combina(combina, combina_len, max_bits_len);
                    for(k = m - 1, base = 1; k >= 0; k--){
                        val = combina[p[k]];
                        printf("choose: [%d] [%d] \n", p[k], val);
                        for(l = max_bits_len - 1; l >= 0; l--){
                            q[l] = q[l] + (val & 1) * base;
                            val = val >> 1;
                        }
                        base = base << 1;
                    }
                    for(k = 0; k < max_bits_len; k++)
                            printf("%d ", q[k]);
                    base = 1;
                    val = 0;
                    // calculate by the function defined
                    for(k = max_bits_len - 1; k >= 0; k--){
                        val = val + base * funcTabs[i].bits[q[k]];
                        base = base << 1;
                    }
                    // add to the combina array
                    if(flag[val] == 0){
                        combina[combina_len++] = val;
                        flag[val] = 1;
                        tmp = 1;
                    }
                    printf("\n");
                }
            }
            if(tmp == 0) break;
        }
        if(complete == 1){
            printf("Complete\n");
        }
        else{
            printf("Combination set:\n");
            print_combina(combina, combina_len, max_bits_len);
            printf("Not Complete\n");
        }

    }
    else{
        printf("No Complete\n");
    }



}



