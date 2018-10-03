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
    char * tmp;
    int args_num;
    // const
    if(token == token_const){
        // push_const 0/1
        printf("PUSH CONST:%d\n", val_const);
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
            printf("CALL FUNC:%s[args=%d]\n", tmp, args_num);
        }
        // variable
        else{
            printf("PUSH VAR:%s\n", tmp);
        }
    }
    //(expression)
    else if(token == token_paren_open){
        fetch_token(0);
        expression();
        if(token != token_paren_close){
            print_error("Syntax-Error: Paren not matched!");
            jump_until(2, token_pound, token_paren_close);
        }
        if(token == token_paren_close)
            fetch_token(0);
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
    int i = 0;
    while(token == token_neg){
        i++;
        fetch_token(0);
    }
    factor();
    if(i % 2 == 1){
        printf("NEG\n");
    }
    return;
}


/* expression syntax analysis
    factor(){ ⊕|→|↔|∧|∨ factor()}
*/
void expression(){
    Token ts[] = {token_conjunc, token_disjunc, token_equival, token_implica, token_xor};
    Token tmp_token;
    term();
    while(inside_tokens(token, ts, 5)){
        tmp_token = token;
        fetch_token(0);
        term();
        if(tmp_token == token_conjunc){
            printf("CONJUNC\n");
        }
        else if(tmp_token == token_disjunc){
            printf("DISJUNC\n");
        }
        else if(tmp_token == token_equival){
            printf("EQUIVAL\n");
        }
        else if(tmp_token == token_implica){
            printf("IMPLCA\n");
        }
        else{
            printf("XOR\n");
        }
    }
    return;
}

/* declare function
    # func_name func_args_count bits
*/
void function_declaration(){
    int i;
    printf("Function-Declaration-Begin\n");
    fetch_token(0);
    if(token != token_ident){
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
    printf("Function-Declaration-End\n");
    fetch_token(0);
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
            jump_until(2,token_comma, token_paren_close);
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
    if(token != token_pound){
        print_error("Syntax-Error: Unexpected token behind the expression!");
        jump_until(1, token_pound);
    }
    while(token == token_pound){
        function_declaration();
    }
    return;
}






















