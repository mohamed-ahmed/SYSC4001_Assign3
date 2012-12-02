#include<string.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<sys/errno.h>

extern int errno;       // error NO.
#define MSGPERM 0600    // msg queue permission
#define MSGTXTLEN 128   // msg text length

int msgqid, rc;
int done;

struct msg_buf {
  long mtype;
  char mtext[MSGTXTLEN];
} msg;

int main(int argc,char **argv)
{
  // create a message queue. If here you get a invalid msgid and use it in msgsnd() or msgrcg(), 
	//an Invalid Argument error will be returned.
  msgqid = msgget(IPC_PRIVATE, MSGPERM|IPC_CREAT|IPC_EXCL);
  if (msgqid < 0) {
    perror(strerror(errno));
    printf("failed to create message queue with msgqid = %d\n", msgqid);
    return 1;
  }
  printf("message queue %d created\n",msgqid);

  // message to send
  msg.mtype = 1; // set the type of message
  sprintf (msg.mtext, "%s\n", "a text msg..."); /* setting the right time format by means of ctime() */

  // send the message to queue
  rc = msgsnd(msgqid, &msg, sizeof(msg.mtext), 0); // the last param can be: 0, IPC_NOWAIT, 
  //MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR.
  if (rc < 0) {
    perror( strerror(errno) );
    printf("msgsnd failed, rc = %d\n", rc);
    return 1;
  }

  // read the message from queue
  rc = msgrcv(msgqid, &msg, sizeof(msg.mtext), 0, 0);
  if (rc < 0) {
    perror( strerror(errno) );
    printf("msgrcv failed, rc=%d\n", rc);
    return 1;
  }
  printf("received msg: %s\n", msg.mtext);

  // remove the queue
  rc=msgctl(msgqid,IPC_RMID,NULL);
  if (rc < 0) {
    perror( strerror(errno) );
    printf("msgctl (return queue) failed, rc=%d\n", rc);
    return 1;
  }
  printf("message queue %d is gone\n",msgqid);

  return 0;
}