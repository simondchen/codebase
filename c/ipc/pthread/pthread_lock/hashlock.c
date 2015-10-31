#include <pthread.h>

#define NHASH 29
#define HASH(fp) (((unsigned long)fp)%NHASH)

struct foo *fn[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo{
	pthread_mutex_t f_lock;
	int f_count;
	struct foo *f_next;
	int f_id;
};

struct foo *foo_alloc(void){
	struct foo *fp;
	int fid = HASH(fp);
	if((fp=malloc(sizeof(foo)))!=NULL)
	{
		fp->f_count=1;
		if(pthread_mutex_init(&fp->f_lock,NULL)!=NULL){
			free(fp);
			return NULL;
		}
		pthread_mutex_lock(&hashlock);
		fp->next=fn[fid];
		fn[fid]=fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		//init the struct foo
		pthread_mutex_unlock(&fp->f_lock);
	}
	return fp;
}

void foo_hold(struct foo *fp){
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

struct foo *foo_find(int fid){
	struct foo *fp;
	int i;
	pthread_mutex_lock(&hashlock);
	for(i=0;i<NHASH;i++){
		for(fp=fn[i];fp!=NULL;fp=fp->next)
		{
			if(fp->f_id==fid)
			{
				pthread_mutex_lock(&fp->f_lock);
				foo_hold(fp);
				break;
			}
		}
	}
	pthread_mutex_unlock(&hashlock);
	return fp;
}

void foo_rele(struct foo *fp){
	struct foo *tfp;
	int fid = HASH(fp);
	pthread_mutex_lock(&fp->f_lock);
	if(fp->f_count==1){//last reference,delete fp from hashlist and free the structure,so wo must apply hashlock
		//we must obey the order of lock,we should not lock fp->f_lock and then lock hashlock,that is we mush release fp->f_lock before we lock hashlock 
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_lock(&hashlock);
		pthread_mutex_lock(&fp->f_lock);
		if(fp->f_count!=1){
			fp->f_count--;
			pthread_mutex_unlock(&hashlock);
			pthread_mutex_unlock(&fp->f_lock);
			return;
		}
		tfp=fn[fid];
		if(tfp==fp){
			fn[fid]=tfp->f_next;
		}else{
			while(tfp->f_next!=fp)
				tfp=tfp->f_next;
			tfp->f_next=fp->f_next;
		}
		pthread_mutex_unlock(&hashlock);	
		pthread_mutex_unlock(&fp->f_lock);
	}else{
	//no need to change the hash,so not need to apply hashlock
		fp->f_count--;
		pthread_mutex_unlock(&fp->f_lock);
	}
}
