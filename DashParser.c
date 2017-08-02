#include "DashParser.h"
#include "DashCGen.h"

//ebnf rules functions

void prog();
void dec_part();
node* dec_stat();
void stat_part(int excep);
void stat(int excep);
node* Ariexp();
node* term();
node* Ariterm();
node* factor1();
node* Relexp();
node* ass_stat();
node* if_stat();
node* switch_stat();
void case_seq();
node* rep_stat();
node* while_stat();
node* for_stat();
node* input_stat();
node* output_stat(); // </editor-fold>

//it holds the token type 
        Token *token;


node* initialization(node* n){
    n->child[0]=NULL;
    n->child[1]=NULL;
    n->child[2]=NULL;
    n->child[3]=NULL;
    n->Datatype=EMPTY;
    strcpy(n->VAL[0],"");
     strcpy(n->VAL[1],"");
     strcpy(n->VAL[2],"");
    n->ID=NULL;
    n->op=EMPTY;
    n->sibling=NULL;
    return n;
}

void match(TokenType tt){
    
    if(token->type==tt){
        token=getToken();
     }
    else{
        CERROR=1;
        printf("expecting type = %d\n",tt);
        printf("Error! Unexpected token:%s at line %d\n",token->lexeme,token->line);
    }
}

void parse(){
    node* n;
    token=getToken();
    prog();
}
void prog(){
    node *n,*n1,*n2;
    n=NULL;
    match(HASH);
    match(MODE);
    if(token->type==PINTEGER)
    {
        if(strcmp(token->lexeme,"2")==0){ MODE_STATUS=2;}
        else if(strcmp(token->lexeme,"8")==0){MODE_STATUS=8;}
        else if(strcmp(token->lexeme,"10")==0){MODE_STATUS=10;}
        else if(strcmp(token->lexeme,"16")==0){MODE_STATUS=16;}
        else{ printf("invalid mode type :%s at line:%d \n",token->lexeme,token->line);}
    }
    match(PINTEGER);
    initData();
    
    dec_part(); 


    match(START);
    initCode();
    stat_part(0);
    match(END); 
    endCode();
}
void dec_part(){
    printf("\n\n\t--dec part called--\t\n\n");
    node *n=NULL,*n1;
    match(DECLARATION);

    while(token->type!=START){
        n=dec_stat();
        printf("kind=%d\n",n->kind);
        generate(n->kind,n);
        if(n==NULL){n=n1;}
    }
}
node* dec_stat(){
     printf("\n\n\t--dec stat called--\t\n\n");
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    IDS* ids=(IDS*)malloc(sizeof(IDS));
    
    n->kind=DecStat;
    switch(token->type){
        case INTEGER_DATATYPE:
            n->Datatype=INTEGER_DATATYPE;
            match(INTEGER_DATATYPE);
            break;
        case CHARACTER_DATATYPE:
            n->Datatype=CHARACTER_DATATYPE;
            match(CHARACTER_DATATYPE);
            break;
        case STRING_DATATYPE:
            n->Datatype=STRING_DATATYPE;
            match(STRING_DATATYPE);
            break;
        default:
            break;
    }
    
    if(token->type==IDENTIFIER){
        n->ID=ids;
        strcpy(ids->IDname,token->lexeme);
    }
    match(IDENTIFIER);
    if(token->type==ASSIGN){
        n->kind=DAAStat;
        match(ASSIGN);
        switch(token->type){
                case CHARACTER:
                        strcpy(n->VAL[0],token->lexeme);
                        match(CHARACTER);
                        break;
                case STRING:
                        strcpy(n->VAL[0],token->lexeme);
                        match(STRING);
                        break;
                case PINTEGER:
                        strcpy(n->VAL[0],token->lexeme);
                        match(PINTEGER);
                        break;
                case NINTEGER:
                        strcpy(n->VAL[0],token->lexeme);
                        match(NINTEGER);
                        break;
                case HEX:
                        strcpy(n->VAL[0],token->lexeme);
                        match(HEX);
                        break;
                case NHEX:
                        strcpy(n->VAL[0],token->lexeme);
                        match(NHEX);
                        break;
                default:
                    printf("match no");
                        break;
        }
    }
    else{
        while(token->type!=START&&token->type!=INTEGER_DATATYPE&&
              token->type!=CHARACTER_DATATYPE&&token->type!=STRING_DATATYPE){
            match(COMMA);
            if(token->type==IDENTIFIER){
                ids->next=(IDS*)malloc(sizeof(IDS));
                ids=ids->next;
                strcpy(ids->IDname,token->lexeme); 
            }
            
            match(IDENTIFIER);
        }
        if(ids!=NULL){ids->next=NULL;}
        else{ids=NULL;}
    }
 
 return n;   
}
void stat_part(int excep){
     node *n=NULL;
     printf("statement part exception is %d\n",excep);
    while(token->type!=END&&token->type!=ELSE&&token->type!=ENDIF&&token->type!=ENDCASE&&token->type!=BREAK
            &&token->type!=ENDFOR&&token->type!=UNTIL&&token->type!=ENDWHILE){ 
        stat(excep);
    }

}
void stat(int excep){
    node *n;
    switch(token->type){
        case IF:
            if(excep==1){printf("Error! nesting not allowed for IF statement at line:%d \n",token->line);}
            else{
                n=if_stat();
            }
            break;
        case SWITCH:
            if(excep==2){printf("Error! nesting not allowed for SWITCH statement at line:%d \n",token->line);}
            else{
                n=switch_stat();
            }
            break;
        case FOR:
             if(excep==3){printf("Error! nesting not allowed for FOR statement at line:%d \n",token->line);}
            else{
                n=for_stat();
            }
            break;
        case REPEAT:
            if(excep==4){printf("Error! nesting not allowed for REPEAT statement at line:%d \n",token->line);}
            else{
                n=rep_stat();
            }
            break;
        case WHILE:
            if(excep==5){printf("Error! nesting not allowed for WHILE statement at line:%d \n",token->line);}
            else{
                n=while_stat();
            }
            break;
        case INPUT:
            n=input_stat();
            generate(n->kind,n);
            break;
        case OUTPUT:
            n=output_stat();
            generate(n->kind,n);
            break;
        case IDENTIFIER:
            
            n=ass_stat();
            break;
        default:
            return NULL;
            break;  
    }
}
node* ass_stat(){
    printf("assignment statement\n");
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    IDS* ids=(IDS*)malloc(sizeof(IDS));
    
    n->kind=AssignStat;
    if(token->type==IDENTIFIER){
        n->ID=ids;
        strcpy(ids->IDname,token->lexeme);
        ids->next=NULL;
    }
    match(IDENTIFIER);
    match(ASSIGN);
    printf("=\n");
            switch(token->type){
                case STRING:
                        strcpy(n->VAL[0],token->lexeme);
                        n->Datatype=CHARACTER;
                        match(STRING);
                        break;
                default:          
                        n->child[0]=Ariexp();
                        break;
        }
            
            generate(AssignStat,n);
    return n;
}
node* Ariexp(){
    node* n=term();
    while(token->type==PLUS||token->type==MINUS){
        node* n1=(node*)malloc(sizeof(node));
        n1=initialization(n1);
        n1->op=token->type;
        n1->child[0]=n;
        n->kind=AExp;
        n=n1;
        match(token->type);
        n->child[1]=term();
    }
    return n;
}
node* term(){
    node* n=Ariterm();
    while(token->type==MULTI||token->type==DIV){
        node* n1=(node*)malloc(sizeof(node));
        n1=initialization(n1);
        n1->op=token->type;
        n1->child[0]=n;
        n=n1;
        match(token->type);
        n->child[1]=Ariterm();
    }
    return n;
    
    
}
node* Ariterm(){
        node* n=factor1();
    while(token->type==MOD){
        node* n1=(node*)malloc(sizeof(node));
        n1=initialization(n1);
        n1->op=token->type;
        n1->child[0]=n;
        n=n1;
        match(token->type);
        n->child[1]=factor1();
    }
    return n;
    
}
node* factor1(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    if(token->type==PINTEGER||token->type==NINTEGER||token->type==HEX||token->type==NHEX){
        strcpy(n->VAL[0],token->lexeme);
        match(token->type);
    }
    else if(token->type==IDENTIFIER){
        IDS* id=(IDS*)malloc(sizeof(IDS));
        n->ID=id;
        strcpy(id->IDname,token->lexeme);
        id->next=NULL;
        match(IDENTIFIER); 
    }
    else if(token->type==LCBRACK){
        match(LCBRACK);
        n=Ariexp();
        match(RCBRACK);
    }
    return n;
}

