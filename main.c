#include<stdio.h>
#include<wchar.h>
#include<stdarg.h>
#include<string.h>
#include<locale.h>
#include<math.h>
#include "common.h"

int main(){
    int i, j, cnt, tmp, all0 = 1, all1 = 1;
    int* res;
    setup_file();
    /*do{
        fetch_token(2);
        print_token(token);
    }while(token != token_end);
    */
    scan_passage();
    //print_var_tab();
	//print_func_tab();
    //print_pcodes();
    check_undefined();
    if(error > 0)
        printf("Error: %d\n", error);
    if(error == 0){
    	if(pcode_len > 0){
	        //printf("Interpret...\n");
	        cnt = (int)pow(2, varTab_len);
	        res = (int*)malloc(sizeof(int) * cnt);
	        for(i = 0; i < cnt; i++){
	            assign_var(i);
	            tmp = interpret();
	            if(tmp == 0) all1 = 0;
	            if(tmp == 1) all0 = 0;
	            res[i] = tmp;
	        }
	        if(all0)
	            printf("--Always False!\n");
	        else if(all1)
	            printf("--Always True!\n");
	        else{
	            printf("--Not Always True or Always False!\n");
	            for(i = 0; i < varTab_len; i++)
	                printf("%s ", varTabs[i].ident);
	            printf(" Res\n");
	            for(i = 0; i < cnt; i++){
	                assign_var(i);
	                for(j = 0; j < varTab_len; j++)
	                    printf("%d ", varTabs[j].val);
	                printf(" %d\n", res[i]);
	            }
	        }
		}
		if(funcTab_len > 0)
        	check_complete();

    }
    return 0;
}

// functions for token
// setup the input file pointer, Fptr
void setup_file(){
    wchar_t src[MAX_PATH_LEN];
    wcscpy(src, L"test.txt");
    if(!(Fptr = _wfopen( src, L"rt+, ccs=UNICODE")))
        exit(1);
    error = 0;
    ch = ' ';
    return;
}

// assign ch one by one, end of the file return '\0'
void fetch_char(){
    token_len++;
    if(col_ind >= row_len){
        col_ind = 0;
        row_ind++;
        if(!(fgetws(row_val, MAX_ROW_LEN, Fptr))){
            ch ='\0';
            return;
        }
        row_len = wcslen(row_val);
    }
    ch = row_val[col_ind++];
    return;
}

