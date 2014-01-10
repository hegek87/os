/**
 * threadpool.c
 *
 * This file will contain your implementation of a threadpool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "threadpool.h"

// _threadpool is the internal threadpool structure that is
// cast to type "threadpool" before it given out to callers
typedef struct _threadpool_st {
	int threads_avail;
	pthread_t **threads;
	pthread_mutex_t mutex;
	pthread_cond_t condvar;
	// you should fill in this structure with whatever you need
} _threadpool;

threadpool create_threadpool(int num_threads_in_pool) {
	_threadpool *pool;

	// sanity check the argument
	if ((num_threads_in_pool <= 0) || (num_threads_in_pool > MAXT_IN_POOL))
		return NULL;

	pool = (_threadpool *) malloc(sizeof(_threadpool));
	if (pool == NULL) {
		fprintf(stderr, "Out of memory creating a new threadpool!\n");
		return NULL;
	}

	// add your code here to initialize the newly created threadpool
 	pool->threads_avail = num_threads_in_pool;
 	pool->threads = malloc(num_threads_in_pool*sizeof(pthread_t *));
 	pthread_mutex_init(&(pool->mutex), NULL);
 	pthread_cond_init(&(pool->condvar), NULL);

	return (threadpool) pool;
}


void dispatch(threadpool from_me, dispatch_fn dispatch_to_here,
	      void *arg) {
	_threadpool *pool = (_threadpool *) from_me;

	// add your code here to dispatch a thread
	pthread_mutex_lock(&pool->mutex);
	if(!pool->threads_avail){
		//block if no available threads
		pthread_cond_wait(&(pool->condvar), &(pool->mutex));
	}
	--(pool->threads_avail);
	pthread_create(*(pool->threads+pool->threads_avail), NULL,
					dispatch_to_here, arg);
	return;
}

void destroy_threadpool(threadpool destroyme) {
  _threadpool *pool = (_threadpool *) destroyme;

  // add your code here to kill a threadpool
}

