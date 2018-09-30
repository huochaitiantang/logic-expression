#include "common.h"


// setup the input file pointer, Fptr
void setup_file(){
    char src[MAX_PATH_LEN];
    printf("Please input path whose length should < %d", MAX_PATH_LEN);
    scanf("%s", src);
    //strcpy(src, "test.txt");
    while(!(Fptr=fopen(src, "r"))){
        printf("Cannot open the file %s\n", src);
        printf("Please input path whose length should < %d", MAX_PATH_LEN);
        scanf("%s", src);
    }
    return 0;
}


// assign ch one by one, end of the file return '\0'
void fetch_char(){
    if(col_ind >= row_len){
        col_ind = 0;
        if(!(fgets(row_val, MAX_ROW_LEN, Fptr))){
            ch ='\0';
            return;
        }
        row_len = strlen(row_val);
    }
    ch = row_val[col_ind++];
    return;
}


// if a char is A-Za-z
int is_letter(char ch){
    return ((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z'));
}


// if a char is 0-9
int is_number(char ch){
    return (ch >= '0' && ch <= '9');
}

// assign token one by one, end of the file with token_end
void fetch_token(){
    int k = 0;
    int out_flag = 0;

    while(ch == ' ' || ch == '\n' || ch == '\n')
        fetch_char();
    
    if(ch == '\0') token = token_end;
    else if(ch == '('){
        token = token_paren_open;
        fetch_char();
    }
    else if(ch == ')'){
        token = token_paren_close;
        fetch_char();
    }
    else if(ch == ','){
        token = token_comma;
        fetch_char();
    }
    else if(ch == '#'){
        token = token_pound;
        fetch_char();
    }
    else if(ch == 'fei'){
        token = token_neg;
        fetch_char();
    }
    else if(ch == 'hequ'){
        token = token_conjunc;
        fetch_char();
    }
    else if(ch == 'xiqu'){
        token = token_disjunc;
        fetch_char();
    }
    else if(ch == 'yunhan'){
        token = token_implica;
        fetch_char();
    }
    else if(ch == 'dengjia'){
        token = token_equival;
        fetch_char();
    }
    else if(ch == 'yihuo'){
        token = token_xor;
        fetch_char();
    }
    // identifer, variable or function name
    else if(is_letter(ch)){
        do{
            val_ident[k++] = ch;
            if(k < MAX_IDENT_LEN)
                val_ident[k++] = ch;
            else
                out_flag = 1;
            fetch_char();
        }while(is_letter(ch) || is_number(ch));
        val_ident[k] = '\0';
        if(out_flag)
            printf("Error: Lenth of identify > 10!");
        token = token_ident;
    }
    else if(ch >= '2' && ch <= '9'){
        val_int = ch - '0';
        fetch_char();
        while(is_number(ch)){
            val_int = val_int * 10 + (ch - '0');
            fetch_char();
        }
        token = token_int;
    }
    // should read by three types: const, int, bits
    else if(ch == '0'){
        printf("Error: Number should not begin with 0!");


    }
    else if(ch == '1'){

    }
    else{
        printf("Error: Character unexpected: %c!", ch);
        fetch_char();
    }



}


// print the token
void print_token(Token t){


}
