#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>

typedef struct{
    char* ident;
    int args_count;
    int bits_count;
    int* bits;
} FuncTab;

typedef struct{
	int len;
	int* inds;
} Complete;

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
int check_complete(FuncTab* cands, int cands_len, int* chooses, int chooses_len){
    int i,j,k,l,m,n;
    int flag[16];
    int comp[16];
    int res_ind[4];
    int base_n[10];
    int res, base, cnt, len, add_new_flag;
    int comp_ptr = 0, pre_comp_ptr = 0;   
	FuncTab tmp_func;
    //printf("Judge if complete set...\n");
    for(i = 0; i < 16; i++)
        flag[i] = 0;
    // init comp with p(0011), q(0101)
    comp[comp_ptr++] = 3;
    comp[comp_ptr++] = 5;
    flag[3] = 1;
    flag[5] = 1;
    //judge
    while(1){
        add_new_flag = 0;
        n = comp_ptr;
        for(i = 0; i < chooses_len; i++){
            // choose m as func args from n existed result
            tmp_func = cands[chooses[i]];
            m = tmp_func.args_count;
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
                    res += (base * tmp_func.bits[res_ind[k]]);
                    base = base << 1;
                }
                // add to the existed array
                if(flag[res] == 0){
                    comp[comp_ptr] = res;
                    flag[res] = 1;
                    add_new_flag = 1;
					comp_ptr++;
                }
            }
        }
        pre_comp_ptr = n;
        //print_comp(comp, comp_ptr);
        //printf("pre:%d ptr:%d\n", pre_comp_ptr, comp_ptr);
        if(add_new_flag == 0 || comp_ptr >= 16)
            break;
    }
    if(comp_ptr >= 16) return 1;
    else return 0;
}

// init the candidate functions 
void init_cands(FuncTab* cands, int cands_len, int start_args_cnt){
	int i, j, args_cnt, bits_cnt, func_cnt;
	args_cnt = start_args_cnt;
	bits_cnt = (int)(pow(2, args_cnt) + 0.5);
	func_cnt = (int)(pow(2, bits_cnt) + 0.5);
	for(i = 0, j = 0; i < cands_len; i++){
		// function name f1,f2,f3,...	
		cands[i].ident = (char*)malloc(sizeof(char)*10);
		sprintf(cands[i].ident, "f%d", i);
		// complete the value of the function tables
		cands[i].args_count = args_cnt;
		cands[i].bits_count = bits_cnt;
		cands[i].bits = (int *)malloc(sizeof(int) * bits_cnt);
		to_base_n(j, 2, bits_cnt, cands[i].bits);
		j++;
		if(j >= func_cnt){
			args_cnt++;
			bits_cnt *= 2;
			func_cnt = (int)(pow(2, bits_cnt) + 0.5);
			j = 0;
		}
	}
	return;
}

void print_cands(FuncTab* cands, int cands_len){
	int i, j, args_cnt, bits_cnt;
	printf("-----------CandTabs------------\n");
    for(i = 0; i < cands_len; i++){
        args_cnt = cands[i].args_count;
        bits_cnt = cands[i].bits_count;
        printf("%d %s %d ", i, cands[i].ident, args_cnt);
        for(j = 0; j < bits_cnt; j++)
            printf("%d", cands[i].bits[j]);
        printf("\n");
    }
    printf("-------------------------------\n");
}


int inside_array(int x, int len, int* array){
	int i;
	for(i = 0; i < len; i++){
		if(array[i] == x)
			return 1;
	}
	return 0;
}

