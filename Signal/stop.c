#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


static void sig_fn(int signo)
{
	printf("\nSignal_Number:%d\n", signo);
	printf("\nCtrl-C is pressed. Try Again\n");
}

int main() {
	struct sigaction act;
	act.sa_handler = sig_fn;
	sigaction(SIGINT, &act, 0);
	
	
	sleep(5);

	return 0;
}

