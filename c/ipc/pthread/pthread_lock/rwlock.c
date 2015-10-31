#include <pthread.h>

struct job{
	//two way queue
	struct job *prev;
	struct job *next;
	pthread_t j_id;
	//other members may be
};

struct queue{
	struct job *head;
	struct job *tail;
	pthread_rwlock_t q_lock;
};

void job_insert(struct queue *qp,struct job *jp){
	pthread_rwlock_wrlock(&qp->q_lock);
	
}
