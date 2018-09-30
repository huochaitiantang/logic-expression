#define MAX_IDENT_LEN 10
#define MAX_BITS_LEN 1024
#define MAX_PATH_LEN 512
#define MAX_ROW_LEN 512

typedef enum{
    token_paren_open,   // (
    token_paren_close,  // )
    token_comma,        // ,
    token_pound         // #

    token_neg           // 
    token_conjunc       //
    token_disjunc       //
    token_implica       //
    token_equival       //
    token_xor           //

    token_const         // 0, 1, corresponding to val_const
    token_ident         // abcd, correspondint to val_ident
    token_int           //  123, corresponding to val_int
    token_bits          // 0011, corresponding to val_bits

    token_end           // end of the file

} Token;

FILE * Fptr;
int  col_ind;
int  row_len;
int  val_const;
int  val_int;
int* val_bits;
char val_ident[MAX_IDENT_LEN + 1]; // max length of ident is 10
char row_val[MAX_ROW_LEN + 1];
char ch = ' ';  // cur char value
Token token;    // cur token value

void setup_file();
void fetch_char();
void fetch_token();
void print_token(Token t);


