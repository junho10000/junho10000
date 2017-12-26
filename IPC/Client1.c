/*Sending Program*/

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define M_SIZE 1024

struct my_msg_st
{
        long int my_msg_type;
        char some_text[M_SIZE];
	int ch_N;
};


int main()
{
        int running=1;
        int msgid, msgid_r;
        char buffer[M_SIZE];
        struct my_msg_st some_data;
	

        msgid=msgget((key_t)1234, 0666 | IPC_CREAT);
	msgid_r = msgget((key_t)1237, 0666 | IPC_CREAT);
	printf("msgid: %d\n", msgid);
	
        if(msgid == -1)
        {
                fprintf(stderr, "msgget failed\n");
                exit(EXIT_FAILURE);
        }

        while(1)
        {
		printf("1. Clinet 2,  2. Client 3,  3. Else\n");
		

               	printf("Send Message: ");
                fgets(buffer, M_SIZE, stdin);
                some_data.my_msg_type= 1;
                strcpy(some_data.some_text, buffer);
		
                if(msgsnd(msgid, (void*)&some_data, M_SIZE, 0) == -1)
                {
                        fprintf(stderr, "msgsnd failed\n");
                        exit(EXIT_FAILURE);
                }

		if(msgrcv(msgid_r, (void *)&Msg_data, BUFSIZ, msg_to_receive, 0) != -1) 
		{
			printf("%s", Msg_data.some_text);
		}

        }
	return 0;

}
