#include "hybrid_lock.h"
#include <sys/time.h>

int hybrid_lock_init(struct hybrid_lock *lock)
{
	if(pthread_mutex_init(&lock->m_lock,NULL)!=0)
		return 1;
	if(pthread_spin_init(&lock->s_lock,PTHREAD_PROCESS_PRIVATE)!=0)
		return -1;
	return 0;
}

int hybrid_lock_destroy(struct hybrid_lock *lock)
{
	if(pthread_spin_destroy(&lock->s_lock)!=0)
		return -1;
	if(pthread_mutex_destroy(&lock->m_lock)!=0)
		return 1;
	return 0;
}

int hybrid_lock_lock(struct hybrid_lock *lock)
{
	time_t start, end;
	start = time(NULL);
	double diff_time;

	int i=0,j=0;
	int count=0;

	while(1){
		count++;
		if(pthread_spin_trylock(&lock->s_lock)==0){
			//printf("catch spin\n");
			break;
		}
		/* 1sec spinlock until get lock */
		end = time(NULL);
		diff_time = difftime(end, start);
		if(diff_time>=1) {
			//printf("fail to spin\n");
			pthread_mutex_lock(&lock->m_lock);
			//break;
			return 0;
		}

	}

	while(1){
		count++;
		if(pthread_mutex_trylock(&lock->m_lock)==0) {
			pthread_spin_unlock(&lock->s_lock);
			//return 0;
			break;
		}
		end = time(NULL);
		diff_time = difftime(end, start);

		if(diff_time>=1) {
			//return 0;
			break;
		}
	}


	
	
	
	printf("difftime count : %d\n",count);
	printf("total spin time : %lf\n",diff_time);
	return 0;
}

int hybrid_lock_unlock(struct hybrid_lock *lock)
{
	if(pthread_spin_unlock(&lock->s_lock)!=0)
		return -1;
	if(pthread_mutex_unlock(&lock->m_lock)!=0)
		return 1;
	return 0;
}
