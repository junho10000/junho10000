#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

pid_t arr_pid[3];
int COUNT = 0;

static void sig_usr(int signo) {
	if(signo == SIGUSR1)
		printf("received USR1\n");
}


int main(int argc, char* argv[]) {
	FILE *fp;
	int i, tempNum, limit;
	pid_t pid;
	clock_t start, end;
	limit = atoi(argv[1]);

	struct sigaction act;
	act.sa_handler = sig_usr;	
	sigaction(SIGUSR1, &act, 0);	

	start = clock();
	if(argv[1] <= 0) {
		printf("The Argument is lower than 0\n");
	}
		

	fp = fopen(argv[2], "rt");
	fscanf(fp, "%d", &tempNum);
	fclose(fp);
	printf("/////////\n");
	
	//signal(SIGUSR1, sig_usr);
	usleep(10000);
	int temp;
	printf("Make 3 Processor\n");
	for(i=0; i<4; i++) {
		arr_pid[i] = fork();
		if(arr_pid[i] == 0)
		{	
			while(1) {
			pause();
			fp = fopen(argv[2], "rt");
			fscanf(fp, "%d", &tempNum);
			fclose(fp);
			
			fp = fopen(argv[2], "wt");
			fprintf(fp, "%d", tempNum+1);
			fclose(fp);
			
			printf("Processor[%d]  Save:%d\n",i,tempNum+1);
			usleep(1000);
			if(COUNT == limit) break;
			}
			exit(0);
		}
	}
	
	//kill(arr_pid[0], SIGUSR1);
	usleep(10000);

	while(1){

		for(i=1; i<4; i++) {
			printf("Calling Processor[%d]\n", i);
			kill(arr_pid[i], SIGUSR1);
			COUNT++;
			usleep(5000);
			if(COUNT == limit) {
				end = clock();
				printf("Total Processing Time: %.3lfms, %.3lfs\n",(double)end-start, (end-start)/(double)1000);
				return 0;
			}
		}
	}
		
	//printf("COUNT:%d\n", COUNT);

	printf("END\n");
	return 0;
}
