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
	struct timeval start_time,current_time,end_time;
	double diff_time;
	gettimeofday(&start_time,NULL);
	int i=0,count=0,j=0;
	while(i==0){
		if(pthread_spin_trylock(&lock->s_lock)==0){
			//i++;
			return 0;
		}
		/* 1sec spinlock until get lock */
		gettimeofday(&current_time,NULL);
		count++;
		diff_time=current_time.tv_sec-start_time.tv_sec;
		diff_time += (current_time.tv_usec - start_time.tv_usec) / 1000000.0;//1sec
		if(diff_time>1){
			
			break;
		}

	}
/*
	//And if didn't catch the spin lock--> mutex_lock()
	while(1) {
		if(pthread_mutex_trylock(&lock->m_lock)==0){
			return 0;
		}
	}
*/

	//if catch the spinlock in 1sec
	if(i!=0){
		pthread_spin_unlock(&lock->s_lock);
		if(pthread_mutex_trylock(&lock->m_lock)!=0){
			while(1){
				if(pthread_mutex_trylock(&lock->m_lock)==0){
					j++;
					//return 0;
				}
				//to catch the reamin 1sec 
				
				gettimeofday(&end_time,NULL);
				diff_time=end_time.tv_sec-current_time.tv_sec;
	         		diff_time += (end_time.tv_usec - current_time.tv_usec) / 1000000.0;
        	 		if(diff_time>1){
            				break;
        			}
			}
		}
		else{
			j++;
			//return 0;
		}
	}
	
	/*to catch the reamin 1sec */
	//And if didn't catch the spin lock--> mutex_lock()
	gettimeofday(&end_time,NULL);
	diff_time=end_time.tv_sec-start_time.tv_sec;
	diff_time +=(end_time.tv_usec-start_time.tv_usec)/ 1000000.0;
	
	pthread_mutex_lock(&lock->m_lock);//
	if(i==0&j==0){
		pthread_spin_lock(&lock->s_lock);
		
	}
	return 0;
	//printf("total spin time : %lf\n",diff_time);
	//printf("gettimeofday count : %d\n",count);
}

int hybrid_lock_unlock(struct hybrid_lock *lock)
{
	if(pthread_spin_unlock(&lock->s_lock)!=0)
		return -1;
	if(pthread_mutex_unlock(&lock->m_lock)!=0)
		return 1;
	return 0;
}
