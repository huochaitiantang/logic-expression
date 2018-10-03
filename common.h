#define MAX_IDENT_LEN 10
#define MAX_LOGIC_LEN 10
#define MAX_BITS_LEN 1024
#define MAX_PATH_LEN 512
#define MAX_ROW_LEN 512
#define MAX_PCODE_LEN 1024
#define MAX_VARTAB_LEN 64
#define MAX_FUNCTAB_LEN 64

typedef enum{
    token_paren_open,   // (
    token_paren_close,  // )
    token_comma,        // ,
    token_pound,        // #

    token_neg,          //
    token_conjunc,      //
    token_disjunc,      //
    token_implica,      //
    token_equival,      //
    token_xor,          //

    token_const,        // 0, 1, corresponding to val_const
    token_ident,        // abcd, correspondint to val_ident
    token_int,          //  123, corresponding to val_int
    token_bits,         // 0011, corresponding to val_bits

    token_end           // end of the file

} Token;

FILE * Fptr;
int  col_ind;
int  row_ind;
int  row_len;
int  val_const;
int  val_int;
int  val_bits[MAX_BITS_LEN];
int  len_bits;
char val_ident[MAX_IDENT_LEN + 1];  // max length of ident is 10
wchar_t row_val[MAX_ROW_LEN + 1];      // cur row string value
wchar_t  ch;                      // cur char value
Token token;                        // cur token value
int  error;
int  token_len;


typedef struct{
    char* ident,
    int val,
} VarTab;
VarTab varTabs[MAX_VARTAB_LEN];


typedef struct{
    char* ident,
    int args_count,
    int* bits
} FuncTab;
FuncTab funcTabs[MAX_FUNCTAB_LEN];


typedef enum{
    opt_push_const,
    opt_push_var,
    opt_call_func,
    opt_neg,
    opt_and,
    opt_or,
    opt_xor,
    opt_equal,
    opt_refer
} Opt;

typedef struct{
    Opt opt,
    char * ident,
    int val
} PCode;


int  pcode_len;
PCode PCodes[MAX_PCODE_LEN];

// function in token.c
void setup_file();
void fetch_char();
void fetch_token(int read_number_type);
void print_token(Token t);

// function in error.c
void print_error(char* s);


//function in syntax.c
void scan_passage();
