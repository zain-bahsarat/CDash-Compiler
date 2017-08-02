#include "DashParser.h"
//ebnf rules functions

node* prog();
node* dec_part();
node* dec_stat();
node* stat_part(int excep);
node* stat(int excep);
node* Ariexp();
node* term();
node* Ariterm();
node* factor1();
node* Relexp();
node* ass_stat();
node* if_stat();
node* switch_stat();
node* case_seq();
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
        //printf("---------------------------------------\n"); 
        //printf("expecting type = %d\n",tt);
        //print_tokens(token);
      //  printf("---------------------------------------\n");
        token=getToken();

    }
    else{
        printf("expecting type = %d\n",tt);
        printf("Error! Unexpected token:%s at line %d\n",token->lexeme,token->line);
    }
}

node* parse(){
    node* n;
    token=getToken();
    n=prog();
    return n;
}
node* prog(){
    node *n,*n1,*n2;
    n=NULL;
    match(HASH);
    match(MODE);
    if(token->type!=PINTEGER){printf("invalid mode type :%s at line--:%d \n",token->lexeme,token->line);}
    else{
        if(strcmp(token->lexeme,"2")==0){ MODE_STATUS=2;}
        else if(strcmp(token->lexeme,"8")==0){MODE_STATUS=8;}
        else if(strcmp(token->lexeme,"10")==0){MODE_STATUS=10;}
        else if(strcmp(token->lexeme,"16")==0){MODE_STATUS=16;}
        else{ printf("invalid mode type :%s at line:%d \n",token->lexeme,token->line);}
    }
    match(PINTEGER);
    if(token->type==DECLARATION){
        n=dec_part(); 
    }
    match(START);
    n1=stat_part(0);
    match(END);
    if(n==NULL){n=n1;}
    else{
        n2=n;
        while(n2!=NULL){
            n2=n2->sibling;
        }
       
        n2=n1;
    }
    
  
    return n;
}
node* dec_part(){
    printf("\n\n\t--dec part called--\t\n\n");
    node *n=NULL,*n1;
    match(DECLARATION);

    while(token->type!=START){
        
        n1->sibling=dec_stat();
        n1=n1->sibling;
        if(n==NULL){n=n1;}
    }
    
    n1->sibling=NULL;
    return n;
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
        ids=NULL;
    }
 
 return n;   
}
node* stat_part(int excep){
     node *n=NULL,*n1;
     printf("statement part exception is %d\n",excep);
    while(token->type!=END&&token->type!=ELSE&&token->type!=ENDIF&&token->type!=ENDCASE&&token->type!=BREAK
            &&token->type!=ENDFOR&&token->type!=UNTIL&&token->type!=ENDWHILE){ 
        n1->sibling=stat(excep);
        n1=n1->sibling;
        if(n==NULL&&n1!=NULL){printf(":: \n");n=n1;}
    }
    if(n1!=NULL){
    n1->sibling=NULL;}
    return n;
}
node* stat(int excep){
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
            break;
        case OUTPUT:
            n=output_stat();
            break;
        case IDENTIFIER:
            
            n=ass_stat();
            break;
        default:
            return NULL;
            break;  
    }
    return n;
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
                              
                        n->child[0]=Ariexp();
                        break;
        }
            
    
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
                //getvalue(token->lexeme);
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
int getvalue(){

    return 1;
}
node* Relexp(){
     node* n;
     if(token->type==LCBRACK){
         match(LCBRACK);
        node* n1=(node*)malloc(sizeof(node));
        n1=initialization(n1);
        n1->kind=RExp;
        n1=Relexp();
        match(RCBRACK);
        n=n1;
     }
     else if(token->type==IDENTIFIER||token->type==PINTEGER||token->type==NINTEGER||
             token->type==HEX||token->type==NHEX||token->type==STRING||token->type==CHARACTER){
         n=(node*)malloc(sizeof(node));
         n=initialization(n);
         if(token->type==IDENTIFIER){
            IDS* id=(IDS*)malloc(sizeof(IDS));
             n->ID=id;
             strcpy(id->IDname,token->lexeme);
             id->next=NULL;
             match(IDENTIFIER);
         }
         else if(token->type==PINTEGER||token->type==NINTEGER||token->type==HEX||token->type==NHEX){
             n=(node*)malloc(sizeof(node));
             n=initialization(n);
             strcpy(n->VAL[0],token->lexeme);
                     //getvalue(token->lexeme);
             match(token->type);
         }
         else if(token->type==STRING||token->type==CHARACTER){
             n=(node*)malloc(sizeof(node));
             n=initialization(n);
             strcpy(n->VAL[0],token->lexeme);
                     //getvalue(token->lexeme);
             match(token->type);
         }
         while(token->type==LT||token->type==GT||token->type==LEQ||token->type==GEQ||token->type==EQ
          ||token->type==NEQ){
                node* n1=(node*)malloc(sizeof(node));
                n1=initialization(n1);
                n1->op=token->type;
                n1->child[0]=n;
                n->kind=RExp;
                n=n1;
                match(token->type);
                n->child[1]=Relexp();
         }
     }
    return n;
}
node* if_stat(){
     node* n=(node*)malloc(sizeof(node));
     n=initialization(n);
     n->kind=IfStat;
     match(IF);
     match(LCBRACK);
     n->child[0]=Relexp();
     match(RCBRACK);
     n->child[1]=stat_part(1);
     
     if(token->type==ELSE){
         n->child[2]=stat_part(1);
     }
     match(ENDIF);
     return n;
}
node* switch_stat(){
    node* n=(node*)malloc(sizeof(node));
     n=initialization(n);
     n->kind=SwitchStat;
     match(SWITCH);
     match(LCBRACK);
     n->child[0]=Relexp();
     match(RCBRACK);
     match(CASES);
     n->child[1]=case_seq();
     match(ENDCASE);
     printf("ennnndddddd\n");
     return n;
}
node* case_seq(){
    node *n=NULL,*n1;
    n=n1;
    if(token->type!=ENDCASE){
        n1=(node*)malloc(sizeof(node));
        n1=initialization(n1);
        if(n==NULL){n=n1;}
        if(token->type!=ELSE&&(token->type==PINTEGER||token->type==NINTEGER||
            token->type==HEX||token->type==NHEX||token->type==STRING||token->type==CHARACTER)){
            strcpy(n1->VAL[0],token->lexeme);
            match(token->type);
            printf("\nhhhh\n");match(SEMI);
                n1->child[0]=stat_part(2);
                match(BREAK);
                n1->sibling=case_seq();
                n1=n1->sibling;
        }
        else if(token->type==ELSE){
             n1->sibling=(node*)malloc(sizeof(node));
                n1=n1->sibling;
                n1=initialization(n1);
                strcpy(n1->VAL[0],token->lexeme);
                match(ELSE);
                printf("ye wala semi-::::");
                match(SEMI);
                n1->child[0]=stat_part(2);
                match(BREAK);
                n1->sibling=NULL;
        }
         else{
                n1->sibling=NULL;
        }
        
    }
    return n;
}
node* for_stat(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    match(FOR);
    n->kind=ForStat;
    n->kind=ForStat;
    n->child[0]=ass_stat();
    n->child[1]=Relexp();
    n->child[2]=ass_stat();
    n->child[3]=stat_part(3);
    match(ENDFOR);
    return n;
}
node* rep_stat(){
    node* n=(node*)malloc(sizeof(node));
    n=initialization(n);
    n->kind=RepeatStat;
    match(REPEAT);
    n->child[0]=stat_part(4);
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
    n->child[2]=stat_part(5);
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
    if(token->type==STRING||token->type==CHARACTER){
        strcpy(n->VAL[0],token->lexeme);
        match(token->type);
    }
    else{
        n->child[0]=Ariexp();
    }
    
    return n;
}
