/*Receiving program*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#define M_SIZE 1024

pthread_t thread1;
pthread_t thread2;
pthread_t thread3;



struct MsgType
{
        long int my_msg_type;
        char some_text[BUFSIZ];
	int ch_N;
};


int msgid1, msgid2, msgid3, msgid1_r, msgid2_r, msgid3_r;
long int msg_to_receive=0;
struct MsgType Msg_data;

void listener1(void *ptr) {
	msgid1=msgget((key_t)1234, 0666 | IPC_CREAT);

	msgid2_r=msgget((key_t)1238, 0666 | IPC_CREAT);
	msgid3_r=msgget((key_t)1239, 0666 | IPC_CREAT);
	 while(1)
        {    if(msgrcv(msgid1, (void *)&Msg_data, BUFSIZ, msg_to_receive, 0) != -1)
                {
			
			char CLI2[2000] = "[CLI2]";
			char CLI3[2000] = "[CLI3]";
			
			if(Msg_data.some_text[0] == '1') {
				strcat(CLI2,  Msg_data.some_text);
				strcpy(Msg_data.some_text, CLI2);
				msgsnd(msgid2_r, (void*)&Msg_data, M_SIZE, 0);
			}
			else if(Msg_data.some_text[0] == '2') {
				strcat(CLI3,  Msg_data.some_text);
				strcpy(Msg_data.some_text, CLI3);
				msgsnd(msgid3_r, (void*)&Msg_data, M_SIZE, 0);
			}
			else {
				 printf("Client1: %s", Msg_data.some_text);
			}
			
                }
	
        }
}

void listener2(void *ptr) {
	msgid2=msgget((key_t)1235, 0666 | IPC_CREAT);

	msgid1_r=msgget((key_t)1237, 0666 | IPC_CREAT);
	msgid3_r=msgget((key_t)1239, 0666 | IPC_CREAT);
	 while(1)
        {    if(msgrcv(msgid2, (void *)&Msg_data, BUFSIZ, msg_to_receive, 0) != -1)
                {
			
			char CLI1[2000] = "[CLI1]";
			char CLI3[2000] = "[CLI3]";
			
			if(Msg_data.some_text[0] == '1') {
				strcat(CLI1,  Msg_data.some_text);
				strcpy(Msg_data.some_text, CLI1);
				msgsnd(msgid1_r, (void*)&Msg_data, M_SIZE, 0);
			}
			else if(Msg_data.some_text[0] == '2') {
				strcat(CLI3,  Msg_data.some_text);
				strcpy(Msg_data.some_text, CLI3);
				msgsnd(msgid3_r, (void*)&Msg_data, M_SIZE, 0);
			}
			else {
				 printf("Client2: %s", Msg_data.some_text);
			}
			
                }
	
        }
}

void listener3(void *ptr) {
	msgid3=msgget((key_t)1236, 0666 | IPC_CREAT);

	msgid1_r=msgget((key_t)1237, 0666 | IPC_CREAT);
	msgid2_r=msgget((key_t)1238, 0666 | IPC_CREAT);
	 while(1)
        {    if(msgrcv(msgid3, (void *)&Msg_data, BUFSIZ, msg_to_receive, 0) != -1)
                {
			
			char CLI1[2000] = "[CLI1]";
			char CLI2[2000] = "[CLI2]";
			
			if(Msg_data.some_text[0] == '1') {
				strcat(CLI1,  Msg_data.some_text);
				strcpy(Msg_data.some_text, CLI1);
				msgsnd(msgid1_r, (void*)&Msg_data, M_SIZE, 0);
			}
			else if(Msg_data.some_text[0] == '2') {
				strcat(CLI2,  Msg_data.some_text);
				strcpy(Msg_data.some_text, CLI2);
				msgsnd(msgid2_r, (void*)&Msg_data, M_SIZE, 0);
			}
			else {
				 printf("Client3: %s", Msg_data.some_text);
			}
			
                }
	
        }

}

int main()
{
	char *msg1 = "Thread 1";
	char *msg2 = "Thread 2";
	char *msg3 = "Thread 3";

	pthread_create(&thread1, NULL, (void *) &listener1, (void *) msg1);
	pthread_create(&thread2, NULL, (void *) &listener2, (void *) msg2);
	pthread_create(&thread3, NULL, (void *) &listener3, (void *) msg3);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);


	return 0;
}
