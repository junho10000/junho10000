#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo);
static void sig_int(int signo);
unsigned int sleep2(unsigned int seconds);
void catchint(int signo);

int main(void)
{
	unsigned int unslept;
	pid_t pid;	
	
	if(pid=fork()) {
		if(signal(SIGINT, sig_int) ==SIG_ERR) {
			exit(1);
		}
		sleep(5);
		exit(0);
	}
	
		
	unslept = sleep2(5);

	printf("sleep2 returned__: %u\n", unslept);
	return 0;
}

static void sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
}

static void sig_int(int signo)
{
	int i,j;
	volatile int k;

	printf("\nsig_int starting\n");

	/*
	 * 아래 for문이 5초 이상 실행되도록 적당히 바꿔주세요. 
	 */
	for (i = 0; i<500000; i++)
		for (j = 0; j<5000; j++)
			k += i*j;
	
	printf("sig_int finished\n");
}

void catchint(int signo) {
	unsigned int unslept;
	sleep(alarm(0));
}


unsigned int sleep2(unsigned int seconds)
{
	static struct sigaction act ;

	act.sa_handler = catchint;
	//sigfillset(&(act.sa_mask));
	sigaction(SIGINT, &act, NULL);

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		return seconds;
	
	if (setjmp(env_alrm) == 0){
		alarm(seconds);
		pause();
	}
	return alarm(0);
}

