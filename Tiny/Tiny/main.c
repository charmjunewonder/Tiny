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

int look;

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

//Get an identifier
char getname(){
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
    if(!isAlpha(look)){
        expected("Name");
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

//Initialize
void init(){
    getChar();
}

int main(int argc, const char * argv[])
{

    init();
    return 0;
}

