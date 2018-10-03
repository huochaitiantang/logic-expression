#include<stdio.h>
#include <stdarg.h>
#include "common.h"


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
    // const
    if(token == token_const){
        // push_const 0/1
    }
    // variable or func_call
    else if(token == token_ident){
        // push_var name


    }
    //(expression)
    else if(token == token_paren_open){
        fetch_token();
        expression();
        if(token != token_paren_close){
            print_error("Syntax-Error: Paren not matched!");
            jump_until(2, token_pound, token_paren_close);
        }
        if(token == token_paren_close)
            fetch_token();
    }
    else{
        print_error("Syntax-Error: Factor should begin with const, ident or paren_open!");
    }
    return;
}


/* term syntax analysis, priority of ¬ > others
    {¬}factor()
*/
void term(){
    return;
}


/* expression syntax analysis
    factor(){ ⊕|→|↔|∧|∨ factor()}
*/
void expression(){

    return;
}

/* declare function
    # func_name func_args_count bits
*/
void function_declaration(){
    int i;
    printf("Function-Declaration-Begin");
    fetch_token(0);
    if(token == token_ident){
        print_error("Syntax-Error: Identifier should behind the #!");
        jump_until(1, token_pound);
        return;
    }
    fetch_token(1);
    if(token != token_int){
        print_error("Syntax-Error: args number should behind the function name!");
        jump_until(1, token_pound);
        return;
    }
    fetch_token(2);
    if(token != token_bits){
        print_error("Syntax-Error: bits number should behind the function args number!");
        jump_until(1, token_pound);
        return;
    }
    // write to the symbol table
    //identifier_name, type(1-function), args_count, bits
    printf("Function-Declaration-Begin");
    return;
}


/* function call
    function_name( expression, expression, ...)
*/
void function_call(){

    return;
}


/* scan all passage
    expression()
    {function_declaration()}
*/
void scan_passage(){


}






















