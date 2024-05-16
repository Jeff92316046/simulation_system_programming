/*********** A Multitasking System ************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int findCmd(char*);
void tswitch(void);
int do_switch(void);
int do_fork(void);
int do_ps(void);
int do_exit(void);
int kexit(int value);
int kfork(int (*func)(void));
int body(void);
int do_sleep(void);
int do_wakeup(void);
int do_wait(void);
int ksleep(int event);
int kwakeup(int event);
int kwait(int *status);

#include "type.h"

PROC proc[NPROC];		//NPROC procs
PROC *freeList;			//free list of procs 
PROC *sleepList;
PROC *readyQueue;		//priority queue of READY procs
PROC *running;			//current running proc pointer

#include "queue.c"

char *cmds[ ] =  {"ps",  "fork",  "switch",  "exit",  "sleep",  "wakeup",   "wait",  0};

int (*fptr[ ])()={ do_ps, do_fork, do_switch, do_exit, do_sleep, do_wakeup,  do_wait  };

/*******************************************************
kfork() creates a child process; returns child pid.
When scheduled to run, child PROC resumes to func();
********************************************************/
int kfork(int (*func)(void))
{
  PROC *p;
  int  i;
  /*** get a proc from freeList for child proc: ***/
  p = dequeue(&freeList);
  if (!p){
     printf("no more proc\n");
     return(-1);
  }
  
  p->child = 0; p->sibling = 0; p->parent = running;
      // insert p to END of running's child list
  if (running->child == 0)
      running->child = p;
  else{
      PROC *q = running->child;
      while (q->sibling)
          q = q->sibling;
      q->sibling = p;
  }

  /* initialize the new proc and its stack */
  p->status = READY;
  p->priority = 1;         // for ALL PROCs except P0
  p->ppid = running->pid;
  p->parent = running;

  //                    -1   -2  -3  -4  -5  -6  -7  -8   -9
  // kstack contains: |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
  for (i=1; i<10; i++)
    p->kstack[SSIZE - i] = 0;

  p->kstack[SSIZE-1] = (int)func;
  p->saved_sp = &(p->kstack[SSIZE - 9]); 

  enqueue(&readyQueue, p);

  return p->pid;
}

int kexit(int value)
{
  // 將目前的process的exitCode設為value
  // 將目前的process的狀態設為zombie
  // 將目前的process優先度設為0
  
  /* 把目前process的所有子process接到目前process的父process上
	1. 紀錄process 1 的第一個child
		(宣告 PROC* 變數名稱 = &proc[1]; 變數名稱 = 變數名稱->child;)
	2. 將前一步的process指向下一個sibling直到process的sibling = NULL(0) 
		(此時該process指向process 1的最後一個子process)
	3. 將前一步process的sibling接上目前process的child */
  
  /* 將所有目前process的child，其parent設為process 1
	1. 紀錄目前process的第一個child
	2. 將前一步process的parent改為process 1(&proc[1])
	3. 同上一步，將process的ppid設為1
	4. 指向下一個sibing
	5. 循環直到沒有其他的sibling */
  
  /* 如果目前process的parent狀態是sleep則
	呼叫kwakeup(目前process->parent->event)來喚醒父process */
  
  // 呼叫tswitch釋出CPU
} 

int do_fork(void)
{
   int child = kfork(body);
   if (child < 0)
      printf("kfork failed\n");
   else{
      printf("proc %d kforked a child = %d\n", running->pid, child); 
      printList("readyQueue", readyQueue);
   }
   return child;
}

int do_switch(void)
{
   printf("proc %d switch task\n", running->pid);
   tswitch();
   printf("proc %d resume\n", running->pid);
}

char *pstatus[]={"FREE   ","READY  ","SLEEP  ","ZOMBIE ", "RUNNING"};

