#include <stdio.h>
#include<string.h>
#include "common.h"


// setup the input file pointer, Fptr
void setup_file(){
    char src[MAX_PATH_LEN];
    printf("Please input path whose length should < %d:\n", MAX_PATH_LEN);
    //scanf("%s", src);
    strcpy(src, "test.txt");
    while(!(Fptr=fopen(src, "r"))){
        printf("Cannot open the file %s!\n", src);
        printf("Please reinput path whose length should < %d:\n", MAX_PATH_LEN);
        scanf("%s", src);
    }
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
    while(ch == ' ' || ch == '\n' || ch == '\n')
        fetch_char();
    
    if(ch == '\0')
        token = token_end;
    // identifer, variable or function name
    else if(is_letter(ch)){
        do{
            if(k < MAX_IDENT_LEN)
                val_ident[k++] = ch;
            else
                error_flag = 1;
            fetch_char();
        }while(is_letter(ch) || is_number(ch));
        val_ident[k] = '\0';
        if(error_flag)
            print_error("Error: Lenth of identify > 10!");
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
                print_error("Error: read number by const type and number should be 0 or 1!");
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
                print_error("Error: read number by int type and nonzero number begin with 0!");
        }
        // read number by bits value: 01001000
        else{
            token = token_bits;
            while(is_number(ch)){
                if(ch != '0' && ch != '1')
                    error_flag = 1;
                else
                    val_bits[k++] = ch - '0';
                fetch_char();
            }
            len_bits = k;
            if(error_flag)
                print_error("Error: read number by bits type and number should be only 0 or 1!");
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
    else if(ch == '!'){
        token = token_neg; fetch_char();
    }
    else if(ch == '&'){
        token = token_conjunc; fetch_char();
    }
    else if(ch == '|'){
        token = token_disjunc; fetch_char();
    }
    else if(ch == '-'){
        token = token_implica; fetch_char();
    }
    else if(ch == '='){
        token = token_equival; fetch_char();
    }
    else if(ch == '^'){
        token = token_xor; fetch_char();
    }
    // note
    else{
        //¬ → ↔ ∧ ∨ ⊕

        //0x00ac,0x2192,0x2194,0x2227,0x2228,0x2295

        printf("%d\n", ch); 
        print_error("Error: Character unexpected!");
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
