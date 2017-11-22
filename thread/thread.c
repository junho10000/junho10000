#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define BUFFSIZE 20000
#define P_SIZE 4000

int procession_A[P_SIZE][P_SIZE];
int procession_B[P_SIZE][P_SIZE];
int procession_C[P_SIZE][P_SIZE];

int main() {
	
	char buf_A[BUFFSIZE];
	char buf_B[BUFFSIZE];
	char *number_B;	
	char *number_A;
	FILE *fp_A;
	FILE *fp_B;
	int i,j;

	struct timeval start, end;
	gettimeofday(&start, NULL);
	printf("WHERE\n");
	fp_A = fopen("A.txt", "r");
	fp_B = fopen("B.txt", "r");
	printf("WHERE2\n");
	for(j=0; j<P_SIZE; j++) {
		fgets(buf_A, BUFFSIZE, fp_A);	
		number_A = strtok(buf_A, " ");
		for(i=0; i<P_SIZE; i++) {
			procession_A[j][i] = atoi(number_A);
			number_A = strtok(NULL, " ");
		}
		fflush(fp_A);
	}

	for(j=0; j<P_SIZE; j++) {
		fgets(buf_B, BUFFSIZE, fp_B);		
		number_B = strtok(buf_B, " ");
		for(i=0; i<P_SIZE; i++) {			
			procession_B[j][i] = atoi(number_B);
			number_B = strtok(NULL, " ");
		}
		fflush(fp_B);
	}
	/*
	printf("procession A\n");
	for(j=0; j<P_SIZE; j++) {
		for(i=0; i<P_SIZE; i++) {
			printf("%d ", procession_A[j][i]);
		}
		printf("\n");
	}

	printf("procession B\n");
	for(j=0; j<P_SIZE; j++) {
		for(i=0; i<P_SIZE; i++) {
			printf("%d ", procession_B[j][i]);
		}
		printf("\n");
	}
*/
	int k;
	for(k=0; k<P_SIZE; k++) {
		for(j=0; j<P_SIZE; j++) {
			for(i=0; i<P_SIZE; i++) {
				procession_C[k][j] += procession_A[k][i]*procession_B[i][j];
			}
		}
	}
/*
	printf("Procession C\n");
	for(j=0; j<P_SIZE; j++) {
		for(i=0; i<P_SIZE; i++) {
			printf("%d ",procession_C[j][i]);
		}
		printf("\n");
	}
	*/
	gettimeofday(&end, NULL);
	end.tv_usec = end.tv_usec - start.tv_usec;
	end.tv_sec = end.tv_sec - start.tv_sec;

	end.tv_usec += end.tv_sec*1000000;

	printf("Processing time: %lf sec\n", end.tv_usec/1000000.0);
	getchar();	

	return 0;
}