int do_exit(void)
{
  // 宣告一個int用來存exitCode的值
  
  /* 如果目前process的pid = 1則回報錯誤
	process 1 不會被exit(記得return不然後面會繼續跑) */
  
  // 印字串要求使用者輸入exit code的值
  // scanf
  /* 以下兩行code接在scanf後可以用來清除buffer
	(使用者多輸入時造成buffer沒清空，導致下次scanf會先從buffer裡找值)
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {} */
  
  // 呼叫kexit(exitCode的值)
} 

int do_ps(void)
{
  int i; PROC *p;
  printf("pid   ppid    status    event\n");
  printf("-----------------------------\n");
  for (i=0; i<NPROC; i++){
     p = &proc[i];
     printf("%2d%7d", p->pid, p->ppid);
     if (p==running)
       printf("%13s", pstatus[4]);
     else
       printf("%13s", pstatus[p->status]);
	 printf("%6d\n", p->event);
  }
}

int do_sleep(void){
	
	/* 如果目前process的pid = 1則回報錯誤
	process 1 不會被sleep"指令"改變狀態(不是不會sleep)(記得return不然後面會繼續跑) */
	
	// 宣告一個int用來存event的值
	// 印字串要求使用者輸入event的值
	// scanf
	/* 以下兩行code接在scanf後可以用來清除buffer
		(使用者多輸入時造成buffer沒清空，導致下次scanf會先從buffer裡找值)
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {} */
	
	// 呼叫ksleep(event的值)
}

int do_wakeup(void){
	// 宣告一個int用來存event的值
	// 印字串要求使用者輸入event的值
	// scanf
	/* 以下兩行code接在scanf後可以用來清除buffer
		(使用者多輸入時造成buffer沒清空，導致下次scanf會先從buffer裡找值)
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {} */
	
	// 呼叫kwakeup
}

int do_wait(void){
	// 宣告一個int用來存exitCode的值
	// 宣告一個int用來存kwait的回傳值並呼叫kwait(&1.宣告的int)

	/* 如果回傳值 < 0則回報錯誤，不存在子process
		如果回傳值 > 0則告知使用者該zombie child已經free */
}

int ksleep(int event){
	
	/* 如果目前event的值 <= 0則回報錯誤
	event的值必須大於0 (記得return不然後面會繼續跑) */
	
	// 將目前process的event設為傳進此函數的event值
	// 將目前process的狀態設為SLEEP

	// 將目前process放進sleep list(方法參考77行)
	// 呼叫tswitch釋出CPU
}

int kwakeup(int event){		
	
	/* 根據傳進函數event的值來決定要改變哪個process的狀態(或找不到)
		1. 在sleep list中找符合條件的process需要兩個process指標
		，其中一個指向目前尋找的，另一個指向目前尋找的前一個
		2. 先將前述所說之兩個指標都先指向sleep list的開頭
		3. 宣告一個bool型態紀錄使否有在sleep list中找到event值，初始為找不到
		4. 如果目前尋找的process的event值等於要尋找的event值則
		分為以下兩種情形，宣告一個bool型態紀錄是(1)還是(2)
			(1). 找到的process是sleep list的開頭
			(2). 找到的process不是sleep list的開頭
		--5. 如果是(1)，則呼叫dequeue(方法參考46行)
		   如果是(2)，尋找到的process的前一個process(在1. 中宣告)，
			   其下一個process指向找到的process的下一個process
		-6. 將找到的process放入ready queue(方法參考77行)
		-7. 將找到的process的event設為0
		-8. 將找到的process的狀態設為ready，並將3. 宣告的bool改為已找到，並告知使用者以喚醒該process
		--9. 如果4. 的結果為(1)則將目前找到的process和前一個找到的process都重新設為sleep list的開頭
			如果為(2)則將目前找到的process設為目前找到的process的下一個
		10. 如果3.判斷不相同則
			將找到的process的前一個process設為找到的process
			將找到的process設為list中的下一個process
			跳回到3.做判斷直到已經指到list中的底部
		11. 如果都沒找到(根據3.)回報使用者錯誤 */
}