void get_one_complete_set_and_delete(FuncTab* cands, int* cands_len){
    int i, j, tmp, cnt = 0;
    int* flag;
    FILE* fptr;
    int* p;
    flag = (int*)malloc(sizeof(int) * (*cands_len));
    p = (int*)malloc(sizeof(int) * 1);
    fptr = fopen("out.txt", "a");
    // get complete set
    for(i = 0; i < (*cands_len); i++){
        p[0] = i;
        if(check_complete(cands, (*cands_len), p, 1)){
            flag[cnt++] = i;
            fprintf(fptr, " %s %d ", cands[i].ident, cands[i].args_count);
            for(j = 0; j < cands[i].bits_count; j++)
                fprintf(fptr, "%d", cands[i].bits[j]);
            fprintf(fptr, "\n");
        }
    }
    // delete one element in the candiate set
    for(i = cnt - 1; i >= 0; i--){
        tmp = flag[i];
		free(cands[tmp].ident);
		free(cands[tmp].bits);
        for(j = tmp + 1; j < (*cands_len); j++)
            cands[j - 1] = cands[j];
        *cands_len = *cands_len - 1;
    }
    fclose(fptr);
    free(p);
    free(flag);
    return;
}


// get the 0,1,2,...m-1, and which complete set it has been in
void get_index_complete(int m, Complete* complete, int complete_len, Complete* index_complete, int* complete_flag){
    int i, j, cnt;
    for(i = 0; i < complete_len; i++)
        complete_flag[i] = 0;
    for(i = 0; i < m; i++){
        index_complete[i].len = 0;
        index_complete[i].inds = NULL;
    }
    for(i = 0; i < m; i++){
        cnt = 0;
		for(j = 0; j < complete_len; j++){
            if(inside_array(i, complete[j].len, complete[j].inds))
                cnt++;
        }
        if(cnt > 0){
            index_complete[i].len = cnt;
            index_complete[i].inds = (int*)malloc(sizeof(int) * cnt);
            cnt = 0;
            for(j = 0; j < complete_len; j++){
                if(inside_array(i, complete[j].len, complete[j].inds))
                    index_complete[i].inds[cnt++] = j;
            }
        }
    }
    return;
}

// check if a number can add the cur choose
int can_satisfy(int x, Complete* index_complete, int* complete_flag, Complete* complete){
	int i, tmp;
	Complete tmp_comp;
	tmp_comp = index_complete[x];
	for(i = 0; i < tmp_comp.len; i++){
		tmp = tmp_comp.inds[i];
		if(complete_flag[tmp] + 1 >= complete[tmp].len){
			//printf("Not Satisfy: %d conflict with:%d\n", x, tmp);
			return 0;
		}
	}
	for(i = 0; i < tmp_comp.len; i++){
		tmp = tmp_comp.inds[i];
		complete_flag[tmp]++;
	}
	return 1;
}

double cal_cnm(int n, int m){
	double x = 1;
	int i;
	for(i = n - m + 1; i <= n; i++)
		x *= i;
	for(i = 2; i <= m; i++)
		x /= i;
	return x;
}


