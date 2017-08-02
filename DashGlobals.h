/* 
 * File:   Globals.h
 * Author: zain basharat
 *
 * Created on April 6, 2013, 7:34 PM
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

//header files of c library to be used in making this compiler.
   #include<stdio.h>
   #include<string.h>
   #include<stdlib.h>
   #include <stdlib.h>
   
//it define the number type in the program
    int MODE_STATUS;
//errorflag to stop from generating code
    int CERROR;
    
//it holds token types which this language supports.
      typedef enum{
		ERROR,REPEAT,UNTIL,WHILE,ENDWHILE,FOR,ENDFOR,INPUT,
                OUTPUT,DECLARATION,START,END,SWITCH,CASES,ENDCASE,
                OTHERWISE,IF,THEN,ENDIF,ELSE,ENDELSE,INTEGER_DATATYPE,
                CHARACTER_DATATYPE,STRING_DATATYPE,INC,DEC,PLUS,MINUS,DIV,
                MULTI,MOD,ASSIGN,EQ,LEQ,NEQ,GT,GEQ,LT,LCBRACK,RCBRACK,
                COMMA,SEMI,NOT,IDENTIFIER,CHARACTER,STRING,VALUE,ENDFIL,BREAK,
                HASH,MODE,PINTEGER,NINTEGER,HEX,NHEX,EMPTY}TokenType;

      typedef enum{
                   AExp,RExp,DecStat,DAAStat,AssignStat,InputStat,OutputStat,IfStat,EndIfStat,IfElseStat,SwitchStat,ForStat,
                           WhileStat,RepeatStat,factor,EndCaseStat,CaseStat,SwitchElse,EndforStat}StatKind;
//Token datatype which holds all the information about the token.
          typedef  struct{
		int ID;
		char lexeme[41];
		TokenType type;
		int value;
		int line;
		int column;
	}Token;      

 //Source file pointer 
	FILE *f;
        
        //typedef struct Symbol_Table;
        
        
       
        
         struct Symbol_Table{
                int ID;
                char lexeme[41];
		TokenType type;
		int line;
		int column;
                struct Symbol_Table* next;
        };
        
        
 
        
         struct Literal_Table{
                int ID;
                char lexeme[41];
		TokenType type;
                int value;
                struct Literal_Table* next;
        };
        

      
        
   struct Symbol_Table* Sym_Cur; 
   struct Literal_Table* Lit_Cur; 
   
   typedef struct IDLIST{
       char IDname[41];
       struct IDLIST* next;
   }IDS;
   
   
 typedef struct tnode{
       struct tnode *child[4];
       struct tnode *sibling;
       StatKind kind;
       IDS* ID;
       TokenType Datatype;
       TokenType op;
       char VAL[3][41];
   }node;
   

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

