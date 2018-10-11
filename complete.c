#include<stdio.h>
#include<math.h>
#include<string.h>
#include<wchar.h>
#include "common.h"


// get max args num in the funcTabs
int max_args_num(){
    int i, tmp = 0;
    for(i = 0; i < funcTab_len; i++)
        if(funcTabs[i].args_count > tmp)
            tmp = funcTabs[i].args_count;
    return tmp;
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


// check if all number in p < base if base > 0
int all_less(int* p, int m, int base){
    int i;
    for(i = 0; i < m; i++)
        if(p[i] >= base)
            return 0;
    return 1;
}

void print_base2(int n){
    int i;
    int tmp_array[4];
    if(n <0 || n >= 16){
        printf("Error!");
        return;
    }
    for(i = 3; i >= 0; i--){
        tmp_array[i] = n & 1;
        n = n >> 1;
    }
    for(i = 0; i < 4; i++)
        printf("%d",tmp_array[i]);
}

// print array
void print_comp(int comp[], int n){
    int i;
    for(i = 0; i < n; i++){
        print_base2(comp[i]);
        printf(" [%d] [%d]\n", i, comp[i]);
    }
    return;
}


// check complete by violence
void check_complete(){
    int i,j,k,l,m,n;
    int flag[16];
    int comp[16];
    int res_ind[4];
    int res, base, cnt, len, add_new_flag;
    int comp_ptr = 0, pre_comp_ptr = 0;
    int * base_n;
    char * formula_str[16];

    //printf("Judge if complete set...\n");
    base_n = (int *)malloc(sizeof(int) * max_args_num());
    for(i = 0; i < 16; i++)
        flag[i] = 0;
    // init comp with p(0011), q(0101)
    comp[comp_ptr++] = 3;
    comp[comp_ptr++] = 5;
    flag[3] = 1;
    flag[5] = 1;
    formula_str[0] = (char *)malloc(sizeof(char) * 2);
    formula_str[1] = (char *)malloc(sizeof(char) * 2);
    strcpy(formula_str[0], "p");
    strcpy(formula_str[1], "q");
    //judge
    while(1){
        add_new_flag = 0;
        n = comp_ptr;
        for(i = 0; i < funcTab_len; i++){
            // choose m as func args from n existed result
            m = funcTabs[i].args_count;
            cnt = (int)(pow(n, m) + 0.5);
            // m = 3, n = 2, cnt = 8, base_n = 000,001,010,011,100,101,110,111
            for(j = 0; j < cnt; j++){
                to_base_n(j, n, m, base_n);
                // decrease repeat calculate,
                // only attention new res added last time
                if(all_less(base_n, m, pre_comp_ptr))
                    continue;
                //  comp(n = 2): 0011,0101
                //  chose(m = 3, base_n = 011): 0011,0101,0101
                //  res_ind = 0347(000,011,100,111)
                for(l = 0; l < 4; l++)
                    res_ind[l] = 0;
                for(k = m - 1, base = 1; k >= 0; k--){
                    res = comp[base_n[k]];
                    for(l = 3; l >= 0; l--){
                        res_ind[l] += ((res & 1) * base);
                        res = res >> 1;
                    }
                    base = base << 1;
                }
                // res = func.bits[0],func.bits[3],func.bits[4],func.bits[7] = 0110 = 6
                for(k = 3, base = 1, res = 0; k >= 0; k--){
                    res += (base * funcTabs[i].bits[res_ind[k]]);
                    base = base << 1;
                }
                // add to the existed array
                if(flag[res] == 0){
                    comp[comp_ptr] = res;
                    flag[res] = 1;
                    add_new_flag = 1;
                    // malloc space for new formula
                    len = strlen(funcTabs[i].ident) + 1;
                    for(k = 0; k < m; k++){
                        len += (strlen(formula_str[base_n[k]]) + 1);
                    }
                    formula_str[comp_ptr] = (char *)malloc(sizeof(char) * (len + 1));
                    // formula: f_name(args1, args2, args3,...)
                    strcpy(formula_str[comp_ptr], funcTabs[i].ident);
                    strcat(formula_str[comp_ptr], "(");
                    for(k = 0; k < m; k++){
                        strcat(formula_str[comp_ptr], formula_str[base_n[k]]);
                        if(k + 1 < m)
                            strcat(formula_str[comp_ptr], ",");
                    }
                    strcat(formula_str[comp_ptr], ")");
                    comp_ptr++;

                    /*
                    printf("--j:%d base(%d):\n", j, n);
                    for(k = 0; k < m; k++){
                        printf("%d:", base_n[k]);
                        print_base2(comp[base_n[k]]);
                        printf("\n");
                    }
                    printf("res_ind: ");
                    for(l = 0; l < 4; l++)
                        printf("%d", res_ind[l]);
                    printf("\nadd_new_res:%d\n", res);
                    */
                }
            }
        }

        pre_comp_ptr = n;
        //print_comp(comp, comp_ptr);
        //printf("pre:%d ptr:%d\n", pre_comp_ptr, comp_ptr);
        if(add_new_flag == 0 || comp_ptr >= 16)
            break;
    }
    //print_comp(comp, comp_ptr);
    if(comp_ptr >= 16){
        printf("--Complete!\n");
    }
    else{
        printf("--Not Complete!\n");
    }
    for(i = 0; i < comp_ptr; i++){
        printf("[%d] ",i);
        print_base2(comp[i]);
        printf(" %s\n", formula_str[i]);
    }
    free(base_n);
    for(i = 0; i < comp_ptr; i++){
        free(formula_str[i]);
    }
}


// calculate C(n,m), not used
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


// calculate A(n,n), not used
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




