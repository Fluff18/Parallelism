#include<stdio.h> 
#include<pthread.h> 
#include<unistd.h> 
#include<stdlib.h>
int main(){
	pthread_t tid;
	struct sched_param param;
	int priority;
	int policy;
	int ret;

	/* scheduling parameters of target thread */
	ret = pthread_getschedparam (tid, &policy, &param);

	/* sched_priority contains the priority of the thread */
	priority = param.sched_priority; 
	printf("Priority : %d",priority);
}