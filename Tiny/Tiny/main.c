//
//  main.c
//  Tiny
//
//  Created by charmjunewonder on 8/2/12.
//  Copyright (c) 2012 charmjunewonder. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define strbufsize 1024
#define errbufsize 1024

char look;

//Report an Error
void error(char *err){
    fprintf(stderr, "Error: %s.\n", err);
}

//Report error and halt
void fail(char *err){
    error(err);
    abort();
}

//Read New Character From Input Stream
void getChar(){
    look = getchar();
    if(ferror(stdin))
        fail("Error reading stdin");
    if(feof(stdin))
        error("EOF on stdin");
}

//Report what was expected and halt
void expected(char* s){
    char err[errbufsize];
    strncpy(err, s, errbufsize);
    strncat(err, " Expected", errbufsize - strlen(" Expected"));
    fail(err);
}

//Match a specific input character
void match(char c){
    char err[4] = {"' '"};
    if (look == c){
        getChar();
    }else{
        err[1] = c;
        expected(err);
    }
}

//Recognize an Alpha character
#define isAlpha(c) isalpha(c)

//Recognize a decimal digit
#define isDigit(c) isdigit(c)

//Recognize an addop
int isAddop(char c){
    return '+'==c || '-'==c;
}

//Get an identifier
char getName(){
    char retval = 0;
	if (!isAlpha(look)){
        expected("Name");
    }else{
        retval = toupper(look);
        getChar();
    }
    return retval;
}

//Get a number
char getNum(){
    char retval = 0;
    if(!isdigit(look)){
        expected("Integer");
    }else{
        retval = toupper(look);
        getChar();
    }
    return retval;
}

//Output a string with a leading Tab
void emit(char *s){
    printf("\t%s", s);
}

//Output a string with tab and newline
void emitln(char *s){
    printf("\t%s\n", s);
}

//forward declaration
void expression();

//Parse and Translate a Math Factor 
void factor(){
    char strbuf[strbufsize];
    if('('==look){
        match('(');
        expression();
        match(')');
    }else{
        snprintf(strbuf, strbufsize, "movl\t$%c,%%eax", getNum());
        emitln(strbuf);
    }
}

//Recognize and Translate a Multiply
void multiply(){
    match('*');
    factor();
    emitln("pop\t%ebx"); //pop top of stack to ebx
    emitln("mul\t%ebx"); //multiply eax by ebx
}

//Recognize and Translate a Divide
void divide(){
    match('/');
    factor();
    emitln("mov\t%eax,%ebx"); //move second factor to ebx
    emitln("pop\t%eax"); //pop first factor into eax
    emitln("xor\t%edx,%edx"); //clear high word of dividend
    emitln("div\t%ebx"); //divide first factor by second factor
}

//Parse and Translate a Math Term
void term(){
    factor();
    while ('*'==look || '/'==look) {
        emitln("push\t%eax");
        switch (look) {
            case '*':
                multiply();
                break;
            case '/':
                divide();
                break;
            default:
                expected("Mulop");
                break;
        }
    }
}

//Recognize and Translate an Add
void add(){
    match('+');
    term();
    emitln("pop\t%ebx"); //pop top of stack to ebx
    emitln("add\t%ebx,%eax"); //add ebx to eax
}

//Recognize and Translate a Subtract
void substract(){
	match('-');
	term();
	emitln("pop\t%ebx"); //pop top of stack to ebx
	emitln("sub\t%ebx,%eax"); //subtract ebx from eax
	emitln("neg\t%eax"); //negate eax to fix sign error
}

//Parse and translate a Math Expression
void expression(){
    if (isAddop(look)) {
        emitln("xor\t%eax,%eax"); //unary - or + so first operand is zero
    }else{
        term();
    }
    while (isAddop(look)) {
        emitln("push\t%eax"); //push eax to top of stack
        switch (look) {
            case '+':
                add();
                break;
            case '-':
                substract();
                break;
            default:
                expected("Addop");
                break;
        }
    }
}

//Initialize
void init(){
    getChar();
}

int main(int argc, const char * argv[])
{
    init();
    expression();
    return 0;
}