// if a char is A-Za-z
int is_letter(wchar_t ch){
    return ((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z'));
}

// if a char is 0-9
int is_number(wchar_t ch){
    return (ch >= '0' && ch <= '9');
}

/*  assign token one by one, end of the file with token_end
    read number type:
        0 - const: 0,1
        1 - int: 123, 24, ...
        2 - bits: 0101, 11001100, ...
*/
void fetch_token(int read_number_type){
    int k = 0;
    int error_flag = 0;
    token_len = 0;
    while(ch == ' ' || ch == '\n' || ch == '\t')
        fetch_char();

    if(ch == '\0')
        token = token_end;
    // identifer, variable or function name
    else if(is_letter(ch)){
        do{
            if(k < MAX_IDENT_LEN)
                val_ident[k++] = (char)ch;
            else
                error_flag = 1;
            fetch_char();
        }while(is_letter(ch) || is_number(ch));
        val_ident[k] = '\0';
        if(error_flag)
            print_error("Token-Error: Lenth of identify > 10!");
        token = token_ident;
    }
    // start with 0-9
    else if(is_number(ch)){
        // read number by const value: 0 or 1
        if(read_number_type == 0){
            token = token_const;
            val_const = (ch == '1') ? 1 : 0;
            if(ch >= '2')
                error_flag = 1;
            fetch_char();
            while(is_number(ch)){
                error_flag = 1;
                fetch_char();
            }
            if(error_flag)
                print_error("Token-Error: read number by const type and number should be 0 or 1!");
        }
        // read number by int value: 0 - MAX_LOGIC_LEN(10)
        else if(read_number_type == 1){
            token = token_int;
            val_int = ch - '0';
            fetch_char();
            while(is_number(ch)){
                if(val_int == 0)
                    error_flag = 1;
                val_int = val_int * 10 + (ch - '0');
                fetch_char();
            }
            if(error_flag)
                print_error("Token-Error: read number by int type and nonzero number begin with 0!");
        }
        // read number by bits value: 01001000
        else{
            token = token_bits;
            while(is_number(ch)){
                if(ch != '0' && ch != '1')
                    error_flag = 1;
                else if(k < MAX_BITS_LEN)
                    val_bits[k++] = ch - '0';
                else
                    error_flag = 2;
                fetch_char();
            }
            len_bits = k;
            if(error_flag == 2)
                print_error("Token-Error: bits len out of max!");
            if(error_flag == 1)
                print_error("Token-Error: read number by bits type and number should be only 0 or 1!");
        }
    }
    else if(ch == '%'){
        while(ch != '\n' && ch != '\0')
            fetch_char();
        fetch_token(read_number_type);
    }
    else if(ch == '('){
        token = token_paren_open; fetch_char();
    }
    else if(ch == ')'){
        token = token_paren_close; fetch_char();
    }
    else if(ch == ','){
        token = token_comma; fetch_char();
    }
    else if(ch == '#'){
        token = token_pound; fetch_char();
    }
    else if(ch == 0x00ac){  //¬
        token = token_neg; fetch_char();
    }
    else if(ch == 0x2227){  //∧
        token = token_conjunc; fetch_char();
    }
    else if(ch == 0x2228){  //∨
        token = token_disjunc; fetch_char();
    }
    else if(ch == 0x2192){  //→
        token = token_implica; fetch_char();
    }
    else if(ch == 0x2194){  //↔
        token = token_equival; fetch_char();
    }
    else if(ch == 0x2295){  //⊕
        token = token_xor; fetch_char();
    }
    // note
    else{
    	printf("val: %x\n", ch);
        print_error("Token-Error: Character unexpected!");
        fetch_char();
        fetch_token(read_number_type);
    }
    return;
}

// print the token
void print_token(Token t){
    int i;
    char *s[] = {"token_paren_open", "token_paren_close", "token_comma", "token_pound", "token_neg", "token_conjunc", "token_disjunc", "token_implica", "token_equival", "token_xor", "token_const", "token_ident", "token_int", "token_bits", "token_end"};
    printf("%s", s[t]);
    if(t == token_const)
        printf("(%d)", val_const);
    else if(t == token_ident)
        printf("(%s)", val_ident);
    else if(t == token_int)
        printf("(%d)", val_int);
    else if(t == token_bits){
        printf("(%d-", len_bits);
        for(i = 0; i < len_bits; i++)
            printf("%d", val_bits[i]);
        printf(")");
    }
    printf("\n");
    return;
}

// functions for syntax
// if a token in an array of tokens
int inside_tokens(Token t, Token ts[], int n){
    int i;
    for(i = 0; i < n; i++)
        if(t == ts[i])
            return 1;
    return 0;
}

// jump read the token while syntax errror appears
void jump_until(int n, ...){
    int i, k=0;
    va_list arg_ptr;
    Token ts[50];
    // unknown number of args
    va_start(arg_ptr, n);
    for(i = 0; i < n; i++)
        ts[k++] = va_arg(arg_ptr, Token);
    va_end(arg_ptr);
    while(token != token_end &&
          (!inside_tokens(token, ts, n)))
        fetch_token(0);
    return;
}

/* factor syntax analysis
    const number: <0, 1>
    variable: <identifier[type=0]>
    func_call: <identifier[type=1](experssion, ...)>
    <(expression)>
*/
void factor(){
    char * tmp;
    int args_num, ind_var, ind_func;
    // const
    if(token == token_const){
        // push_const 0/1
        gen_pcode(opt_push_const, val_const);
        fetch_token(0);
    }
    // variable or func_call
    else if(token == token_ident){
        tmp = (char*)malloc(sizeof(val_ident));
        strcpy(tmp, val_ident);
        fetch_token(0);
        // function call
        if(token == token_paren_open){
            args_num = function_call();
            ind_var = lookup_var_tab(tmp);
            ind_func = lookup_func_tab(tmp);
            if(ind_var >= 0){
                sprintf(error_info, "Semantics-Error: Variable %s cannot be used as function!", tmp);
                print_error(error_info);
            }
            else{
                if(ind_func < 0)
                    ind_func = insert_func_tab(tmp, args_num);

                if(funcTabs[ind_func].args_count != args_num){
                    sprintf(error_info, "Semantics-Error: Function %s args calling are not consistent: %d vs %d !",
                            tmp, funcTabs[ind_func].args_count, args_num);
                    print_error(error_info);
                }
            }
            gen_pcode(opt_call_func, ind_func);
        }
        // variable
        else{
            ind_func = lookup_func_tab(tmp);
            ind_var = lookup_var_tab(tmp);
            if(ind_func >= 0){
                sprintf(error_info, "Semantics-Error: Function %s cannot be used as variable!", tmp);
                print_error(error_info);
            }
            else if(ind_var < 0)
                ind_var = insert_var_tab(tmp);
            gen_pcode(opt_push_var, ind_var);
        }
    }
    //(expression) 
    else if(token == token_paren_open){
        fetch_token(0);
        expression();
        if(token != token_paren_close){
            print_error("Syntax-Error: Paren not matched!");
            jump_until(3, token_pound, token_paren_close, token_end);
        }
        if(token == token_paren_close)
            fetch_token(0);
    }
    else if(token != token_pound){
        print_error("Syntax-Error: Factor should begin with const, ident or paren_open!");
    }
    return;
}

// priority: neg > conjunc > disjunc > xor > implica > equival 
//	term_neg syntax analysis: {neg} factor()
void term_neg(){
    int i = 0;
    while(token == token_neg){
        i++;
        fetch_token(0);
    }
    factor();
    if(i % 2 == 1)
        gen_pcode(opt_neg, 0);
    return;
}

// term_conjunc syntax analysis: term_neg(){ conjunc term_neg()}
void term_conjunc(){
	term_neg();
	while(token == token_conjunc){
		fetch_token(0);
		term_neg();
		gen_pcode(opt_conjunc, 0);
	}
	return;
}

// term_disjunc syntax analysis: term_conjunc(){ disjunc term_conjunc()}
void term_disjunc(){
	term_conjunc();
	while(token == token_disjunc){
		fetch_token(0);
		term_conjunc();
		gen_pcode(opt_disjunc, 0);
	}
	return;
}

// term_xor syntax analysis: term_disjunc(){ xor term_disjunc()}
void term_xor(){
	term_disjunc();
	while(token == token_xor){
		fetch_token(0);
		term_disjunc();
		gen_pcode(opt_xor, 0);
	}
	return;
}

// term_implica syntax analysis: term_xor(){ implica term_xor()}
void term_implica(){
	term_xor();
	while(token == token_implica){
		fetch_token(0);
		term_xor();
		gen_pcode(opt_implica, 0);
	}
	return;
}

// expression syntax analysis: term_implica(){ equival term_implica}
void expression(){
	term_implica();
	while(token == token_equival){
		fetch_token(0);
		term_implica();
		gen_pcode(opt_equival, 0);
	}
	return;
}

/* declare function
    # func_name func_args_count bits {func_name func_args_count bits}
*/
void function_declaration(){
    int tmp_args_cnt, tmp_bits_cnt, ind_var, ind_func;
    char *tmp;
    fetch_token(0);
    while(token != token_end){
	    if(token != token_ident){
	        print_error("Syntax-Error: Identifier should begin with ident!");
	        jump_until(2, token_end, token_ident);
	        continue;
	    }
	    tmp = (char*)malloc(sizeof(val_ident));
	    strcpy(tmp, val_ident);
	    fetch_token(1);
	    if(token != token_int){
	        print_error("Syntax-Error: args number should behind the function name!");
	        jump_until(2, token_end, token_ident);
	        continue;
	    }
	    tmp_args_cnt = val_int;
	    fetch_token(2);
	    if(token != token_bits){
	        print_error("Syntax-Error: bits number should behind the function args number!");
	        jump_until(2, token_end, token_ident);
	        continue;
	    }
	    tmp_bits_cnt = (int)pow(2, tmp_args_cnt);
	    if(tmp_bits_cnt != len_bits){
	        sprintf(error_info, "Semantics-Error: Function %s bits define wrong, require %d but given %d !",
	                 tmp, tmp_bits_cnt, len_bits);
	        print_error(error_info);
	    }
	    ind_var = lookup_var_tab(tmp);
	    if(ind_var >= 0){
	        sprintf(error_info, "Semantics-Error: Variable %s cannot be defined as function!", tmp);
	        print_error(error_info);
	    }
	    else{
	        ind_func = lookup_func_tab(tmp);
	        if(ind_func < 0){
	            printf("Warning: Function %s defined but not be used!\n", tmp);
	            ind_func = insert_func_tab(tmp, tmp_args_cnt);
	        }
	        if(tmp_args_cnt != funcTabs[ind_func].args_count){
	            sprintf(error_info, "Semantics-Error: Function %s args define wrong, calling %d but defining %d!",
	                     tmp, funcTabs[ind_func].args_count, tmp_args_cnt);
	            print_error(error_info);
	        }
	        complete_func_tab(ind_func, val_bits, len_bits);
	    }
	    fetch_token(0);
	}
    return;
}

/* function call
    function_name( expression, expression, ...)
    return number of args
*/
int function_call(){
    int args_num = 0;
    fetch_token(0);
    // f()
    if(token == token_paren_close){
        fetch_token(0);
        return 0;
    }
    while(1){
        expression();
        args_num++;
        if(token == token_paren_close){
           fetch_token(0);
           break;
        }
        if(token != token_comma){
            print_error("Syntax-Error: Function args table format error!");
            jump_until(4, token_comma, token_paren_close, token_pound, token_end);
        }
        if(token != token_comma)
            break;
        fetch_token(0);
    }
    return args_num;
}

/* scan all passage
    expression()
    {function_declaration()}
*/
void scan_passage(){
    fetch_token(0);
    expression();
    /*if(token != token_pound){
        print_error("Syntax-Error: Unexpected token behind the expression!");
        jump_until(1, token_pound);
    }*/
    if((token != token_end) && (token != token_pound)){
    	print_error("Syntax-Error: redundant structure between expression tail and function define head!");
    }
    while(token == token_pound){
        function_declaration();
    }
    return;
}

// functions for semantics
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

// functions for interpret
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

// functions for complete
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

// functions for errors
void print_error(char * s){
    int i;
    error++;
    printf("Error[%d] in line [%d] column [%d]:\n", error, row_ind, col_ind);
    setlocale(LC_CTYPE, "");
    wprintf(L"\t%ls", row_val);
    //setlocale(LC_CTYPE, "C");
    //for(i = 0; i < col_ind - token_len - 1; i++)
    //    wprintf(L" ");
    printf("\t%s\n", s);
    return;
}
