/*
 * program_A.c - initially the clinent
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/errno.h>

extern int errno;       // error NO.
//#define MSGPERM 0600    // msg queue permission
#define MSGSTR_LEN 255  // msg text length: 255 + 1 null character
#define NUM_LEN 11      // number buffer length: 10 + 1 nunll character

struct data_st{
  char source[NUM_LEN];
  char msgstr[MSGSTR_LEN];
};

struct msgbuf_st{
  long mtype;
  struct data_st data;
};

int cnt = 0;  //counts how many iterations of message recieving there have been

int main(void)
{

  /*Initial declarations*/
  int msgqid, //message queue identifier 
      ret;  //will hold return value of message recieve
  char *messages[5] = {"What", "colour", "is", "pandas", "fur?"};
 
  struct msgbuf_st msg_buf; //To be used by msgrcv
  msg_buf.mtype = 16;
  strncpy(msg_buf.data.source, "6134445555", NUM_LEN);
  //char s[MSGSTR_LEN];
  //gets(s);

  if ((msgqid = msgget(1234, 0 )) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
    }
  else
  printf("progA: connected to the queue %d. \n", msgqid);

  int i; //for loop iterator
  char s[1];
  for(i =0 ; i < 5 ; i++){
    gets(s);
    printf("progA: Iteration: %d ",i+1 );
    strncpy(msg_buf.data.msgstr, messages[i], MSGSTR_LEN);
    msg_buf.data.msgstr[MSGSTR_LEN - 1] = '\0';


    if(msgsnd(msgqid, &msg_buf, MSGSTR_LEN*sizeof(char), 0) == -1){
      perror("msgsnd");
      exit(1);
    }
    else{
      printf(" Message sent.\n");
    }
  }

  for(;cnt<5;){
    printf("\nprogA: Iteration: %d ",cnt + 1);
      ret = msgrcv(msgqid, &msg_buf, sizeof(struct data_st), 0, 0);
    if(ret == -1 ){
      perror("msgrcv");
      exit(1);
    }
    else{
      printf(" message received.\n");
      printf("progA: Received %d bytes from message queue.\n", ret);
      printf("progA: Message payload: Source: %s\n", msg_buf.data.source);
      printf("progA: Message payload: Message string: \"%s\"\n", msg_buf.data.msgstr);
      cnt++;
    }
  }

  /*if(msgrcv(msgqid, &msg_buf, sizeof(struct data_st),0, 0) == -1){
    perror("msgrcv");
    exit(1);
  }
  else{
    printf("progA: message recieved.\n");
  }*/

  
  

  

  return 0;
}

