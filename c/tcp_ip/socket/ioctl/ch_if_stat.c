#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void){
	struct ifreq ifr;
	const char *name = "wlan0";
	int sockfd;
	int ret;
	struct sockaddr_in *pa_addr;
	char *addr;
	unsigned char hwaddr[6];
	//struct in_addr addr;
	//atmost
	printf("%d\n",IFNAMSIZ);
	strncpy(ifr.ifr_name,name,IFNAMSIZ);

	//1.创建socket
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0))<0){
		printf("socket error\n");
		return -1;
	}
	
	//2.调用ioctl获取ip地址
	ret = ioctl(sockfd,SIOCGIFADDR,&ifr);
	if(ret<0){
		printf("ioctl error\n");
		return -1;
	}
	pa_addr = (struct sockaddr_in *)&(ifr.ifr_addr);
	printf("my addr is %s\n",(char *)inet_ntoa(pa_addr->sin_addr));

	//3.调用ioctl获取hw地址
	ret = ioctl(sockfd,SIOCGIFHWADDR,&ifr);
	if(ret<0){
		printf("ioctl error\n");
		return -1;
	}
	memcpy(hwaddr,((struct sockaddr *)&(ifr.ifr_hwaddr))->sa_data,6);
	printf("hw address is %x:%02x:%02x:%02x:%02x:%02x\n",hwaddr[0],hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);
	return 0;
}
