#include <stdio.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string.h>

int main(){
	struct ifreq ifr;
	int fd;
	strcpy(ifr.ifr_name,"eth0");
	if((fd = socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("create socket error\n");
		return -1;
	}
	if(ioctl(fd,SIOCGIFHWADDR,&ifr)<0)
	{
		printf("get macaddr error\n");
		return -1;
	}
	printf("mac:%02x:%02x:%02x:%02x:%02x:%02x\n",(unsigned char)ifr.ifr_hwaddr.sa_data[0],(unsigned char)ifr.ifr_hwaddr.sa_data[1],(unsigned char)ifr.ifr_hwaddr.sa_data[2],(unsigned char)ifr.ifr_hwaddr.sa_data[3],(unsigned char)ifr.ifr_hwaddr.sa_data[4],(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	return 0;
}
