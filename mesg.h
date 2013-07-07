#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27

#define MSGSZ     128

//the following database is the message structure 

typedef struct msgbuf1 {
  int id;
	int pid;
	int uid;
	int check;	
	int mqid;
	long mtype;
	char mtext[100];
}message_buf1;

//the following structure is used to hold the chat program users entry for the server

typedef struct data_base{
	int id;	
	int pid;
	int mqid;
}database;
