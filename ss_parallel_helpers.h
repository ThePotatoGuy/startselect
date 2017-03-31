/**
 * @author Andre Allan Ponce
 * andreponce@null.net
 * 
 * These functions help parallel methods do their stuff
 */

#ifndef SS_P_HELP_H
#define SS_P_HELP_H

#include "ss_config.h"

/*	FUNCTIONS	======================================================*/

/* these functions are for dividing work for each thread */
#ifdef USE_WORK_DIV
/**
 *	@returns the index the given thread will stop at
 */
big_num ph_calculate_end_index(
    unsigned long thread_id, 
    unsigned long thread_count, 
    big_num size
);

/**
 * @returns the range of elements that should be given to the thread with 
 * the given id
 */
big_num ph_calculate_range(
    unsigned long thread_id, 
    unsigned long thread_count, 
    big_num size
);

/**
 * @returns the index the given thread will start at
 */
big_num ph_calculate_start_index(
    unsigned long thread_id, 
    unsigned long thread_count, 
    big_num size
);

#endif /* USE_WORK_DIV */

/**
 * Generates a random number from 0 to the given max
 * @param max - end range of random number
 * @returns random number from [0, max)
 */
int ph_generate_random_int(int max);

/**
 * Generates a random number from 0 to the given max 
 * @param max - end range of random number
 * @returns random number from [0, max)
 */
long ph_generate_random_long(long max);

/*  mutex condition stuff below =============================================*/

/**
 * Adds the given value to the given variable, performing mutex locks
 * accordingly.
 *
 * Assumes all variables are initalized
 *
 * pthread methods used:
 *  pthread_mutex_lock
 *  pthread_mutex_unlock
 *
 * IN:
 *  @param mutex        - pointer to pthread_mutex for this variable
 *  @param variable     - pointer to variable to add to
 *  @param value        - value to add the variable by
 *
 * OUT:
 *  @param variable     - variable += value
 */
void ph_add(pthread_mutex_t *mutex, int *variable, int value);

/**
 * Modular broadcast method that peforms mutex lock and cond broadcast
 * while setting the broadcast variable!
 * 
 * Assumes all variables are iniitalized
 * 
 * pthread methods used:
 * 	pthread_cond_broadcast
 * 	pthread_mutex_lock
 * 	pthread_mutex_unlock
 * 
 * IN
 * 	@param mutex		- pointer to pthread_mutex for this condition
 * 	@param cond			- pointer to pthread_cond for this condition
 * 	@param condition	- pointer to condition variable to change
 * OUT
 * 	@param condition	- condition variable is set to 1
 */
void ph_broadcast(
	pthread_mutex_t *mutex, 
	pthread_cond_t *cond, 
	int *condition);
	
/**
 * Modular broadcast wait method that performs mutex lock and cond
 * broadcast. This does NOT change the condition variable
 * 
 * Assumes all variables are initialized
 * Assumes a thread will broadcast this condition
 * 
 * pthread methods used:
 * 	pthread_cond_wait
 * 	pthread_mutex_lock
 * 	pthread_mutex_unlock
 * 
 * IN
 * 	@param mutex		- pointer to pthread_mutex for this condition
 * 	@param cond			- pointer to pthread_cond for this condition
 * 	@param condition	- pointer to condition variable to check
 */
void ph_broadcast_wait(
	pthread_mutex_t *mutex, 
	pthread_cond_t *cond, 
	int *condition);

/**
 * Gets the value of a variable that may potentially be a race condition by 
 * performing mutex locks before reading the condition. Returns the value of 
 * the variable
 *
 * Assumes all variables are initalized
 * 
 * pthread methods used:
 *  pthread_mutex_lock
 *  pthread_mutex_unlock
 *
 * IN:
 *  @param mutex        - pointer to pthread_mutex for this variable
 *  @param variable     - pointer to variable to get
 *
 * OUT:
 *  @returns the value of the variable
 */
int ph_get(pthread_mutex_t *mutex, int *variable);

/**
 * Sets the variable to the given value, performing mutex locks before doing so
 *
 * Assumes all variables are initialized
 *
 * pthread_methods used:
 *  pthread_mutex_lock
 *  pthread_mutex_unlock
 *
 * IN:
 *  @param mutex        - pointer to pthread_mutex for this variable
 *  @param variable     - pointer to the variable to set
 *  @param value        - value to set the the variable to
 *
 * OUT:
 *  @param variable     - value has been set to this variable
 */
void ph_set(pthread_mutex_t *mutex, int *variable, int value);

/**
 * Modular signal method that performs mutex lock and cond broadcast
 * while setting the condition variable.
 * 
 * Assumes all variables are initalized
 * 
 * pthread methods used:
 * 	pthread_cond_signal
 * 	pthread_mutex_lock
 * 	pthread_mutex_unlock
 * 
 * IN
 * 	@param mutex		- pointer to pthread_mutex for this condition
 * 	@param cond			- pointer to pthread_cond for this condition
 * 	@param condition	- pointer to condition variable to change
 * OUT
 * 	@param condition	- condition variable is set to 1
 */
void ph_signal(
	pthread_mutex_t *mutex, 
	pthread_cond_t *cond, 
	int *condition);

/**
 * Subtracts the variable by the given value, performing mutex locks 
 * accordingly.
 *
 * Assumes all variables are initalized
 *
 * pthread methods used:
 *  pthread_mutex_lock
 *  pthread_mutex_unlock
 *
 * IN:
 *  @param mutex        - pointer to pthread_mutex for this variable
 *  @param variable     - pointer to the variable to subtract
 *  @param value        - value to subtract by
 *
 * OUT:
 *  @param variable     - variable -= value
 */
void ph_sub(pthread_mutex_t *mutex, int *variable, int value);

/**
 * Modular wait method that performs mutex lock and cond broadcast
 * while checking the condition variable. This method WILL reset 
 * the condition variable after it becomes true.
 * 
 * Assumes all variables are initalized
 * Assumes a thread will signal this condition.
 * 
 * pthread methods used:
 * 	pthread_cond_wait
 * 	pthread_mutex_lock
 * 	pthread_mutex_unlock
 * 
 * IN
 * 	@param mutex		- pointer to pthread_mutex for this condition
 * 	@param cond			- pointer to pthread_cond for this condition
 * 	@param condition	- pointer to condition variable to check
 * OUT
 * 	@param condition	- condition variable is set to 0
 */
void ph_wait(
	pthread_mutex_t *mutex, 
	pthread_cond_t *cond, 
	int *condition);

/* these functions are for using Math library to do some calculations */
#ifdef USEMATH

/**
 * Calculates how many threads the given thread needs to spawn for 
 * computing a tree sum
 * 
 * @returns the number of threads the given thread needs to spawn
 */
unsigned int ph_calculate_thread_spawn_size(
    unsigned long divisor, 
    unsigned long thread_id, 
    unsigned long thread_count
);

/**
 * @returns the nearest power of two to the given value
 * that is greater than the given value
 */
int ph_find_larger_power_of_two(int value);

/**
 * @returns the log_2 of the value, rounded up to the nearest integer
 */
int ph_find_largest_expo_of_two(int value);

/**
 * @returns the nearest power of two to the given value that is
 * smaller than the given value
 */
int ph_find_smaller_power_of_two(int value);

/**
 * @returns the log_2 of the value, rounded down to the nearest integer
 */
int ph_find_smallest_expo_of_two(int value);

/**
 * @returns the log base of the given value and base
 */
double ph_log_base(double value, double base);

#endif /* USEMATH not defined */

#endif
