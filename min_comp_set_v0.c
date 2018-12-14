#include<stdio.h>
#include "common.h"
#define N 18


/*
	version_0:
	
	for 18 functions min complete set
	slow and memory cost a lot, but for these 18 functions is enough
	
*/ 

typedef struct{
	int len;
	int* inds;
} Complete;

FuncTab cands[N];
Complete complete[100];
int complete_len = 0;

void init_cands(){
	int i, j, args_cnt, bits_cnt;
	char* idents[] = {"F","G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W"};
	int args_counts[] = {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int bits_counts[] = {1, 1, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
	char* bits[] = {"0", "1", "01", "10",
					"0001", "0010", "0011", 
					"0100", "0101", "0110", "0111", 
					"1000", "1001", "1010", "1011",
					"1100", "1101", "1110"};
	for(i = 0; i < N; i++){
		cands[i].ident = (char*)malloc(sizeof(char)*1);
		strcpy(cands[i].ident, idents[i]);
		cands[i].args_count = args_counts[i];
		cands[i].bits_count = bits_counts[i];
		cands[i].bits = (int *)malloc(sizeof(int) * bits_counts[i]);
		for(j = 0; j < bits_counts[i]; j++)
			cands[i].bits[j] = bits[i][j] - '0';
	}
	/*
	printf("-----------initTabs------------\n");
    for(i = 0; i < N; i++){
        args_cnt = cands[i].args_count;
        bits_cnt = cands[i].bits_count;
        printf("%d %s %d ", i, cands[i].ident, args_cnt);
        for(j = 0; j < bits_cnt; j++)
            printf("%d", cands[i].bits[j]);
        printf("\n");
    }
    printf("-------------------------------\n");
	*/
}


int inside_array(int x, int len, int* array){
	int i;
	for(i = 0; i < len; i++){
		if(array[i] == x)
			return 1;
	}
	return 0;
}

int include_comp(Complete comp, int * cur_choose, int cur_len){
	int i;
	for(i = 0; i < comp.len; i++){
		if(!inside_array(comp.inds[i], cur_len, cur_choose))
			return 0;
	}
	return 1;
}

int has_sub_complete(int * cur_choose, int cur_len){
	int i;
	for(i = 0; i < complete_len; i++){
		if(include_comp(complete[i], cur_choose, cur_len))
			return 1;
	}
	return 0;
}

int cnm(int n, int m){
	int i, res = 1;
	// n!/(m!(n-m)!)
	if(n - m < m)
		return cnm(n, n - m);
	for(i = n - m + 1; i <= n; i++)
		res *= i;
	for(i = 2; i <= m; i++)
		res /= i;
	return res;
}


// calculate C(n,m), not used
int C_n_m(int n, int m, unsigned char* res){
    int* p;
    int j, k = 0, ind, flag;
    p = (int *)malloc(sizeof(int) * m);
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
            // check if their sub set is already complete, if yes jump 
            flag = has_sub_complete(p, m);
            if(!flag){
            	for(j = 0; j < m; j++){
            	    res[k++] = p[j];
            	}
        	}
            p[ind]++;
        }
        else{
            ind++;
            p[ind]=p[ind-1]+1;
        }
    }
    free(p);
    return k;
}

void print_complete(){
	int i,j,k,tmp;
	for(i = 0; i < complete_len; i++){
		printf("[%d]: ", i + 1);
		for(j = 0; j < complete[i].len; j++){
			tmp = complete[i].inds[j];
			//printf("(%d)%s ", tmp, cands[tmp].ident);
			printf("%s %d ", cands[tmp].ident, cands[tmp].args_count);
			for(k = 0; k < cands[tmp].bits_count; k++)
				printf("%d",cands[tmp].bits[k]);
			printf(" ");
		}
		printf("\n");
	}
}

// check complete by violence
int check_complete(){
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
                if(m > 0 && all_less(base_n, m, pre_comp_ptr))
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
                // if m == 0, res_ind = 0000
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
    //if(comp_ptr >= 16) printf("--Complete!\n");
    //else printf("--Not Complete!\n");
	if(comp_ptr >= 16)
		for(i = 0; i < comp_ptr; i++){
	        printf("[%d] ",i);
	        print_base2(comp[i]);
	        printf(" %s\n", formula_str[i]);
	    }
    free(base_n);
    for(i = 0; i < comp_ptr; i++){
        free(formula_str[i]);
    }
    if(comp_ptr >= 16) return 1;
    else return 0;
}

int main(){
	int i, j, k, m, is_comp, cnt, tmp, valid;
	unsigned char* CNM;
	int* cur_choose;
	freopen("out.txt", "w", stdout); 
	init_cands();
	// N choose m func to check complete set
	for (m = 1; m < N; m++){
		//printf("Choose Num: %d\n", m);
		cnt = cnm(N, m); // num of poissible situation
		CNM = (unsigned char *)malloc(sizeof(unsigned char) * cnt * m);
		cur_choose = (int*)malloc(sizeof(int) * m);
		valid = C_n_m(N, m, CNM); // all situation 
		//printf("all: %d handle: %d\n", cnt, valid / m);
		for(i = 0; i < valid; i = i + m){
			//printf("Choose: ");
			for(j = 0; j < m; j++){
				cur_choose[j] = CNM[i + j];
			//	printf("%d ", cur_choose[j]);
			}
			//printf("\n");		
			// copy the func to the funcTab
			funcTab_len = 0;
			for(j = 0; j < m; j++){
				funcTabs[funcTab_len++] = cands[cur_choose[j]];
			}			
			// check if the complete and add the flag
			is_comp = check_complete();
			if(is_comp){
				//print_func_tab();
				complete[complete_len].len = m;
				complete[complete_len].inds = (int *)malloc(sizeof(int) * m);
				for(j = 0; j < m; j++)
					complete[complete_len].inds[j] = cur_choose[j];
				complete_len++; 
				printf("--------------------------------------\n");
			}
		}
		free(cur_choose);		
		free(CNM);
	}
	print_complete();
	return 0;
} 
