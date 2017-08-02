/* 
 * File:   main.c
 * Author: zain basharat
 *
 * Created on April 6, 2013, 8:13 PM
 */

#include "DashScanner.h"
#include "DashUtil.h"
#include "DashParser.h"
/*
 * 
 */
int main(int argc, char** argv) {
    f = fopen("code","r");
    Token *t;
    
    CERROR=0;

/*
    printf("\t\t\tList of tokens.\n");
    
    while(!EndOfFile()){
        t=getToken();
	print_tokens(t);//print the token.	
		}

*/

    
    
   parse();
    
    return (EXIT_SUCCESS);
}

