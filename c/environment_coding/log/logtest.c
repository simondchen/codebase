#include <syslog.h>
int main(int argc,char *argv[]){
	syslog(LOG_DEBUG,"this is a log generated from program %s\n",argv[0]);
	closelog();
	return 0;
}
