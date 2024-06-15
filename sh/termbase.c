/***** term project base code *****/ 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char gpath[128];    // hold token strings  
char *name[64];     // token string pointers
int  n;             // number of token strings

char dpath[128];    // hold dir strings in PATH
char *dir[64];      // dir string pointers
int  ndir;          // number of dirs   

int p_or_r_index[64];
int p_or_r_kind[64];
int p_or_r_n = 0;
/* void io_redirection();
void pipe_fun();
int has_pipe(); */
int p_and_r();
int r_pipe_and_redirection(int begin,int end);
void execute_command(int cmd_index, int input_fd, int output_fd);
int tokenize(char *pathname) 
{
  char *s;
  strcpy(gpath, pathname);   // copy into global gpath[]
  s = strtok(gpath, " ");    
  n = 0;
  while(s){
    name[n++] = s;           // token string pointers   
    s = strtok(0, " ");
  }
  name[n] = 0;               // name[n] = NULL pointer
}

int main(int argc, char *argv[ ], char *env[ ])
{
  int i;
  int pid, status;
  char *cmd;
  char line[128];

  // YOU DO: Write code to print argc, argv and env
  printf("argc = %d\n",argc);
  printf("argv = ");
  for(int i=0;argv[i]!=NULL;i++){
    printf("%s ",argv[i]);
  }
  printf("\nenv = ");
  for(int i=0;env[i]!=NULL;i++){
    printf("%s\n",env[i]);
  }
  printf("\n");
  while(1){
    printf("mysh %d running\n", getpid());
    printf("enter a command line : "); // cat file1 file2
                                       // ls -l -a -f
                                       // cat file | grep print
                                       // ANY valid Linux command line
    fgets(line, 128, stdin);
    line[strlen(line) - 1] = 0;        // kill \n at end 

    if (line[0]==0) // if (strcmp(line, "")==0) // if line is NULL
      continue;
    
    tokenize(line);       // divide line into token strings
    
    for (i=0; i<n; i++){  // show token strings   
        printf("name[%d] = %s\n", i, name[i]);
    }

    cmd = name[0];         // line = name0 name1 name2 ....

    if (strcmp(cmd, "cd")==0){
      chdir(name[1]);
      continue;
    }
    if (strcmp(cmd, "exit")==0)
      exit(0); 
    
    pid = fork();
     
    if (pid){
       printf("mysh %d forked a child sh %d\n", getpid(), pid);
       printf("mysh %d wait for child sh %d to terminate\n", getpid(), pid);
       pid = wait(&status);
       printf("ZOMBIE child=%d exitStatus=%x\n", pid, status); 
       printf("mysh %d repeat loop\n", getpid());
    }
    else{
        printf("child sh %d running\n", getpid());
        
        // make a cmd line = ./cmd for execve()
        strcpy(line, cmd);  // line="./cmd"
        printf("line = %s\n", line);
        /* io_redirection();
        int r;
        if(has_pipe()==-1){
          r = execvp(line, name);
        }else{
          pipe_fun();
        } */
        int r = p_and_r();
        
        printf("execve failed r = %d\n", r);
        exit(1);
    }
  }
}

/********************* YOU DO ***********************
1. I/O redirections:

Example: line = arg0 arg1 ... > argn-1

  check each arg[i]:
  if arg[i] = ">" {
     arg[i] = 0; // null terminated arg[ ] array 
     // do output redirection to arg[i+1] as in Page 131 of BOOK
  }
  Then execve() to change image
****************************************************/
/* void io_redirection(){
  for(int i=0;i<n;i++){
    if(strcmp(name[i],">") == 0){
      name[i] = 0;
      int fd = open(name[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if(fd < 0){
        return;
      }
      if(dup2(fd,STDOUT_FILENO)){
        close(fd);
      }
    }else if(strcmp(name[i],"<") == 0){
      name[i] = 0;
      int fd = open(name[i+1],O_RDONLY);
      if(fd < 0){
        return;
      }
      if(dup2(fd,STDIN_FILENO)){
        close(fd);
      };
    }
  }
}

void pipe_func(){
  for(int i=0;i<n;i++){
    printf("p%d\n",i);
    if(strcmp(name[i],"|") == 0){
      name[i] = 0;
      pid_t pid;
      int pd[2];
      pipe(pd);
      pid = fork();
      if(pid == -1){
        perror("fork");
        exit(0);
      }else if(pid == 0){
        close(pd[0]);
        dup2(pd[1],STDOUT_FILENO);
        close(pd[1]);
        execvp(name[0],name);
        perror("chi");
      }else{
        close(pd[1]);
        dup2(pd[0],STDIN_FILENO);
        close(pd[0]);
        execvp(name[i+1],&name[i+1]);
        perror("par");
      }
    }
  }
}
int has_pipe(){
  for(int i=0;i<n;i++){
    if(strcmp(name[i],"|") == 0){
      return 1;
    }
  }
  return -1;
} */

