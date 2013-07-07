#include "mesg.h"

//below global variable declaration 

message_buf1 rbuf;
database db[100];  
int user = 0;        //number of users 
int msqid;

//this function is for the registration process

void reg(int user){
	db[user].id = user;
	db[user].pid = rbuf.pid;
	db[user].mqid = rbuf.mqid;
}

//registration function ends here

//the following function returns id if the user is available
//otherwise returns 0

int message(int id){
	int i;
	if((id>user)||(id<0))
		return 0;
	for(i=1;i<=user;i++){
		if(db[i].id == id)
			return id;
	}	
	return 0;
}




//availability checking function ends here

main()
{
    int shmid;         //shared memory id  
    key_t key;         //key of shared memory  
    int *shm, *s;      //pointer to the shared memory  
	//int msqid;         //message queue id of the server
    int available;     //contains 0 if user not available otherwise id

    key = 5678;

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    s = shm;
    
    if ((msqid = msgget(key, IPC_CREAT|0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    *s = msqid;
	
    int test = 0;
    while(1){
	if(msgrcv(msqid,&rbuf,sizeof(rbuf),0,0) < 0){
	       	perror("msgrcv1");	
       		exit(1);
    	}
	printf("this is test %d\n",test++);
    	if(rbuf.check == 1){
		//printf("ki hoilo\n");
		user++;
		available = 0;
		reg(user); 
		rbuf.id = user;
		//rbuf.mtype = 0;
		rbuf.pid = getpid();
		if (msgsnd(db[user].mqid, &rbuf, sizeof(rbuf),0) < 0) {
			perror("msgsnd1");
	     		exit(1);
		}
	}
	else if(rbuf.check == 2){
		int ack1,ack2,pid;
		ack1 = rbuf.pid;
		ack2 = rbuf.id;
		available = message(rbuf.id);
		pid = db[ack2].pid;
		if(available!=0){
			rbuf.mtype = 1;
			//printf("%d\n",rbuf.mqid);
			if (msgsnd(db[ack1].mqid, &rbuf, sizeof(rbuf),0) < 0) {
				perror("msgsnd2");
	      			exit(1);
			}
		}
		else{
			rbuf.id = 5000;
			rbuf.mtype = 1;
			if (msgsnd(db[ack1].mqid, &rbuf, sizeof(rbuf),0) < 0) {
				perror("msgsnd3");
	      			exit(1);
			}
		}
	}
	else{
		int ack;
		ack = rbuf.pid;
		available = rbuf.id;                    //this is the real message
		available = db[available].mqid;
		if(msgsnd(available, &rbuf, sizeof(rbuf),0) < 0) {
			perror("msgsnd4");
	      		exit(1);
		}
	}
    }
    exit(0);
}
