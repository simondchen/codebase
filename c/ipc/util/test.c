#include "simon.h"
#include <mqueue.h>
int main(){
	mqd_t mqd;
	int flags;
	flags=O_RDONLY|O_CREAT;
	if((mqd=mq_open("/new",flags))==-1){
		err_sys("mq_open error");
	}
	return 0;
}
