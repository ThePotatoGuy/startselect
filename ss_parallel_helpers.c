/**
 * @author Andre Allan Ponce
 * andreponce@null.net
 * 
 * These functions help parallel methods to their stuff
 */

#ifdef USEMATH
#include <math.h>
#endif /* USEMATH not defined */

#include <stdlib.h>

#include <pthread.h>

#include "ss_parallel_helpers.h"

#include "ss_config.h"

/*	FUNCTIONS	======================================================*/

#ifdef USE_WORK_DIV

big_num ph_calculate_end_index(
    unsigned long thread_id, 
    unsigned long thread_count, 
    big_num size
){
	return (( (thread_id+1)*size) / thread_count) -1 ;
}

big_num ph_calculate_range(
    unsigned long thread_id, 
    unsigned long thread_count, 
    big_num size
){
	return ph_calculate_end_index(thread_id,thread_count,size)
        - calculate_start_index(thread_id,thread_count,size) + 1;
}

big_num ph_calculate_start_index(
    unsigned long thread_id, 
    unsigned long thread_count, 
    big_num size
){
	return (thread_id * size)  / thread_count ;
}

big_num ph_factorial(int num){
	if(num == 1){
		return 1;
	}
	return ((big_num)num)*ph_factorial(num-1);
}

#endif /* USE_WORK_DIV */

int ph_generate_random_int(int max){
    return rand() % max;
}

long ph_generate_random_long(long max){
    return rand() % max;
}

void ph_add(pthread_mutex_t *mutex, int *variable, int value){
    /* lock the variable, then add */
    pthread_mutex_lock(mutex);
    *variable += value;
    pthread_mutex_unlock(mutex);
} /* ph_add */

void ph_broadcast(
    pthread_mutex_t *mutex, 
    pthread_cond_t *cond, 
    int *condition
){
	
	/* signal the broadcast */
	pthread_mutex_lock(mutex);
	*condition = 1;
	pthread_cond_broadcast(cond);
	pthread_mutex_unlock(mutex);
	
} /* ph_broadcast */

void ph_broadcast_wait(
    pthread_mutex_t *mutex, 
    pthread_cond_t *cond, 
    int *condition
){
	
	/* wait until the broadcast has been sent */
	pthread_mutex_lock(mutex);
	while(!(*condition)){
		pthread_cond_wait(cond,mutex);
	}
	pthread_mutex_unlock(mutex);
	
} /* ph_broadcast_wait */

int ph_get(pthread_mutex_t *mutex, int *variable){
    /* containers */
    int var;    /* the value of variable */

    /* lock access to the variable, then get */
    pthread_mutex_lock(mutex);
    var = *variable;
    pthread_mutex_unlock(mutex);

    return var;
} /* ph_get */

void ph_set(pthread_mutex_t *mutex, int *variable, int value){
    /* lock the variable, then set it */
    pthread_mutex_lock(mutex);
    *variable = value;
    pthread_mutex_unlock(mutex);
} /* ph_set */

void ph_signal(pthread_mutex_t *mutex, pthread_cond_t *cond, int *condition){
	
	/* signal the condition */
	pthread_mutex_lock(mutex);
	*condition = 1;
	pthread_cond_signal(cond);
	pthread_mutex_unlock(mutex);
	
} /* ph_signal */

void ph_sub(pthread_mutex_t *mutex, int *variable, int value){
    /* lock the variable, then subtract */
    pthread_mutex_lock(mutex);
    *variable -= value;
    pthread_mutex_unlock(mutex);
} /* ph_sub */

void ph_wait(pthread_mutex_t *mutex, pthread_cond_t *cond, int *condition){
	
	/* wait until the signal has been sent */
	pthread_mutex_lock(mutex);
	while(!(*condition)){
		pthread_cond_wait(cond,mutex);
	}
	/* set the condition back to 0 */
	*condition = 0;
	pthread_mutex_unlock(mutex);
	
} /* ph_wait */

#ifdef USEMATH

unsigned int ph_calculate_thread_spawn_size(
    unsigned long divisor, 
    unsigned long thread_id, 
    unsigned long thread_count
){
	unsigned int log_bse;
	
	/* dont make anymore threads */
	if(thread_id >= thread_count){
		return 0;
	}
	else{
		log_bse = ph_find_smallest_expo_of_two(divisor);
		
		/* fixes issues with odd/non-power-of-two numbers of threads */
		if(thread_count - log_bse <= thread_id){
			log_bse = thread_count - thread_id-1;
		}
		return log_bse;
	}
} /* ph_calculate_thread_spawn_size */

int ph_find_larger_power_of_two(int value){
	return (int) pow(2,ph_find_largest_expo_of_two(value));
} 

int ph_find_largest_expo_of_two(int value){
	return (int) ceil(ph_log_base(value, 2));
}

int ph_find_smaller_power_of_two(int value){
	return (int) pow(2,ph_find_smallest_expo_of_two(value));
}

int ph_find_smallest_expo_of_two(int value){
	return (int) floor(ph_log_base(value, 2));
}

double ph_log_base(double value, double base){
	return log(value) / log(base);
}

#endif /* USEMATH not defined */
