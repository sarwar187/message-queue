#include "mesg.h"

key_t key;
int msqid;
int msqid1;
message_buf1 sbuf;
    

int cache[100] = {0};



main()
{
    char c,str[80] = {"check"};
    int shmid,num,myid;
    int *shm, *s;
    key = 5678;
sbuf.mtype = 1;
    if ((shmid = shmget(key, SHMSZ, IPC_EXCL)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    s = shm;

    msqid1 = *s;

    if ((msqid = msgget(IPC_PRIVATE,IPC_CREAT|0666)) < 0) {
      perror("msgget");
        exit(1);
    }
    sbuf.mtype = 1;
    sbuf.id = 10;
    sbuf.check = 1; 
    sbuf.pid = getpid();
    sbuf.uid = getuid();
    sbuf.mqid = msqid;
    (void) strcpy(sbuf.mtext, "Did?");

    if (msgsnd(msqid1,&sbuf,sizeof(sbuf), IPC_NOWAIT) < 0) {
		perror("msgsnd1");                                  
		exit(1);
    }

    if (msgrcv(msqid,&sbuf,sizeof(sbuf), 0, 0) < 0) {
        	perror("msgrcv");
       		exit(1);
    }
    myid = sbuf.id;

    printf("\n\n***registration done for you!!!! your id is %d enjoy chatting!!!***\n",myid);
    int k;
    sbuf.id = 1;
	
    while(1){
	sbuf.pid = 0;
	//printf("checkpoint1\n");
	printf("\ntype the user id to check user availability\n"); 
	printf("or if user is available then use the user id to send message\n");
	printf("or type 0 to receive message\n\n");
	
	scanf("%d",&k);
	if(k == 0)
	{
		sbuf.check = 1;
		if(msgrcv(msqid, &sbuf,sizeof(sbuf), 0,IPC_NOWAIT) < 0) {
       			//perror("msgrcv");
			printf("\nyou have no message at this time\n");
		}
		if(sbuf.pid>0){
			printf("got a message from user %d\n\n",sbuf.pid);
			printf("the message is %s\n\n",sbuf.mtext);
		}
		continue;
	}
	sbuf.id = k;
		//THIS PORTION IS FOR AVAILABILITY CHECKING AND MESSAGE SENDING 
		if(cache[k] == 1)
		{
			sbuf.check = 3;	
			printf("user is available and cached!! So, type your message\n");
			gets(str);
			gets(str);			
			(void) strcpy(sbuf.mtext,str);
			sbuf.pid = myid;
			if (msgsnd(msqid1, &sbuf, sizeof(sbuf),0) < 0) {
     			        perror("msgsnd12");
     	 			exit(1);
   		        }
		}
		else
		{
			sbuf.check = 2;
			sbuf.pid = myid;
			if(msgsnd(msqid1, &sbuf, sizeof(sbuf),0) < 0) {
    				perror("msgsnd2");
     		 		exit(1);
   			}
			if(msgrcv(msqid, &sbuf,sizeof(sbuf), 0,0) < 0) {
   			    	perror("msgrcv");
  		    		exit(1);
  		 	}
			if(sbuf.id!=5000){
				cache[sbuf.id] = 1;
				printf("%d is available and cached in your databse\n",sbuf.id);
			}
			else
				printf("\n\n**Sorry user is not available at this time knock later**\n\n");
   		}
	//printf("checkpoint2\n");
	}    
    	exit(0);
}