/********************* YOU DO ***********************
2. Pipes:

Single pipe   : cmd1 | cmd2 :  Chapter 3.10.3, 3.11.2

Multiple pipes: Chapter 3.11.2
****************************************************/

int p_and_r(){
  for(int i=0;i<n;i++){
    if(strcmp(name[i],">") == 0){
      /* printf("> %d\n",i); */
      name[i] = 0;
      p_or_r_index[p_or_r_n] = i;
      p_or_r_kind[p_or_r_n] = 1;
      /* printf("%d %d",p_or_r_index[p_or_r_n],p_or_r_kind[p_or_r_n]); */
      p_or_r_n++;
    }else if(strcmp(name[i],"<") == 0){
      /* printf("< %d\n",i); */
      name[i] = 0;
      p_or_r_index[p_or_r_n] = i;
      p_or_r_kind[p_or_r_n] = 2;
      p_or_r_n++;
    }else if(strcmp(name[i],"|") == 0){
      /* printf("| %d\n",i); */
      name[i] = 0;
      p_or_r_index[p_or_r_n] = i;
      p_or_r_kind[p_or_r_n] = 3;
      p_or_r_n++;
    }
  }
  return r_pipe_and_redirection(0,p_or_r_n);
}    

int r_pipe_and_redirection(int begin,int end){
  /* printf("%d %d ",begin,end); */
  if(begin == end){
    /* printf("0\n"); */
/*     perror(name[0]); */
    execvp(name[0],name);
    return 1;
  }
  if(p_or_r_kind[end-1] == 1){
    /* printf("1\n"); */
    int fd ;
    if(fd < 0){
      return -1;
    }
    pid_t pid;
    int pd[2];
    pipe(pd);
    pid = fork();
    if(pid == -1){
      /* perror("fork"); */
      exit(0);
    }else if(pid == 0){
      close(pd[0]);
      fd = open(name[p_or_r_index[end-1] + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if(fd < 0){
        return -1;
      }
      dup2(fd,STDOUT_FILENO);
      close(fd);
      r_pipe_and_redirection(begin,end-1);
      /* perror("chi"); */
    }else{
      close(pd[1]);
      wait(NULL);
      /* perror("par"); */
    }
  }else if(p_or_r_kind[end-1] == 2){
    /* printf("2\n"); */
    int fd ;
    if(fd < 0){
      return -1;
    }
    pid_t pid;
    int pd[2];
    pipe(pd);
    pid = fork();
    if(pid == -1){
      /* perror("fork"); */
      exit(0);
    }else if(pid == 0){
      close(pd[1]);
      fd = open(name[p_or_r_index[end-1] + 1],O_RDONLY);
      if(fd < 0){
        return -1;
      }
      dup2(fd,STDIN_FILENO);
      close(fd);
      r_pipe_and_redirection(begin,end-1);
      /* perror("chi"); */
    }else{
      close(pd[0]);
      wait(NULL);
      /* perror("par"); */
    }//用pipe
  }else if(p_or_r_kind[end-1] == 3){
    /* printf("3\n"); */
    pid_t pid;
    int pd[2];
    pipe(pd);
    pid = fork();
    if(pid == -1){
      /* perror("fork"); */
      exit(0);
    }else if(pid == 0){
      close(pd[0]);
      dup2(pd[1],STDOUT_FILENO);
      close(pd[1]);
      r_pipe_and_redirection(begin,end-1);
      /* perror("chi"); */
    }else{
      close(pd[1]);
      dup2(pd[0],STDIN_FILENO);
      close(pd[0]);
      execvp(name[p_or_r_index[end-1]+1],&name[p_or_r_index[end-1]+1]);
      wait(NULL);
      /* perror("par"); */
    }
  }
}
