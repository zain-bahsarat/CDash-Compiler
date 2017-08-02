#include "DashUtil.h"

//it is used for debugging purposes .It prints the information about the token.  
//it takes token as an argument.    

void print_tokens(Token* test){
        printf("\t%5d\t%20s\t%5d\t%5d\t%5d\t%5d\n\n",
        test->ID,test->lexeme,test->type,test->line,test->column,test->value);
    }


void print_tree(node* r){
    IDS* id=(IDS*)malloc(sizeof(IDS));
    while(r!=NULL){
        
        printf("node properties\n");
        printf("node address = %s\n",r);
        printf("child address = %d , %d , %d\n",r->child[0],r->child[1],r->child[2],r->child[3]);
        printf("staement kind = %d\n",r->kind);
        printf("id's = ");
        id=r->ID;
        while(id!=NULL){
            printf(" %s ,",id->IDname);
            id=id->next;
        }
        printf("\ndatatype = %d\n",r->Datatype);
        printf("operator = %d\n",r->op);
        printf("values list = %s %s %s\n\n",r->VAL[0],r->VAL[1],r->VAL[2]);
        if(r->child[0]!=NULL){print_tree(r->child[0]);}
        if(r->child[1]!=NULL){print_tree(r->child[1]);}
        if(r->child[2]!=NULL){print_tree(r->child[2]);}
        if(r->child[3]!=NULL){print_tree(r->child[3]);}
        r=r->sibling;
    }

}