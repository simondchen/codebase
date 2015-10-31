#include <mqueue.h>
#include <stdio.h>
int main(int argc,char **argv){
	struct mq_attr mqattr;
	int error;
	mqd_t mqd;
	if(argc!=2)
	{
		printf("wrong parm\n");
		return -1;
	}
	if((mqd=mq_open(argv[1],O_RDONLY))==-1)
	{
		printf("mq_open error\n");
		return -1;
	}
	mq_getattr(mqd,&mqattr);
	printf("max msgs:%ld, max msgsize:%ld, current msgs:%ld\n",mqattr.mq_maxmsg,mqattr.mq_msgsize,mqattr.mq_curmsgs);
	mq_close(mqd);
	return 0;
}
