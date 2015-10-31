#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#define MAX_BUFFER 2048

int main(){
	char buffer[MAX_BUFFER];
	int sock,size,proto;
	char *ethhead,*iphead,*tcphead,*udphead,*icmphead,*p;
	if((sock=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP)))<0)
	{
		printf("create socket error\n");
		return -1;
	}
	while(1){
		size=recvfrom(sock,buffer,2048,0,NULL,NULL);
		printf("%d\n",size);
		if(size==18)
		{
			printf("==========================\n");
		}
		/*if(size<42){
			printf("incomplete header\n");
			continue;
		}
		ethhead=buffer;
		p=ethhead;
		int n=0xFF;
		printf("MAC:%02x:%02x:%02x:%02x:%02x:%02x=====>%02x:%02x:%02x:%02x:%02x:%02x\n",p[6]&n,p[7]&n,p[8]&n,p[9]&n,p[10]&n,p[11]&n,p[0]&n,p[1]&n,p[2]&n,p[3]&n,p[4]&n,p[5]&n);
		iphead=ethhead+14;
		p=iphead+12;
		printf("IP:%d.%d.%d.%d=>%d.%d.%d.%d\n",p[0]&0xFF,p[1]&0xFF,p[2]&0xFF,p[3]&0xFF,p[4]&0xFF,p[5]&0xFF,p[6]&0xFF,p[7]&0xFF);
		proto=(iphead+9)[0];
		p=iphead+20;
		printf("Protocol:");
		switch(proto){
			case IPPROTO_ICMP:
				printf("ICMP\n");
				break;
			case IPPROTO_IGMP:
				printf("IGMP\n");
				break;
			case IPPROTO_IPIP:
				printf("IPIP\n");
				break;
			case IPPROTO_TCP:
				printf("TCP\n");
				break;
			case IPPROTO_UDP:
				printf("UDP\n");
				break;
			default:
				break;
		}*/
	}
}