// calculate members of C(n,m), res as a array
int get_complete_C_n_m(int n, int m, Complete* complete, int complete_len, FuncTab* cands, int cands_len){
    int* p;
    int i, j;
	int ind, is_complete, origin_complete_len;
    int * complete_flag;
    double cnm;
	long long cnt = 0;
    Complete* index_complete;
    Complete tmp_comp;
	FuncTab tmp_func;
	FILE *fptr;
    
    cnm = cal_cnm(n, m);
    origin_complete_len = complete_len;
    complete_flag = (int*)malloc(sizeof(int) * complete_len);
    index_complete = (Complete*)malloc(sizeof(Complete) * n);
    p = (int *)malloc(sizeof(int) * m);

    // initial with the first number
    get_index_complete(n, complete, complete_len, index_complete, complete_flag);
    ind = 0;
    p[ind] = 0;
    for(i = 0; i < index_complete[0].len; i++)
        complete_flag[index_complete[0].inds[i]]++;
    while(1){
        if(p[ind] >= n){
            if(ind == 0) break;
            // sub the complete_flag
			ind--;
            tmp_comp = index_complete[p[ind]];
            for(i = 0; i < tmp_comp.len; i++){
            	complete_flag[tmp_comp.inds[i]]--;
            }
            p[ind]++;
            while(p[ind] < n && !can_satisfy(p[ind], index_complete, complete_flag, complete))
            	p[ind]++;
        }
        else if(ind == m - 1){
            cnt += 1;
			if(cnt % 10000 == 0){
				printf("[%lld/%.0lf] complete_len: %d\n[%d]choose: ", cnt, cnm, complete_len, m);
				for(i = 0; i < m; i++)
					printf("%d ", p[i]);
				printf("\n");
			}
            is_complete = check_complete(cands, cands_len, p, m);	
			if(is_complete){
				// save the complete result
				complete[complete_len].len = m;
				complete[complete_len].inds = (int*)malloc(sizeof(int) * m);
				fptr = fopen("out.txt", "a");
				//fprintf(fptr, "[%d]", complete_len);
				for(i = 0; i < m; i++){
					complete[complete_len].inds[i] = p[i];
					tmp_func = cands[p[i]];
					fprintf(fptr, " %s %d ", tmp_func.ident, tmp_func.args_count);
					for(j = 0; j < tmp_func.bits_count; j++)
						fprintf(fptr, "%d", tmp_func.bits[j]);
				}
				fprintf(fptr, "\n");
				fclose(fptr);
				complete_len++; 
			}
 
 			// sub the complete flag
			tmp_comp = index_complete[p[ind]];
            for(i = 0; i < tmp_comp.len; i++){
            	complete_flag[tmp_comp.inds[i]]--;
            }
            p[ind]++;
            while(p[ind] < n && !can_satisfy(p[ind], index_complete, complete_flag, complete))
            	p[ind]++;
        }
        else{
            ind++;
            p[ind] = p[ind-1] + 1;
            while(p[ind] < n && !can_satisfy(p[ind], index_complete, complete_flag, complete))
            	p[ind]++;
        }
    }
    printf("Choose %d from %d: valid: %lld / %.0lf (ratio = %.3lf)\n", m, n, cnt, cnm, cnt/cnm);
    
    // free space
    for(i = 0; i < n; i++){
        if(index_complete[i].inds != NULL)
            free(index_complete[i].inds);
    }
    free(index_complete);
    free(complete_flag);
    free(p);
    return complete_len;
}


int get_all_complete(Complete* complete, FuncTab* cands, int* cands_len){
	int i;
	int complete_len = 0;
	FILE * fptr;
	
	fptr = fopen("out.txt", "w");
	fclose(fptr);
	
    // find all 1 function complete set and get rid of it
    get_one_complete_set_and_delete(cands, cands_len);
    
	for(i = 2; i < *cands_len; i++){
		//printf("Choose %d from %d\n", i, *cands_len);
		complete_len = get_complete_C_n_m(*cands_len, i, complete, complete_len, cands, *cands_len);
		//printf("cur complete: %d\n", complete_len);
	}

	return complete_len;
}

// print the result
void print_complete(Complete* complete, int complete_len, FuncTab* cands){
	int i,j,k,tmp;
	for(i = 0; i < complete_len; i++){
		printf("[%d]: ", i + 1);
		for(j = 0; j < complete[i].len; j++){
			tmp = complete[i].inds[j];
			printf("%s %d ", cands[tmp].ident, cands[tmp].args_count);
			for(k = 0; k < cands[tmp].bits_count; k++)
				printf("%d",cands[tmp].bits[k]);
			printf(" ");
		}
		printf("\n");
	}
}


int main(){
	int i;
	int complete_len;
	int cands_len = 256;
    int start_args_cnt = 3;
	
	FuncTab* cands;
    Complete complete[10000];
	clock_t start, end; 
	
	start = clock();
	//freopen("out_log.txt", "w", stdout); 
	cands = (FuncTab*)malloc(sizeof(FuncTab) * cands_len);
	init_cands(cands, cands_len, start_args_cnt);
	print_cands(cands, cands_len);

	complete_len = get_all_complete(complete, cands, &cands_len);
    //print_cands(cands, cands_len);
    //print_complete(complete, complete_len, cands);
	
	// free space
	for(i = 0; i < cands_len; i++){
		free(cands[i].ident);
		free(cands[i].bits);
	}
	for(i = 0; i < complete_len; i++){
		free(complete[i].inds);
	}
	free(cands);
	
	end = clock();
	printf("Cost: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	return 0;
} 
