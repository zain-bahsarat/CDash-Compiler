#include "DashScanner.h"
        
        int Sflag=0;
 //this flag is set when end of file is encountered.
       int ENDF=0;
       
 //to point the current character in the buffer.
        int current=0;

        //to hold the data from the file.
	char BUFFER[256];
	
	//current line of the token
	int line=1;
	
	//starting column of the token
	int column=1;
	
	//ID of the token
	int ID=0;
	
	//global variable for end of file
	int ENDFILE=0;

	//maximum length of the a token
	int MAX_TK_LEN=40;
	
	//it holds lexeme of the token
	char TOKEN_LEXEME[41];
	
	//it tells whether to add character in the lexeme or leave it.
	int CONSUMED=0;

//characters index 
//0=> NUM
//1=> ALPHA
//2=> WHITESPACE
//3=> {
//4=> }
//5=> '
//6=> "
//7=> <
//8=> >
//9=> !
//10=> other
//11=> *
//12=> =
//13=> SPECIAL_SYMBOL
//14=> -
//15=> $
int TRANSITION[18][16]={
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//0
			{2,5,1,10,-2,3,4,6,7,7,-2,9,17,9,13,16},//1
			{2,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9},//2
			{8,8,8,8,8,-1,8,8,8,8,8,8,8,8,8,8},//3
			{4,4,4,4,4,4,9,4,4,4,4,4,4,4,4,4},//4
			{5,5,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9},//5
			{-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,9,-9,-9,-9},//6
			{-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,9,-9,-9,-9},//7
			{-1,-1,-1,-1,-1,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},//8
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},//9
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,-1,-1,-1},//10
			{11,11,11,11,11,11,11,11,11,11,11,12,11,11,11,11},//11
			{11,11,11,11,1,11,11,11,11,11,11,12,11,11,11,11},//12
                        {14,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,15},//13
                        {14,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9},//14
                        {15,15,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9},//15
                        {16,16,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9},//16
                        {-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,9,-9,-9,-9}//17
		};
						
//isNUM() checks whether a character exists in the number set or not and returns bool value
//Special Symbols set contains [0-9]
	int  isNUM(char ch){
		if(ch>='0'&&ch<='9'){return 1;}
		else{return 0;}
	}
	
	
//isALPHA() checks whether a character exists in the alphabets/letters set or not and returns bool value
//Special Symbols set contains [a-z,A-Z]
	int  isALPHA(char ch){
		if(ch>='A'&&ch<='Z'||ch>='a'&&ch<='z'){return 1;}
		else{return 0;}
	}

	
//isWHITESPACE() checks whether a character exists in the whitespaces set or not and returns bool value
//Special Symbols set contains [nextline,tab,space]
	int  isWHITESPACE(char ch){
		if(ch==' '||ch=='\n'||ch=='\t'){return 1;}
		else{return 0;}
	}
	
	
//isSPECIAL_SYMBOL() checks whether a character exists in the special symbols set or not and returns bool value
//Special Symbols set contains [+,-,*,/,%,=,(,),comma,;]
	int  isSPECIAL_SYMBOL(char ch){
		if(ch=='+'||ch=='/'||ch=='%'||ch=='='||ch=='('||ch==')'||ch==','||ch==';'||ch=='#'){return 1;}
		else{return 0;}
	}	

//isRESERVE_WORD() checks whether a character exists in the Reserve words set or not and returns bool value
	int  isRESERVE_WORD(char str[]){
		if(strcmp(str,"repeat")== 0||strcmp(str,"until")== 0||strcmp(str,"while")== 0||strcmp(str,"endwhile")== 0||
                   strcmp(str,"for")== 0||strcmp(str,"endfor")== 0||strcmp(str,"input")== 0||strcmp(str,"output")== 0||
		   strcmp(str,"declaration")== 0||strcmp(str,"start")== 0||strcmp(str,"end")== 0||strcmp(str,"switch")== 0||
                   strcmp(str,"cases")== 0||strcmp(str,"endcase")== 0||strcmp(str,"otherwise")== 0||strcmp(str,"value")== 0||
		   strcmp(str,"if")== 0||strcmp(str,"then")== 0||strcmp(str,"endif")== 0||strcmp(str,"else")== 0||strcmp(str,"endelse")== 0||
                   strcmp(str,"integer")== 0||strcmp(str,"character")== 0||strcmp(str,"string")== 0||strcmp(str,"inc")== 0||
                   strcmp(str,"dec")== 0||strcmp(str,"break")== 0||strcmp(str,"MODE")== 0||strcmp(str,"#")== 0){return 1;}
		else{return 0;}
	}
	
