#ifndef __MSGHDR_H
#define __MSGHDR_H
#include <sys/types.h>
struct mymsghdr{
	pid_t pid;	
	int len;
	char data[0];
};
#endif /*__MSGHDR_H*/
