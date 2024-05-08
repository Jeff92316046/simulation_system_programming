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

node *root, *cwd,*temp;                           /* root and CWD pointers */
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
    strcmp(root->node_name,"root");
    root->childPtr = NULL;
    temp = root;
    cwd = root;
}
int main(){
    int id;
    initialize();
    printf("Enter menu for help menu\n%p\n",root);
    
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

int split_pathname(){
    char temp[64];
    
    strcpy(temp, pathname);
    strcpy(dname, dirname(temp));   // dname="/a/b"
    //printf("%s\n",dname);
    strcpy(temp, pathname);
    strcpy(bname,basename(temp));   // bname="c"
}
int find_route(int d_or_p){
    char *s;
    bool flag = 0;
    //printf("%s %s\n",dname,bname);
    if(!strcmp(dname,".")){
        temp = cwd;
        return 1;
    }

    if(d_or_p == 0){
        s = strtok(dname,"/");
    }else if(d_or_p == 1){
        s = strtok(pathname,"/");
    }

    if(pathname[0] == '/'){
        temp = root;
    }else{
        temp = cwd;
    }
    for(;;){
        flag = 0;
        if(s == NULL){
            flag = 1;
            break;
        }
        printf("%s\n",s);
        if(!strcmp(".",s)){ 
        }
        else if(!strcmp("..",s)){
            printf("%p",temp->parentPtr);

            temp = temp->parentPtr;
        }else{
            temp = temp->childPtr;
            for(;temp!=NULL;temp = temp->siblingPtr){
                printf("%s %p %s\n",s,temp->siblingPtr,temp->node_name);
                if(strcmp(s,temp->node_name) == 0){
                    flag = 1;
                    break;
                }
            }
            if(!flag)break;
        }
        printf("%p\n",temp);
        s = strtok(NULL,"/");
    }
    if(flag)return 1;
    return -1;
}

int mkdir(char *pathname){
    /* (1). Break up pathname into dirname and basename, e.g.
          ABSOLUTE: pathname=/a/b/c/d. Then dirname=/a/b/c, basename=d
          RELATIVE: pathname= a/b/c/d. Then dirname=a/b/c,  basename=d
    */
    
    //printf("%s\n",bname);
    split_pathname();
    int return_code =  find_route(0);
    /*      
     (2). Search for the dirname node:
          ASSOLUTE pathname: search from /
          RELATIVE pathname: search from CWD.

                if nonexist ==> error messages and return
                if exist but not DIR ==> errot message and return
    */
    if(return_code == -1){
        printf("dir not exist\n");
        return -1;
    }else{
        if(temp->node_type == 'F'){
            printf("not a dir\n");
        }
    }
    /*
     (3). (dirname exists and is a DIR):
           Search for basename in (under) the dirname node:
                if already exists ==> error message and return;
           ADD a new DIR node under dirname. */
    printf("temp name :%p\n",temp);
    
    if(temp->childPtr != NULL){
        printf("child%s %p\n",temp->childPtr->node_name,temp->childPtr);
        temp = temp->childPtr;
        for(;;temp = temp->siblingPtr){
            printf("in");
            if(!strcmp(temp->node_name,bname)){
                printf("file has exist\n");
                return -1;
            }
            if(temp->siblingPtr==NULL)break;
        }
        temp->siblingPtr = malloc(sizeof(node));
        strcpy(temp->siblingPtr->node_name,bname);
        temp->siblingPtr->node_type = 'D';
        temp->siblingPtr->siblingPtr = NULL;
        temp->siblingPtr->childPtr = NULL;
        temp->siblingPtr->parentPtr = temp->parentPtr;
    }else{
        temp->childPtr = malloc(sizeof(node));
        strcpy(temp->childPtr->node_name,bname);
        temp->childPtr->node_type = 'D';
        temp->childPtr->childPtr = NULL;
        temp->childPtr->siblingPtr = NULL;
        temp->childPtr->parentPtr = temp;
    }
}
int menu(char *pathname){
    return -1;
}
int rmdir(char *pathname){
    split_pathname();
    int return_code =  find_route(0);
    
    if(return_code == -1){
        printf("dir not exist\n");
        return -1;
    }else{
        if(temp->node_type == 'F'){
            printf("not a dir\n");
        }
    }
    /*
     (3). (dirname exists and is a DIR):
           Search for basename in (under) the dirname node:
                if already exists ==> error message and return;
           ADD a new DIR node under dirname. */
    printf("temp name :%p\n",temp);
    int child_flag = 0;
    node *sp_temp = temp;
    if(temp->childPtr != NULL){
        temp = temp->childPtr;
        for(;;temp = temp->siblingPtr){
            printf("in");
            if(child_flag == 0){
                if(!strcmp(temp->node_name,bname)){
                    sp_temp->childPtr = temp->siblingPtr;
                    return -1;
                }
                if(temp->siblingPtr==NULL)break;
            }else{
                if(!strcmp(temp->node_name,bname)){
                    sp_temp->siblingPtr = temp->siblingPtr;
                    return -1;
                }
                if(temp->siblingPtr==NULL)break;
            }
            sp_temp = temp;
        }
    }else{
        printf("dir not exist");
    }
    return -1;
}
int ls(char *pathname){
    temp = cwd;
    //printf("%s",temp->childPtr->node_name);
    if(temp->childPtr != NULL){
        for(temp = temp->childPtr;temp!=NULL;temp = temp->siblingPtr){
            printf("%s %p\n",temp->node_name,temp);
        }
    }
    
    //printf("\n");
    return -1;
}
int cd(char *pathname){
    temp = cwd;
    if(!strcmp(pathname,"..")){
        cwd = cwd->parentPtr;
        return -1;
    }
    for(temp = temp->childPtr;temp!=NULL;temp = temp->siblingPtr){
        if(!strcmp(temp->node_name,pathname)){
            cwd = temp;
            break;
        }
    }
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