//get_RW_TK_TYPE takes the string as input and returns the token type of the reserve words.
	
	TokenType get_RW_TK_TYPE(char str[]){
		if(strcmp(str,"repeat") == 0){return REPEAT;}
                else if(strcmp(str,"MODE")== 0){return MODE;}
		else if(strcmp(str,"unitl")== 0){return UNTIL;}
		else if(strcmp(str,"while")== 0){return WHILE;}
		else if(strcmp(str,"endwhile")== 0){return ENDWHILE;}
		else if(strcmp(str,"for")== 0){return FOR;}
		else if(strcmp(str,"endfor")== 0){return ENDFOR;}
		else if(strcmp(str,"input")== 0){return INPUT;}
		else if(strcmp(str,"output")== 0){return OUTPUT;}
		else if(strcmp(str,"declaration")== 0){return DECLARATION;}
		else if(strcmp(str,"start")== 0){return START;}
		else if(strcmp(str,"end")== 0){return END;}
		else if(strcmp(str,"switch")== 0){return SWITCH;}
		else if(strcmp(str,"cases")== 0){return CASES;}
		else if(strcmp(str,"endcase")== 0){return ENDCASE;}
                else if(strcmp(str,"break")== 0){return BREAK;}
		else if(strcmp(str,"otherwise")== 0){return OTHERWISE;}
		else if(strcmp(str,"value")== 0){return VALUE;}
		else if(strcmp(str,"if")== 0){return IF;}
		else if(strcmp(str,"endif")== 0){return ENDIF;}
		else if(strcmp(str,"else")== 0){return ELSE;}
		else if(strcmp(str,"endelse")== 0){return ENDELSE;}
		else if(strcmp(str,"then")== 0){return THEN;}
		else if(strcmp(str,"integer")== 0){return INTEGER_DATATYPE;}
		else if(strcmp(str,"character")== 0){return CHARACTER_DATATYPE;}
		else if(strcmp(str,"string")== 0){return STRING_DATATYPE;}
		else if(strcmp(str,"inc")== 0){return INC;}
		else if(strcmp(str,"dec")== 0){return DEC;}
	}
	
	
//get_SP_TK_TYPE takes the string as input and returns the token type of the special symbols.
	
	TokenType get_SP_TK_TYPE(char str[]){
		if(strcmp(str,"+")==0){return PLUS;}
                 else if(strcmp(str,"#")== 0){return HASH;}
		else if(strcmp(str,"/")== 0){return DIV;}
		else if(strcmp(str,"%")== 0){return MOD;}
		else if(strcmp(str,"(")== 0){return LCBRACK;}
		else if(strcmp(str,")")== 0){return RCBRACK;}
		else if(strcmp(str,",")== 0){return COMMA;}
		else if(strcmp(str,";")== 0){return SEMI;}
	}

//get_TokenType() takes the the input return token type
//It takes following arguments
//1=>previos state
//2=>previous character index
//3=>token lexeme

	TokenType get_TokenType(int index,int state,char str[]){
		if(state==1&& index==13 &&isSPECIAL_SYMBOL(str[0])){return get_SP_TK_TYPE(str);}
		else if(state==2&&index!=0){return PINTEGER;}
		else if(state==4&&index==6){return STRING;}
		else if(state==5&&(index!=0||index!=1)){
		if(isRESERVE_WORD(str)){return get_RW_TK_TYPE(str);}
		else{return IDENTIFIER;}
		}//remember to restrict the size of identifier to 6 characters.
		else if(state==17&&index==12){return EQ ;}
                else if(state==17&&index!=12){return ASSIGN ;}
		else if(state==6&&index!=12){return LEQ ;}
		else if(state==6&&(index!=8||index!=12)){return LT;}
		else if(state==7&&index==12){return GEQ ;}
		else if(state==7&&index!=12){
		if(strcmp(str,"!")== 0){return NOT;}else{return GT ;}
		}
		else if(state==8&&index==5){return CHARACTER;}
                else if(state==14&&index!=0){return NINTEGER;}
                else if(state==16&&(index!=0||index!=1)){return HEX;}
                else if(state==15&&(index!=0||index!=1)){return NHEX;}
                else if(state==13&&(index!=0||index!=15)){return MINUS;}
                else if(state==1&&index==11){return MULTI;}
	}


