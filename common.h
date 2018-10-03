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


typedef struct{
    char* ident;
    int val;
} VarTab;
int varTab_len;
VarTab varTabs[MAX_VARTAB_LEN]; // Variable Tables


typedef struct{
    char* ident;
    int args_count;
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
