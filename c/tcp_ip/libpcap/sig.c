#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

static sigjmp_buf jmpbuf;
int canjmp=0;

void m_send(){
	return;
}

void m_read(){
	for(;;){
	}
}

void sig_alarm(){
	if(canjmp==0)	
		return;
	else
		siglongjmp(jmpbuf,1);
		
}

int main(){
	int time=0,timeout=1;
	signal(SIGALRM,sig_alarm);
	if(sigsetjmp(jmpbuf,1)){
		if(time>3)
		{	
			printf("read error\n");
			exit(-1);
		}
		else{
			timeout*=2;	
		}
	}
	canjmp=1;
	m_send();
	time++;
	printf("send %d\n",time);
	alarm(timeout);
	m_read();
	canjmp=0;
	alarm(0);
}
