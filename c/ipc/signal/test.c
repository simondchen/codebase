#include "../util/simon.h"

static void sig_handler(int signo){
	printf("sigint comes\n");
}

int main(){
	if(signal(SIGINT,sig_handler)==SIG_ERR)
	{
		err_sys("signal SIGINT error");
	}
	for(;;)
		pause();
	return 0;
}
