#include "DashCGen.h"

FILE *file,*ftmp;
int str_counter=0;
IDS* id;
fpos_t pos;
char temp[]="str";
char buffer_temp[256];
long file_size;
int FirstTm=1;
int CHindex,pCHindex;

int for_counter=0;
int while_counter=0;
int repeat_counter=0;
int if_counter=0;
int if_else_counter=0;
int switch_counter=0;
int cases_counter=0;

int case_flag=0;
int else_part=0;

void Exp_CG(node* statement){
                    node* n=statement->child[0];
                statement=statement->child[0];
                if(n->op==EMPTY){
                        fflush(file);
                        fflush(stdout);
                        if(n->ID!=NULL){strcpy(n->VAL[0],n->ID->IDname);}   
                        fprintf(file,"mov ax,%s\n",n->VAL[0]);
                        
                }
                while(n->op!=EMPTY){
                    statement=n;
                        while(statement->child[0]->op!=EMPTY||statement->child[1]->op!=EMPTY){
                            if(statement->child[0]->op==EMPTY){CHindex=1;}
                            else if(statement->child[1]->op==EMPTY){CHindex=0;}
                            printf("index0= %d,indec1=%d\n",statement->child[0]->op,statement->child[1]->op);
                            statement=statement->child[CHindex];
                            printf("index= %d\n",CHindex);
                        }
                        fflush(file);
                        fflush(stdout);
                        if(FirstTm){
                                FirstTm=0;pCHindex=0;
                                if(MODE_STATUS==16){sprintf(statement->child[0]->VAL[0],"%s","h");}
                                if(statement->child[0]->ID!=NULL){strcpy(statement->child[0]->VAL[0],statement->child[0]->ID->IDname);}
                                fprintf(file,"mov ax,%s\n",statement->child[0]->VAL[0]);
                                if(MODE_STATUS==16){sprintf(statement->child[1]->VAL[0],"%s","h");}
                                if(statement->child[1]->ID!=NULL){strcpy(statement->child[1]->VAL[0],statement->child[1]->ID->IDname);}
                                fprintf(file,"mov bx,%s\n",statement->child[1]->VAL[0]);
                                
                        }
                        else{
                            printf("pindex= %d and cindex=%d\n",pCHindex,CHindex);
                            pCHindex=bit_reverse(pCHindex);
                            printf("reverse index=%d\n",pCHindex);
                            fflush(file);
                            fflush(stdout);
                            if(MODE_STATUS==16){sprintf(statement->child[pCHindex]->VAL[0],"%s","h");}
                            if(statement->child[pCHindex]->ID!=NULL){strcpy(statement->child[pCHindex]->VAL[0],statement->child[pCHindex]->ID->IDname);}
                            fprintf(file,"mov bx,%s\n",statement->child[pCHindex]->VAL[0]);
                        }
                        fflush(file);
                        fflush(stdout);
                        
                        switch(statement->op){
                            case PLUS:
                                fprintf(file,"add ax,bx\n");
                                break;
                            case MINUS:
                                fprintf(file,"sub ax,bx\n");
                                break;
                            case MULTI:
                                fprintf(file,"mul bx\n");
                                break;
                            case DIV:
                                fprintf(file,"xchg ax,bx\ndiv bx\n");
                                break;
                            case MOD:
                                fprintf(file,"xchg ax,bx\ndiv bx\nmov ax,dx\n");
                                break;
                            default:
                                break;   
                        }
                        pCHindex=CHindex;
                        statement->op=EMPTY;
                        
                }
}

int split_file(){
    char ch;
    ftmp=fopen("tmp.txt","w+");
    while((ch=fgetc(file))!=EOF){
        fputc(ch,ftmp);
    } 
    return 1;
}

void delete_file(){
    remove("tmp.txt");
}
void embed_file(){
    char ch;
    fseek(ftmp,0,SEEK_SET);
    while((ch=fgetc(ftmp))!=EOF){
        fputc(ch,file);
    }
    fclose(ftmp);
}

void initData(){
    file=fopen("code.asm","w+");
    if(file)
    {printf("okkokok");
    fprintf(file,"extern puts:far");
    fflush(file);
    fflush(stdout);
    switch(MODE_STATUS){
        case 2:
            fprintf(file,",inputBIN:far,outputBIN:far\n"); 
            break;
        case 8:
            fprintf(file,",inputOCT:far,outputOCT:far\n"); 
            break;
        case 10:
            fprintf(file,",inputDEC:far,outputDEC:far\n"); 
            break;
        case 16:
            fprintf(file,",inputHEX:far,outputHEX:far\n"); 
            break;
        default:
            break;
    }
     fprintf(file,".model small\n.stack 200\n%s\n",".data");  }
    else
        printf("errorr");
}

