#include <stdio.h>
#include <pcap/pcap.h>
int main(){
	char *device;
	char errbuf[1024];
	if((device = pcap_lookupdev(errbuf))==NULL){
		printf("lookupdev error:%s\n",errbuf);	
		return -1;
	}
	printf("if:%s\n",device);
	return 0;
}