node* Relexp(){
     node* n,*n1;
        if(token->type==IDENTIFIER||token->type==PINTEGER||token->type==NINTEGER||
             token->type==HEX||token->type==NHEX||token->type==CHARACTER){
         n=(node*)malloc(sizeof(node));
         n=initialization(n);
         if(token->type==IDENTIFIER){
            IDS* id=(IDS*)malloc(sizeof(IDS));
             n->ID=id;
             strcpy(id->IDname,token->lexeme);
             id->next=NULL;
             match(IDENTIFIER);
         }
         else {
             strcpy(n->VAL[0],token->lexeme);
             match(token->type);
             n->Datatype=token->type;
         }}
         if(token->type==LT||token->type==GT||token->type==LEQ||token->type==GEQ||token->type==EQ
          ||token->type==NEQ){
                n1=(node*)malloc(sizeof(node));
                n1=initialization(n1);
                n1->op=token->type;
                n1->child[0]=n;
                match(token->type); 
         }
       if(token->type==IDENTIFIER||token->type==PINTEGER||token->type==NINTEGER||
             token->type==HEX||token->type==NHEX||token->type==CHARACTER){
         n=(node*)malloc(sizeof(node));
         n=initialization(n);
         if(token->type==IDENTIFIER){
            IDS* id=(IDS*)malloc(sizeof(IDS));
             n->ID=id;
             strcpy(id->IDname,token->lexeme);
             id->next=NULL;
             match(IDENTIFIER);
             n1->child[1]=n;
         }
         else {
             strcpy(n->VAL[0],token->lexeme);
             match(token->type);
             n->Datatype=token->type;
             n1->child[1]=n;
         }}
    return n1;
}
node* if_stat(){
     node* n=(node*)malloc(sizeof(node));
     n=initialization(n);
     n->kind=IfStat;
     match(IF);
     match(LCBRACK);
     n->child[0]=Relexp();
     match(RCBRACK);
     generate(IfStat,n);
     stat_part(1);
     
     if(token->type==ELSE){
         n->kind=IfElseStat;
         match(ELSE);
         generate(IfElseStat,n);
         stat_part(1);
     }
     match(ENDIF);
      generate(EndIfStat,n);
     return n;
}
node* switch_stat(){
    node* n=(node*)malloc(sizeof(node));
     n=initialization(n);
     n->kind=SwitchStat;
     match(SWITCH);
     match(LCBRACK);
     IDS* id=(IDS*)malloc(sizeof(IDS));
     n->ID=id;
     strcpy(id->IDname,token->lexeme);
     id->next=NULL;
     match(IDENTIFIER);
     match(RCBRACK);
     generate(SwitchStat,n);
     match(CASES);
     while(token->type!=ENDCASE){case_seq();}
     match(ENDCASE); 
     generate(EndCaseStat,n);
     return n;
}
void case_seq(){
    node *n1;
    if(token->type!=ELSE){
        n1=(node*)malloc(sizeof(node));
        n1=initialization(n1);
        if((token->type==PINTEGER||token->type==NINTEGER||
            token->type==HEX||token->type==NHEX||token->type==STRING||token->type==CHARACTER)){
            strcpy(n1->VAL[0],token->lexeme);
            n1->Datatype=token->type;
            match(token->type);
            match(SEMI);
            generate(CaseStat,n1);
            stat_part(2);
            match(BREAK);
        }
           
     }
     else if(token->type==ELSE){
             n1=(node*)malloc(sizeof(node));
             n1=initialization(n1);
             strcpy(n1->VAL[0],token->lexeme);
             match(ELSE);
             match(SEMI);
             generate(SwitchElse,n1);
             stat_part(2);
             match(BREAK);//SwitchElse
             
        }
}
node* for_stat(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    match(FOR);
    n->kind=ForStat;
    IDS* id=(IDS*)malloc(sizeof(IDS));
    n->ID=id;
    strcpy(id->IDname,token->lexeme);
    id->next=NULL;
    match(IDENTIFIER);
    match(ASSIGN);
    if(token->type==CHARACTER||token->type==PINTEGER||token->type==NINTEGER){
        n->Datatype=token->type;
        strcpy(n->VAL[0],token->lexeme);
        match(token->type);
    }
    else{printf("error");}
    match(COMMA);
    if(token->type==CHARACTER||token->type==PINTEGER||token->type==NINTEGER){
        n->child[0]=(node*)malloc(sizeof(node));
        n->child[0]->Datatype=token->type;
        strcpy(n->child[0]->VAL[0],token->lexeme);
        match(token->type);
    }
    else{printf("error");}
    generate(ForStat,n);
    match(COMMA);
    if(token->type==PLUS||token->type==MINUS){
        n->child[1]=(node*)malloc(sizeof(node));
        n->child[1]->op=token->type;
        match(token->type);
        strcpy(n->child[1]->VAL[0],token->lexeme);
    }
    else{printf("error");}
    
    stat_part(3);
    match(ENDFOR);
    generate(EndforStat,n);
    return n;
}
node* rep_stat(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    n->kind=RepeatStat;
    match(REPEAT);
    stat_part(4);
    match(UNTIL);
    match(LCBRACK);
    n->child[1]=Relexp();
    match(RCBRACK);
     return n;
}
node* while_stat(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    n->kind=WhileStat;
    match(REPEAT);
    match(LCBRACK);
    n->child[0]=Relexp();
    match(RCBRACK);
    stat_part(5);
    match(ENDWHILE);
    return n;
}
node* input_stat(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    n->kind=InputStat;
    match(INPUT);
    IDS* id=(IDS*)malloc(sizeof(IDS));
    n->ID=id;
    strcpy(id->IDname,token->lexeme);
    id->next=NULL;
    match(IDENTIFIER);
    return n;
}
node* output_stat(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    n->kind=OutputStat;
    match(OUTPUT);
    if(token->type==STRING ){
        strcpy(n->VAL[0],token->lexeme);
        match(token->type);
    }
    else{
        n->child[0]=Ariexp();
    }
    
    return n;
}