//getCharIndex functions returns the index of the caracter in Transition table.
//it takes one arguments 
//1st=> character whose index to be find.
	
	int getCharIndex(char ch){	
		if(isNUM(ch)){
			CONSUMED=1;
			return 0;
		}
		else if(isALPHA(ch)){
			CONSUMED=1;
			return 1;
		}	
		else if(isWHITESPACE(ch)){
                    if(Sflag){CONSUMED=1;}
                    else{CONSUMED=0;}
			return 2;
		}
		else if(ch=='{'){
			CONSUMED=0;
			return 3;
		}
		else if(ch=='\''){
			CONSUMED=0;
			return 5;
		}
		else if(ch=='\"'){
                   if(!Sflag){Sflag=1;}
                   else{Sflag=0;}
			CONSUMED=0;
			return 6;
		}
		else if(ch=='<'){
			CONSUMED=1;
			return 7;
		}	
		else if(ch=='>'){
			CONSUMED=1;
			return 8;
		}
		else if(ch=='!'){
			CONSUMED=1;
			return 9;
		}	
		else if(ch=='*'){
			CONSUMED=1;
			return 11;
		}	
		else if(ch=='='){
			CONSUMED=1;
			return 12;
		}	
		else if(isSPECIAL_SYMBOL(ch)){
			CONSUMED=1;
			return 13;
		}
		else if(ch=='}'){
			CONSUMED=0;
			return 4;	
		}
                else if(ch=='$'){
			CONSUMED=1;
			return 15;	
		}
                else if(ch=='-'){
			CONSUMED=1;
			return 14;	
		}
		else{
			CONSUMED=1;
			return 10;
		}	
	}
        
        //reads next character from buffer and return it.
	char getNxtChar(){
		if(!(current<strlen(BUFFER))){
			if(fgets(BUFFER,255,f)){current=0;}
		else{return EOF;}
		}
		column++;
		return BUFFER[current++];
	}
        
	//it returns the current character back to buffer.	
	void BT(){
	current--;
	if(current<0){current=0;}
	}
        
        
        //add token to the symbol table.
        void addSymbolTab(Token *t){
            Sym_Cur=(struct Symbol_Table*)malloc(sizeof(struct Symbol_Table));
            Sym_Cur->ID=t->ID;
            strcpy(Sym_Cur->lexeme,t->lexeme);
            Sym_Cur->type=t->type;
            Sym_Cur->column=t->column;
            Sym_Cur->line=t->line;
            Sym_Cur=Sym_Cur->next;
        }
        
        void addLiteralTable(Token *t){
            Lit_Cur=(struct Literal_Table*)malloc(sizeof(struct Literal_Table));
            Lit_Cur->ID=t->ID;
            strcpy(Lit_Cur->lexeme,t->lexeme);
            Lit_Cur->type=t->type;
            Lit_Cur=Lit_Cur->next;
        }
	
	//getToken returns the current token to the parser.	
	Token* getToken(){		
		int state=1;			//holds the state,which is used for transition.
		char ch;				//holds the character from the buffer.
		int index=0;			//holds the index of the character,which is used for transition.
		int prevState=1;		//holds the previous value of the state
		int col=--column;				//holds the starting col of the token		
		int BackTrack=0;		//whether to backtrack or not.
		int error=0;			//if state is an error state then set to 1
		int counter=0;			//counter for lexeme
		Token *tk=(Token*)malloc(sizeof(Token));			
		while(state!=9&&state!=-1&&state!=-9&&state!=-2){
			if(ch=='\n'){line++;column=1;}//if the next character is line break then add one to line reset column.
			ch=getNxtChar();
			if(ch==EOF){ENDF=1;}	
			prevState=state;
			index=getCharIndex(ch);
			state=TRANSITION[state][index];
																  //if token length is greater than 40 then it is also an error token		
			if(state==-1||state==-2||counter>MAX_TK_LEN){error=1;}//if state is -1 or -2 it means that scanner gets a error token
			if(state==-1||state==-9){BackTrack=1;}//
			if(state==1){counter=0;}// reset counter for white spaces and comments
			if(CONSUMED&&!BackTrack){TOKEN_LEXEME[counter++]=ch;}
		}
		if(BackTrack){BT();}
		TOKEN_LEXEME[counter]='\0';
		tk->ID=++ID;
		strcpy(tk->lexeme,TOKEN_LEXEME);
		if(error){tk->type=ERROR;}
		else{tk->type=get_TokenType(index,prevState,TOKEN_LEXEME);}
		tk->value=0;
		tk->column=col;
		tk->line=line;
                if(tk->type==CHARACTER||tk->type==STRING||tk->type==PINTEGER){addLiteralTable(tk);}
                addSymbolTab(tk);
                
		return tk;
	}		

        //returns the end of file flag.
        int EndOfFile(){
            return ENDF; 
         }	