int kwait(int *status){
	// 如果目前process沒有chlid則return -1
	
	/* 在目前process中尋找是否有child process狀態是ZOMBIE(或找不到)
		1. 找符合條件的process需要兩個process指標
		，其中一個指向目前尋找的，另一個指向目前尋找的前一個
		2. 先將前述所說之兩個指標都先指向目前process的child
		3. 如果目前尋找的process的狀態等於ZOMBIE則
		3-1. 先宣告一個int保存找到的process的pid
		3-2. 將傳進來的參數status指向找到的process的exitCode(*status = 名稱->exitCode)
		3-3. 將尋找到的process狀態設為FREE
		3-4. 再分為以下兩種情形
			(1). 找到的process是目前process第一個child
			(2). 找到的process不是目前process第一個child
		-4. 如果是(1)，則將目前process的child指向尋找到的process的sibling
		     如果是(2)，則將尋找到的process的前一個process的sibling，指向尋找到的process的sibling
		5. 將找到的process的parent設為NULL(0)
		6. 將找到的process的sibling設為NULL(0)
		7. 將找到的process的ppid設為0
		8. 將找到的process放入free list中(方法參考77行)
		9. 回傳3-1.保存的int
		10. 如果3.判斷不相同則
			將找到的process的前一個process設為找到的process
			將找到的process設為其sibling
			跳回到3.做判斷直到已經指到NULL(0)
		*/
	
	/* 如果沒有child狀態是ZOMBIE則將目前的process設為sleep
		1. 呼叫ksleep(ZOMBIE) 
		2. return 0 */
}

int body(void)
{
  char command[64];
  printf("proc %d start from body()\n", running->pid);
  while(1){
    printf("***************************************\n");
    printf("proc %d running: Parent=%d  child = ", running->pid, running->ppid);
	
	PROC* p = running->child;
	while (p != 0){
		printf("[%d %s]->", p->pid, pstatus[p->status]);
		p = p->sibling;
	}
	puts("NULL");

    printList("freeList ", freeList);
	printList("sleepList", sleepList);
    printList("readQueue", readyQueue);
    
    printf("input a command: [ps|fork|switch|exit|sleep|wakeup|wait] : ");
    fgets(command, 64, stdin);
    command[strlen(command)-1] = 0;
	
	int index = findCmd(command);
	if (index >= 0){
		fptr[index]();
	}
	else {
		puts("invalid command");
	}
  }
}

int init()
{
  int i;
  for (i = 0; i < NPROC; i++){
    proc[i].pid = i; 
    proc[i].status = FREE;
    proc[i].priority = 0;
    proc[i].next = (PROC *)&proc[(i+1)];
  }
  proc[NPROC-1].next = 0;
 
  freeList = &proc[0];
  sleepList = 0;  
  readyQueue = 0;

  // create P0 as the initial running process
  running = dequeue(&freeList);
  running->ppid = 0;
  running->status = READY;
  running->priority = 0;  
  running->parent = running;
  printList("freeList", freeList);
  printf("init complete: P0 running\n"); 
}

/*************** main() ***************/
int main()
{
   printf("\nWelcome to UTCS Multitasking System\n");
   init();
   printf("P0 fork P1\n");
   kfork(body);  

   while(1){
     if (readyQueue){
        printf("P0: switch task\n");
        tswitch();
     }
   }
}

/*********** scheduler *************/
int scheduler()
{ 
  printf("proc %d in scheduler()\n", running->pid);
  if (running->status == READY)
      enqueue(&readyQueue, running);
  printList("readyQueue", readyQueue);
  running = dequeue(&readyQueue);
  printf("next running = %d\n", running->pid);  
}

int findCmd(char *command){
   int i = 0;
   while(cmds[i]){
     if (strcmp(command, cmds[i]) == 0)
         return i;
     i++;
   }
   return -1;
}
