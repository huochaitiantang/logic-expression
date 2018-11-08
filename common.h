#define MAX_IDENT_LEN 10
#define MAX_LOGIC_LEN 10
#define MAX_BITS_LEN 1024
#define MAX_PATH_LEN 512
#define MAX_ROW_LEN 512
#define MAX_PCODE_LEN 1024
#define MAX_VARTAB_LEN 64
#define MAX_FUNCTAB_LEN 64
#define MAX_STACK_LEN 512

typedef enum{
    token_paren_open,   // (
    token_paren_close,  // )
    token_comma,        // ,
    token_pound,        // #
    token_neg,          // ¬
    token_conjunc,      // ∧
    token_disjunc,      // ∨
    token_implica,      // →
    token_equival,      // ↔
    token_xor,          // ⊕
    token_const,        // 0, 1, corresponding to val_const
    token_ident,        // abcd, correspondint to val_ident
    token_int,          //  123, corresponding to val_int
    token_bits,         // 0011, corresponding to val_bits
    token_end           // end of the file
} Token;


FILE * Fptr;
int  col_ind;                       // file column index in cur line
int  row_ind;                       // file line index
int  row_len;                       // file line length
wchar_t row_val[MAX_ROW_LEN + 1];   // cur line string value


int  val_const;                     // cur const value if token is token_const
int  val_int;                       // cur int value if token is token_int
int  len_bits;                      // cur bits length if token is token_bits
int  val_bits[MAX_BITS_LEN];        // cur bits value if token is token_bits
char val_ident[MAX_IDENT_LEN + 1];  // cur identifier str is token is token_ident


wchar_t  ch;        // cur char value
Token token;        // cur token value
int  token_len;     // token lenth
int  error;         // error num
char error_info[1024];


typedef struct{
    char* ident;
    int val;
} VarTab;
int varTab_len;
VarTab varTabs[MAX_VARTAB_LEN]; // Variable Tables


typedef struct{
    char* ident;
    int args_count;
    int bits_count;
    int* bits;
} FuncTab;
int funcTab_len;
FuncTab funcTabs[MAX_FUNCTAB_LEN]; // Functions Tables


typedef enum{
    opt_push_const,     // push a const value, 0 or 1
    opt_push_var,       // push a variable value, index in variable tables
    opt_call_func,      // call function, index in function tables
    opt_neg,
    opt_conjunc,
    opt_disjunc,
    opt_xor,
    opt_equival,
    opt_implica
} Opt;                  // option type of p-code
typedef struct{
    Opt opt;            // option type
    int ind;            // option number
} PCode;
int  pcode_len;
PCode pcodes[MAX_PCODE_LEN];


int stacks[MAX_STACK_LEN];
int top;


// function in token.c
void setup_file();
void fetch_char();
void fetch_token(int read_number_type);
void print_token(Token t);

// function in error.c
void print_error(char* s);

//function in syntax.c
void scan_passage();

// function in semantics.c
int lookup_var_tab(char* ident);
int lookup_func_tab(char* ident);
int insert_var_tab(char* ident);
int insert_func_tab(char* ident, int args_num);
void gen_pcode(Opt opt, int ind);
void print_var_tab();
void print_func_tab();
void print_pcodes();

// function in interpret.c
void assign_var(int index);
int interpret();

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




