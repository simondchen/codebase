#include <pcap.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
int main(){
	pcap_if_t *alldevsp;
	struct pcap_addr *paddr;
	struct sockaddr_in *addr;
	char *errbuf;
	if(pcap_findalldevs(&alldevsp,errbuf)<0)
	{
		printf("find dev error:%s\n",errbuf);
		return -1;
	}
	while(alldevsp!=NULL){
		//printf("if:%s\n",alldevsp->name);
		//printf("des:%s\n",alldevsp->description);
		//alldevsp=alldevsp->next;
		if(strcmp(alldevsp->name,"wlan0")==0)
		{
			paddr=alldevsp->addresses;	
			addr=(struct sockaddr_in *)paddr->addr;
			printf("%u\n",(u_long)addr->sin_addr->s_addr);
			break;
		}
	}
	return 0;
}
