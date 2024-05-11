#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdlib.h>

//重要事項!!!!!!
//tree 在windows跑會有亂碼
//可能是mingw的鍋?
typedef struct NODE{
    char node_name[64]; 
    char node_type;
    struct NODE *childPtr;
    struct NODE *siblingPtr;
    struct NODE *parentPtr;
}node;

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
int pwd_r(node *);
int pwd_save(node *);
int save_travel_tree(node *);
int free_travel_tree(node *);
int print_tree(node *);
int tree(char *);
int (*fptr[ ])(char *) = {(int (*)())menu, mkdir, rmdir, ls, cd, pwd, create, rm, reload, save,tree, quit };

char *cmd[] =   {"menu", "mkdir", "rmdir", "ls", "cd", "pwd", "create", "rm",
                    "reload", "save", "tree" ,"quit", 0};

node *root, *cwd,*temp;                           /* root and CWD pointers */
char line[128];                               /* user input line */
char command[16], pathname[64];               /* user inputs */
char dname[64], bname[64];               /* string holders */
int count = 0;
FILE *fp;

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
    strcpy(root->node_name,"root");
    root->childPtr = NULL;
    temp = root;
    cwd = root;
}
int main(){
    int id;
    initialize();
    printf("Enter menu for help menu\n");
               // open a FILE stream for WRITE

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
    //printf("%d\n",strcmp(pathname,""));
    
    if(d_or_p == 0){
        s = strtok(dname,"/");
        if(!strcmp(pathname,"")){
            return -1;
        }
        if(!strcmp(dname,".")){
        temp = cwd;
        return 1;
    }
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
        //printf("%s\n",s);
        if(!strcmp(".",s)){ 
        }
        else if(!strcmp("..",s)){
            //printf("%p",temp->parentPtr);

            temp = temp->parentPtr;
        }else{
            temp = temp->childPtr;
            for(;temp!=NULL;temp = temp->siblingPtr){
                //printf("%s %p %s\n",s,temp->siblingPtr,temp->node_name);
                if(strcmp(s,temp->node_name) == 0){
                    flag = 1;
                    break;
                }
            }
            if(!flag)break;
        }
        //printf("%p\n",temp);
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
    //printf("temp name :%p\n",temp);
    
    if(temp->childPtr != NULL){
        //printf("child%s %p\n",temp->childPtr->node_name,temp->childPtr);
        temp = temp->childPtr;
        for(;;temp = temp->siblingPtr){
           // printf("in");
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
    printf("mkdir  pathname  : make a new directiry for a given pathname\n\
rmdir  pathname  : rm the directory, if it is empty.\n\
cd    [pathname] : change CWD to pathname, or to / if no pathname.\n\
ls    [pathname] : list the directory contents of pathname or CWD\n\
pwd              : print the (absolute) pathname of CWD\n\
creat  pathname  : create a FILE node.\n\
rm     pathname  : rm a FILE node.\n\
save   filename  : save the current file system tree in a file\n\
reload filename  : re-initalize the file system tree from a file\n\
tree             : use tree structure to present file structure\n\
quit             : save the file system tree, then terminate the program.\n");
     /* {"menu", "mkdir", "rmdir", "ls", "cd", "pwd", "create", "rm",
                    "reload", "save", "quit", 0}; */
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
   // printf("temp name :%p\n",temp);
    int child_flag = 0;
    node *sp_temp = temp;
    if(temp->childPtr != NULL){
        temp = temp->childPtr;
        for(;;temp = temp->siblingPtr){
            //printf("in");
            if(child_flag == 0){
                if(!strcmp(temp->node_name,bname)){
                    if(temp->node_type == 'D'){
                        if(temp->childPtr!=NULL){
                            printf("has child dir,can't remove\n");
                            return -1;
                        }
                        sp_temp->childPtr = temp->siblingPtr;
                        free(temp);
                        return -1;
                    }else{
                        printf("not a dir\n");
                        return -1;
                    }
                }
                if(temp->siblingPtr==NULL)break;
            }else{
                if(!strcmp(temp->node_name,bname)){
                    if(temp->childPtr!=NULL){
                        printf("has child dir,can't remove\n");
                        return -1;
                    }
                    sp_temp->siblingPtr = temp->siblingPtr;
                    free(temp);
                    return -1;
                }
                if(temp->siblingPtr==NULL)break;
            }
            child_flag++;
            sp_temp = temp;
        }
    }else{
        printf("dir not exist\n");
    }
    return -1;
}
int ls(char *pathname){
    int return_code =  find_route(1);
    //printf("%s",temp->childPtr->node_name);
    //printf("t %s %p\n",temp->node_name,temp);
    if(return_code == 1){
        if(temp->childPtr != NULL){
            for(temp = temp->childPtr;temp!=NULL;temp = temp->siblingPtr){
                //printf("%s %p\n",temp->node_name,temp);
                printf("%-11s%-12s\n",temp->node_type == 'F' ? "file" : "directory",temp->node_name);
                //printf("%s %s\n",temp->node_name,temp->node_type);
            }
        }
    }else{
        printf("dir not exist\n");
    }
    //printf("\n");
    return -1;
}
int cd(char *pathname){
    int return_code =  find_route(1);
    if(return_code == 1){
        if(temp->node_type!='F'){
            cwd = temp; 
        }else{
            printf("not a dir\n");
        }  
    }else{
        printf("dir not exist\n");
    }    
    return -1;
}
int pwd(char *pathname){
    if(cwd == root){
        printf("/\n");
        return -1;
    }
    pwd_r(cwd);
    printf("\n");
    return -1;
}
int create(char *pathname){
    split_pathname();
    int return_code =  find_route(0);

    if(return_code == -1){
        printf("dir not exist\n");
        return -1;
    }else{
        if(temp->node_type == 'F'){
            printf("not a dir\n");
            return -1;
        }
    }
    
    if(temp->childPtr != NULL){
        //printf("child%s %p\n",temp->childPtr->node_name,temp->childPtr);
        temp = temp->childPtr;
        for(;;temp = temp->siblingPtr){
           // printf("in");
            if(!strcmp(temp->node_name,bname)){
                printf("file has exist\n");
                return -1;
            }
            if(temp->siblingPtr==NULL)break;
        }
        temp->siblingPtr = malloc(sizeof(node));
        strcpy(temp->siblingPtr->node_name,bname);
        temp->siblingPtr->node_type = 'F';
        temp->siblingPtr->siblingPtr = NULL;
        temp->siblingPtr->childPtr = NULL;
        temp->siblingPtr->parentPtr = temp->parentPtr;
    }else{
        temp->childPtr = malloc(sizeof(node));
        strcpy(temp->childPtr->node_name,bname);
        temp->childPtr->node_type = 'F';
        temp->childPtr->childPtr = NULL;
        temp->childPtr->siblingPtr = NULL;
        temp->childPtr->parentPtr = temp;
    }
    return -1;
}
int rm(char *pathname){
    split_pathname();
    int return_code =  find_route(0);
    
    if(return_code == -1){
        printf("dir not exist\n");
        return -1;
    }else{
        if(temp->node_type == 'F'){
            printf("not a dir\n");
            return -1;
        }
    }

    node *sp_temp = temp;
    if(temp->childPtr != NULL){
        temp = temp->childPtr;
        for(;;temp = temp->siblingPtr){
            //printf("in");
            if(!strcmp(temp->node_name,bname)){
                if(temp->node_type == 'F'){
                    if(temp->childPtr!=NULL){
                        printf("has child dir,can't remove\n");
                        return -1;
                    }
                    sp_temp->childPtr = temp->siblingPtr;
                    free(temp);
                    return -1;
                }else{
                    printf("not a dir\n");
                    return -1;
                }
            }
            if(temp->siblingPtr==NULL)break;
            sp_temp = temp;
        }
    }else{
        printf("dir not exist\n");
    }
    return -1;
}
int save(char *pathname){
    //printf("%s",pathname);
    fp = fopen(pathname, "w+");
    if(fp == NULL){
        printf("not find file\n");
        return -1;
    }
    save_travel_tree(root->childPtr);
    fclose(fp);
    fflush(fp);
    return -1;
}
int reload(char *pathname){
    fp = fopen(pathname,"r");
    if(fp == NULL){
        printf("not find file\n");
        return -1;
    }
    char temp_type;
    cwd = root;
    free_travel_tree(root->childPtr);
    for(;;){
        int scan_value = fscanf(fp,"%c %s\n",&temp_type,pathname);
        if(scan_value == EOF)break;
        if(temp_type == 'F'){
            create(pathname);
        }else if(temp_type == 'D'){
            mkdir(pathname);
        }
    }
    printf("reload succssed\n");
    return -1;
}
int tree(char * pathname)
{
    printf(".\n");
    print_tree(root->childPtr);
    return -1;
}
int quit(char *pathname){
    exit(1);
    return -1;
}
int pwd_r(node *now_node){
    if(now_node->node_type == 'R')return -1;
    pwd_r(now_node->parentPtr);
    //printf("save succssed\n");
    printf("/%s", now_node->node_name);
}
int save_travel_tree(node *now_node){
    //printf("%p\n",now_node);
    if(now_node == NULL) return -1;
    fprintf(fp,"%c ",now_node->node_type);
    pwd_save(now_node);
    fprintf(fp,"\n");
    //printf("\n");
    save_travel_tree(now_node->childPtr);
    save_travel_tree(now_node->siblingPtr);
}
int pwd_save(node *now_node){
    if(now_node->node_type == 'R')return -1;
    pwd_save(now_node->parentPtr);
    fprintf(fp,"/%s", now_node->node_name);
    //printf("/%s", now_node->node_name);
}

int print_tree(node *now_node){
    char *s="├── ",*s1="│   ",*s2="└── ",*s3="    ";
	int i;
    if(now_node==NULL)return -1;
    if(count>0 && now_node->siblingPtr!=NULL)printf("%s",s1);
    else if(count>0 && now_node->siblingPtr==NULL) printf("%s",s1);
    else if(count==0 && now_node->siblingPtr!=NULL)printf("%s",s);
    else if(count==0 && now_node->siblingPtr==NULL)printf("%s",s2);
    for(i=0;i<(count-1);i++){
        node *count_temp = now_node;
        for(int j=i;j<(count-1);j++){
            count_temp = count_temp->parentPtr;
        }
        //printf(count_temp->node_name);
        if(count_temp->siblingPtr!=NULL){
            printf("%s",s1);
        }else{
            printf("%s",s3);
        }  
    }   
    if(count>0 && now_node->siblingPtr!=NULL)printf("%s",s);
    else if(count>0 && now_node->siblingPtr==NULL) printf("%s",s2);
    printf("%s",now_node->node_name);
    printf("\n");
    count++;
    print_tree(now_node->childPtr);
    count--;
    print_tree(now_node->siblingPtr);	
}

int free_travel_tree(node *now_node){
    //printf("%p\n",now_node);
    if(now_node == NULL) return -1;
    free_travel_tree(now_node->childPtr);
    free_travel_tree(now_node->siblingPtr);
    //print_tree(root->childPtr);
    free(now_node);
    //printf("\n");
}