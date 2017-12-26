/*Sending Program*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define M_SIZE 1024

long int msg_to_receive=0;
struct MsgType
{
        long int my_msg_type;
        char some_text[BUFSIZ];
};


int main()
{

        int msgid, msgid_r;
        char buffer[BUFSIZ];
        struct MsgType Msg_data;
	

        msgid=msgget((key_t)1235, 0666 | IPC_CREAT);
	msgid_r = msgget((key_t)1238, 0666 | IPC_CREAT);
	printf("msgid: %d\n", msgid);
	
        if(msgid == -1)
        {
                fprintf(stderr, "msgget failed\n");
                exit(EXIT_FAILURE);
        }

        while(1)
        {
               printf("1. Clinet 1,  2. Client 3,  3. Else\n");
		

               	printf("Send Message: ");
                fgets(buffer, M_SIZE, stdin);
                Msg_data.my_msg_type= 1;
                strcpy(Msg_data.some_text, buffer);
		
                if(msgsnd(msgid, (void*)&Msg_data, M_SIZE, 0) == -1)
                {
                        fprintf(stderr, "msgsnd failed\n");
                      	return 0;
                }

		if(msgrcv(msgid_r, (void *)&Msg_data, BUFSIZ, msg_to_receive, 0) != -1) 
		{
			printf("%s", Msg_data.some_text);
		}
        }
	return 0;

}
