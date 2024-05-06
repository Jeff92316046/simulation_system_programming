#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdlib.h>

int menu(char *);
int mkdir(char *);
int rmdir(char *);
int ls(char *);
int cd(char *);
int pwd(char *);
int create(char *);
int rm(char *);
int save(char *);
int reload(char *);
int quit(char *);

typedef struct NODE{
    char node_name[64]; 
    char node_type;
    struct NODE *childPtr;
    struct NODE *siblingPtr;
    struct NODE *parentPtr;
}node;

int (*fptr[ ])(char *) = {(int (*)())menu, mkdir, rmdir, ls, cd, pwd, create, rm, save, reload, quit};

char *cmd[] =   {"menu", "mkdir", "rmdir", "ls", "cd", "pwd", "create", "rm",
                    "reload", "save", "quit", 0};

node *root, *cwd;                             /* root and CWD pointers */
char line[128];                               /* user input line */
char command[16], pathname[64];               /* user inputs */
char dname[64], bname[64];               /* string holders */

int findCmd(char *command)
{
   int i = 0;
   while(cmd[i]){
     if (strcmp(command, cmd[i])==0)
         return i;
     i++;
   }
   return -1;
}
void initialize() {
    root = malloc(sizeof(node));
    root->node_type = 'R';
    root->parentPtr = root;
    root->childPtr = malloc(sizeof(node));
    strcpy(root->childPtr->node_name,"a");
    root->childPtr->childPtr = malloc(sizeof(node));
    strcpy(root->childPtr->childPtr->node_name,"b");
    root->childPtr->siblingPtr = NULL;
    root->childPtr->parentPtr = root;
    root->childPtr->childPtr->siblingPtr = NULL;
    root->childPtr->childPtr->parentPtr = root->childPtr;
    root->childPtr->childPtr->childPtr = NULL;
    root->siblingPtr = NULL;
    cwd = root;
}
int main(){
    int id;
    initialize();
    printf("Enter menu for help menu\n");
    while (1) {
        printf(">");	
        fgets(line, 128, stdin);
        command[0] = pathname[0] = 0;
        sscanf(line, "%s %s", command, pathname);
        if (command[0]!=0) {
            id = findCmd(command);
            if (id<0){
                printf("invalid command\n");
                continue;
            }
            fptr[id](pathname);
            //printf("%d\n",id);
            
            
        }
    }
    return 0;
}

int find_route(){
    char *s;
    bool flag = 0;
    //printf("%s %s\n",dname,bname);
    s = strtok(dname,"/");
    node *temp;
    if(pathname[0] == '/'){
        temp = root;
        for(;temp->childPtr!=NULL;){
            temp = temp->childPtr;
            flag = 0;
            if(s == NULL)break;
            for(;temp!=NULL;temp = temp->siblingPtr){
                if(strcmp(s,temp->node_name) == 0){
                    flag = 1;
                    break;
                }
            }
            if(!flag)break;
            s = strtok(NULL,"/");
        }
        if(flag)return 1;
    }else{
        temp = cwd;
        for(;temp->childPtr!=NULL;){
            temp = temp->childPtr;
            flag = 0;
            printf("%s\n",s);
            if(!strcmp(".",s)){ 
                s = strtok(NULL,"/");
                temp = temp->parentPtr;
                continue;
            }
            else if(!strcmp("..",s)){
                s = strtok(NULL,"/");
                temp = temp->parentPtr;
                temp = temp->parentPtr;
                continue;
            }else{
                for(;temp!=NULL;temp = temp->siblingPtr){
                    printf("%s %p %s\n",s,temp->siblingPtr,temp->node_name);
                    if(strcmp(s,temp->node_name) == 0){
                        flag = 1;
                        break;
                    }
                }
                if(!flag)break;
            }
            printf("%p\n",temp->childPtr);
            s = strtok(NULL,"/");
        }
        if(flag)return 1;
    }
    return -1;
}

int mkdir(char *pathname){
    /* (1). Break up pathname into dirname and basename, e.g.
          ABSOLUTE: pathname=/a/b/c/d. Then dirname=/a/b/c, basename=d
          RELATIVE: pathname= a/b/c/d. Then dirname=a/b/c,  basename=d
    */
    char temp[64];
    
    strcpy(temp, pathname);
    strcpy(dname, dirname(temp));   // dname="/a/b"
    //printf("%s\n",dname);
    strcpy(temp, pathname);
    strcpy(bname,basename(temp));   // bname="c"
    //printf("%s\n",bname);
    printf("%d\n",find_route()) ;
    /*      
     (2). Search for the dirname node:
          ASSOLUTE pathname: search from /
          RELATIVE pathname: search from CWD.

                if nonexist ==> error messages and return
                if exist but not DIR ==> errot message and return
               
     (3). (dirname exists and is a DIR):
           Search for basename in (under) the dirname node:
                if already exists ==> error message and return;
   
           ADD a new DIR node under dirname. */
    
    return -1;
}
int menu(char *pathname){
    return -1;
}
int rmdir(char *pathname){
    return -1;
}
int ls(char *pathname){
    return -1;
}
int cd(char *pathname){
    return -1;
}
int pwd(char *pathname){
    return -1;
}
int create(char *pathname){
    return -1;
}
int rm(char *pathname){
    return -1;
}
int save(char *pathname){
    return -1;
}
int reload(char *pathname){
    return -1;
}
int quit(char *pathname){
    return -1;
}