void initCode(){
    fgetpos(file,&pos);
    fflush(file);
    fflush(stdout);
    fprintf(file,".code\nstart:\nmov ax,@data\nmov ax,0h\n\n");
}
void endCode(){
     fflush(file);
     fflush(stdout);
     fprintf(file,"end ");
}

int bit_reverse(int b){
    if(b==1){return 0;}
    else{return 1;}
}

void generate(StatKind sk,node* statement){
        if(CERROR){fclose(file);remove("code.asm");}
    else{
    switch(sk){
        case DecStat:
     
            id=statement->ID;
            while(id!=NULL){
            fflush(file);
            fflush(stdout);
            fprintf(file,"%s db ?\n",id->IDname);
            id=id->next;
            } 
            break;
        case DAAStat:
            printf("in it");
            id=statement->ID;
            fflush(file);
            fflush(stdout);
            fprintf(file,"%s db ",id->IDname);
            fflush(file);
            fflush(stdout);
            switch(statement->Datatype){
                case STRING_DATATYPE:
                    fprintf(file,"\"%s$\"\n",statement->VAL[0]);
                    break;
                case CHARACTER_DATATYPE:
                    fprintf(file,"\'%s\'\n",statement->VAL[0]);
                    break;
                case INTEGER_DATATYPE:
                    if(MODE_STATUS==16){fprintf(file,"%sh\n",statement->VAL[0]);}
                    else{fprintf(file,"%s\n",statement->VAL[0]);}
                    break;
                default:
                    printf("not identify");
                    break;
            }
            break;
            
        case InputStat:
            id=statement->ID;
             fflush(file);
             fflush(stdout);
             switch(MODE_STATUS){
                case 2:
                        fprintf(file,"call inputBIN\nmov %s,ax\n",id->IDname); 
                        break;
                case 8:
                        fprintf(file,"call inputOCT\nmov %s,ax\n",id->IDname);
                        break;
                case 10:
                        fprintf(file,"call inputDEC\nmov %s,ax\n",id->IDname);
                        break;
                case 16:
                        fprintf(file,"call inputHEX\nmov %s,ax\n",id->IDname);
                        break;
                default:
                        break;
                }
             
            break;
            
        case OutputStat:
            FirstTm=1;
            fflush(file);
            fflush(stdout);
            if(statement->child[0]==NULL){
                fsetpos(file,&pos);
                if(split_file()){
                        fsetpos(file,&pos);
                        sprintf(buffer_temp,"%s%d db \"%s$\"\n",temp,++str_counter,statement->VAL[0]);
                        fprintf(file,"%s",buffer_temp);
                        embed_file();
                        delete_file();
                        fflush(file);
                        fflush(stdout);
                       fprintf(file,"mov dx,offset %s%d\ncall puts\n",temp,str_counter);
                }
            }
            else{
                Exp_CG(statement);
                fflush(file);
                fflush(stdout);
                switch(MODE_STATUS){
                case 2:
                        fprintf(file,"call outputBIN\n"); 
                        break;
                case 8:
                        fprintf(file,"call outputOCT\n");
                        break;
                case 10:
                        fprintf(file,"call outputDEC\n");
                        break;
                case 16:
                        fprintf(file,"call outputHEX\n");
                        break;
                default:
                        break;
                }
            }
            break;
        case IfStat:
            statement=statement->child[0];
            fflush(file);
            fflush(stdout);
            if(statement->child[0]->ID!=NULL){strcpy(statement->child[0]->VAL[0],statement->child[0]->ID->IDname);}                        
            if(statement->child[1]->ID!=NULL){strcpy(statement->child[1]->VAL[0],statement->child[1]->ID->IDname);}
            if(statement->child[0]->Datatype==CHARACTER){sprintf(statement->child[0]->VAL[0],"\'%s\'",statement->child[0]->VAL[0]);}
            if(statement->child[1]->Datatype==CHARACTER){sprintf(statement->child[1]->VAL[0],"\'%s\'",statement->child[1]->VAL[0]);}
            fprintf(file,"\n\npush dx\nmov dx,%s\ncmp dx,%s\n",statement->child[0]->VAL[0],statement->child[1]->VAL[0]);
            fflush(file);
            fflush(stdout);
            switch(statement->op){
                case EQ:
                    sprintf(buffer_temp,"%s%d_end","for",++for_counter);
                    fprintf(file,"jne %s\n",buffer_temp);
                    break;
                case LEQ:
                    sprintf(buffer_temp,"%s%d_end","if",++for_counter);
                    fprintf(file,"jg %s\n",buffer_temp);
                    break;
                case NEQ:
                    sprintf(buffer_temp,"%s%d_end","if",++for_counter);
                    fprintf(file,"je %s\n",buffer_temp);
                    break;
                case GT:
                    sprintf(buffer_temp,"%s%d_end","if",++for_counter);
                    fprintf(file,"jle %s\n",buffer_temp);
                    break;
                case GEQ:
                    sprintf(buffer_temp,"%s%d_end","if",++for_counter);
                    fprintf(file,"jl %s\n",buffer_temp);
                    break;
                case LT:
                    sprintf(buffer_temp,"%s%d_end","if",++for_counter);
                    fprintf(file,"jge %s\n",buffer_temp);
                    break;
                default:
                    break;
            }
            
            break;
        case IfElseStat: 
             else_part=1;
             fflush(file);
             fflush(stdout);
             sprintf(buffer_temp,"jmp %s%d%s_end","if",for_counter,"_else");
             fprintf(file,"%s\n",buffer_temp); 
             fflush(file);
             fflush(stdout);
             sprintf(buffer_temp,"%s%d_end","if",for_counter);
             fprintf(file,"%s:\n",buffer_temp);
            break;
        case EndIfStat:
            fflush(file);
            fflush(stdout);
            if(else_part){
            else_part=0;
            sprintf(buffer_temp,"%s%d%s_end","if",for_counter,"_else");
            fprintf(file,"%s:\npop dx\n\n",buffer_temp);            
            }
            else{
            sprintf(buffer_temp,"%s%d_end","if",for_counter);
            fprintf(file,"%s:\npop dx\n\n",buffer_temp);
            }
            break;
            
        case SwitchStat:
            case_flag=0;
            fflush(file);
            fflush(stdout);
            fprintf(file,"\n\npush dx\nmov dx,%s\n",statement->ID->IDname);
            switch_counter++;cases_counter++; 
            break;
        case EndCaseStat:
            fflush(file);
            fflush(stdout);
            fprintf(file,"switch%d_end:\npop dx\n",switch_counter);
            break;
        case CaseStat:
            
            fflush(file);
            fflush(stdout);
            if(case_flag){
            case_flag=0;
            fprintf(file,"jmp switch%d_end\n\n",switch_counter);
            }
            fflush(file);
            fflush(stdout);
            if(statement->Datatype==CHARACTER){sprintf(buffer_temp,"\'%s\'",statement->VAL[0]);}
            fprintf(file,"switch%d_case%d:\ncmp dx,%s\n",switch_counter,cases_counter,buffer_temp);
            fflush(file);
            fflush(stdout);
            fprintf(file,"jne switch%d_case%d\n",switch_counter,++cases_counter);
            break;
        case SwitchElse:
            fflush(file);
            fflush(stdout);
            fprintf(file,"jmp switch%d_end\n\nswitch%d_case%d:\n",switch_counter,switch_counter,cases_counter);
            break;
        case AssignStat:
            if(strcmp(statement->VAL[0],"")&&statement->Datatype!=EMPTY){
                fsetpos(file,&pos);
                fflush(file);
                fflush(stdout);
                sprintf(buffer_temp,"%s%d db \"%s$\"\n",temp,++str_counter,statement->VAL[0]);
                fprintf(file,"%s",buffer_temp);
                embed_file();
                delete_file();
                fflush(file);
                fflush(stdout);
                fprintf(file,"mov cx,offset %s%d\nmov %s,cx\n",temp,str_counter,statement->ID->IDname);
            }
            else{
                Exp_CG(statement);
                fflush(file);
                fflush(stdout);
                fprintf(file,"mov %s,ax\n",statement->ID->IDname);
            }
            break;
        case ForStat:
                fflush(file);
                fflush(stdout);
                fprintf(file,"push cx\npush dx\nmov dx,%s\nmov cx,%s\n",statement->VAL[0],statement->child[0]->VAL[0]);
                fflush(file);
                fflush(stdout);
                fprintf(file,"for%d:\nmov %s,dx\n",++for_counter,statement->ID->IDname);
            break;
        case EndforStat:
            fflush(file);
            fflush(stdout);
            if(statement->Datatype==CHARACTER){sprintf(buffer_temp,"\'%s\'",statement->child[1]->VAL[0]);}
            else if(statement->Datatype==NINTEGER){sprintf(buffer_temp,"-%s",statement->child[1]->VAL[0]);}
            fprintf(file,"cmp dx,cx\nje end_for%d\nadd ax,%s\njmp for%d\nend_fir%d:\npop dx\n",for_counter,statement->ID->IDname);
                
            break;
        default:
            break;
    }
    }
}
