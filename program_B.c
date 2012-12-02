#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/errno.h>

extern int errno;       // error NO.
//#define MSGPERM 0600    // msg queue permission
#define MSGSTR_LEN 255  // msg text length: 255 + 1 null character
#define NUM_LEN 11 			// number buffer length: 10 + 1 null character

int msgqid, ret;
int done;

struct data_st{
	char source[NUM_LEN];
	char msgstr[MSGSTR_LEN];
};

struct msgbuf_st{
	long mtype;
	struct data_st data;
};

int main(int argc,char **argv)
{
	// create a message queue. If here you get a invalid msgid and use it in msgsnd() or msgrcg(),
	//an Invalid Argument error will be returned.
	msgqid = msgget(123, 0);use 
	if (msgqid < 0) {
		perror(strerror(errno));
		printf("progB: failed to create message queue with msgqid = %d\n", msgqid);
		return 1;
	}
	printf("progB: message queue %d created\n",msgqid);

	/*Receives a message sent by msgsnd*/
	struct msgbuf_st msg_buf; //To be used by msgrcv

	for(;;){
  ret = msgrcv(msgqid, &msg_buf, sizeof(struct data_st), 0, 0);
		if(ret == -1 ){
			perror("msgrcv");
			exit(1);
		}
		else
			printf("progB: message received.\n");

		printf("Received %d bytes from message queue.\n", ret);
		printf("Message payload: Source: %s\n", msg_buf.data.source);
		printf("Message payload: Message string: %s\n", msg_buf.data.msgstr);
	}

	if (msgctl(msgqid, IPC_RMID, NULL) == -1) {
		/* As an example for checking errno. */
		if (errno == EIDRM)
			fprintf(stderr, "Message queue already removed.\n");
		else
			perror("Error while removing message queue");
	}
	else{
		printf("progB: message queue removed.\n");

	}

	return 0;
}
