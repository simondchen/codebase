#include <pcap/pcap.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main(){
	char *device="wlan0";
	char errbuf[1024];
	char *net_addr;
	struct in_addr addr;
	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	if(pcap_lookupnet(device,&netp,&maskp,errbuf)==-1) //an integer value
	{
		fprintf(stderr,"%s",errbuf);
		return -1;
	}
	addr.s_addr=netp;
	net_addr=inet_ntoa(addr);
	printf("%s\n",net_addr);
	return 0;
}
