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

int main(void)
{

  /*Initial declarations*/
  int msgqid;
 
  struct msgbuf_st msg_buf; //To be used by msgrcv
  strncpy(msg_buf.data.source, "6134445555", NUM_LEN);
  strncpy(msg_buf.data.msgstr, "Hello!", MSGSTR_LEN);
  msg_buf.data.msgstr[MSGSTR_LEN - 1] = '\0';


  if ((msgqid = msgget(123, 0 )) == -1) { /* connect to the queue */
    perror("msgget");
    exit(1);
  }
  else
    printf("progA: connected to the queue %d. \n", msgqid);

  if(msgsnd(msgqid, &msg_buf, sizeof(struct data_st), 0) == -1){
    perror("msgsnd");
    exit(1);
  }
  else{
    printf("progA: message sent.\n");
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

