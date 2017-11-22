#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define BUFFSIZE 200000
#define P_SIZE 4000

pthread_mutex_t g_mutex;


int procession_A[P_SIZE][P_SIZE];
int procession_B[P_SIZE][P_SIZE];
int procession_C[P_SIZE][P_SIZE];



void *thr_fn1(void *arg) {
	printf("THR_FN1\n");
	
	
	FILE *fp_A;
	FILE *fp_B;
	FILE *fp_C;

	
	fp_A = fopen("A.txt", "r");
	fp_B = fopen("B.txt", "r");
	fp_C = fopen("C1.txt", "wa");
	
	
	char buf_A[BUFFSIZE];
	char buf_B[BUFFSIZE];
	
	char *number_B;	
	char *number_A;

	int i,j;
	char temp[10000];
	


	
	pthread_mutex_lock(&g_mutex);
	for(j=0; j<P_SIZE/2; j++) {
		//printf("1AJ:%d\n", j);
		fgets(buf_A, BUFFSIZE, fp_A);	
		number_A = strtok(buf_A, " ");
		//fputs(buf_A, stdout);
		
		for(i=0; i<P_SIZE; i++) {
			//printf("1AI:%d\n", i);
			
			procession_A[j][i] = atoi(number_A);
			number_A = strtok(NULL, " ");
		}
		
		fflush(fp_A);
	}
	//printf("WHERE2\n");
	for(j=0; j<P_SIZE; j++) {
		//printf("1BJ:%d\n", j);
		fgets(buf_B, BUFFSIZE, fp_B);		
		number_B = strtok(buf_B, " ");
		for(i=0; i<P_SIZE; i++) {	
		//	printf("1BI:%d\n", i);	
			procession_B[j][i] = atoi(number_B);
			number_B = strtok(NULL, " ");
		}
		fflush(fp_B);
	}
	pthread_mutex_unlock(&g_mutex);
	
	int k;
	for(k=0; k<P_SIZE/2; k++) {
		//printf("fn1_K: %d\n", k);
		for(j=0; j<P_SIZE; j++) {
			for(i=0; i<P_SIZE; i++) {
				procession_C[k][j] += procession_A[k][i]*procession_B[i][j];
			}
		}
	}


	for(j=0; j<P_SIZE/2; j++) {
		for(i=0; i<P_SIZE; i++) {
			sprintf(temp, "%4d ",procession_C[j][i]);
			fputs(temp, fp_C);
			fflush(fp_C);
		}
		sprintf(temp, "\n");
		fputs(temp, fp_C);
		fflush(fp_C);
	}
printf("fn1_finish\n");

}

void *thr_fn2(void *arg) {
	printf("THR_FN2\n");
	
	
	FILE *fp_A;
	FILE *fp_B;
	FILE *fp_C;


	
	fp_A = fopen("A.txt", "r");
	fp_B = fopen("B.txt", "r");
	fp_C = fopen("C2.txt", "wa");
	

	char buf_A[BUFFSIZE];
	char buf_B[BUFFSIZE];

	char *number_B;	
	char *number_A;

	int i,j;
	char temp[10000];


	pthread_mutex_lock(&g_mutex);
	for(j=0; j<P_SIZE; j++) {
		printf("2AJ:%d\n", j);
		fgets(buf_A, BUFFSIZE, fp_A);
		//fputs(buf_A, stdout);	
		number_A = strtok(buf_A, " ");
		for(i=0; i<P_SIZE; i++) {
			//printf("2AI:%d\n", i);
			//printf("2AI_NUM:%s\n", number_A);
			procession_A[j][i] = atoi(number_A);
			//printf("2AIAI:%d\n", procession_A[j][i]);
			number_A = strtok(NULL, " ");
		}
		fflush(fp_A);
	}
	
	//printf("WHERE2\n");
	for(j=0; j<P_SIZE; j++) {
		printf("2BJ:%d\n", j);
		fgets(buf_B, BUFFSIZE, fp_B);		
		number_B = strtok(buf_B, " ");
		for(i=0; i<P_SIZE; i++) {	
			//printf("2BI:%d\n", i);
			procession_B[j][i] = atoi(number_B);
			//printf("2BIBI:%d\n", procession_B[j][i]);
			number_B = strtok(NULL, " ");
		}
		fflush(fp_B);
	}
	pthread_mutex_unlock(&g_mutex);
	

	int k;
	for(k=P_SIZE/2; k<P_SIZE; k++) {
		printf("fn2_K: %d\n", k);
		for(j=0; j<P_SIZE; j++) {
			for(i=0; i<P_SIZE; i++) {
				procession_C[k][j] += procession_A[k][i]*procession_B[i][j];
			}
		}
	}
	

	for(j=P_SIZE/2; j<P_SIZE; j++) {
		for(i=0; i<P_SIZE; i++) {
			sprintf(temp, "%4d ",procession_C[j][i]);
			fputs(temp, fp_C);
			fflush(fp_C);
		}
		sprintf(temp, "\n");
		fputs(temp, fp_C);
		fflush(fp_C);
	}
printf("fn2_finish\n");

}

int main() {
	printf("START\n");
	FILE *fp1, *fp2, *fp3;
	struct timeval start, end;
	pthread_t tid1, tid2;
	int rc;
	char buf[BUFFSIZE];
	

	gettimeofday(&start, NULL);

	pthread_mutex_init(&g_mutex, NULL);
	pthread_create(&tid1, NULL, thr_fn1, NULL);
	pthread_create(&tid2, NULL, thr_fn2, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
		
	fp1 = fopen("C1.txt", "r");
	fp2 = fopen("C2.txt", "r");
	fp3 = fopen("C.txt", "wa");


	while(fgets(buf, BUFFSIZE, fp1)!=NULL) {
		fputs(buf, fp3);
		fflush(fp3);
	}
	
	while(fgets(buf, BUFFSIZE, fp2)!=NULL) {
		fputs(buf, fp3);
		fflush(fp3);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);	
	
	gettimeofday(&end, NULL);
	end.tv_usec = end.tv_usec - start.tv_usec;
	end.tv_sec = end.tv_sec - start.tv_sec;

	end.tv_usec += end.tv_sec*1000000;

	printf("Processing time: %lf sec\n", end.tv_usec/1000000.0);
	getchar();


	return 0;
}
