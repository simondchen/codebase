#include <pcap/pcap.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>

//radiotap mask
#define TSFT(present) present&0x00000001?8:0
#define FLAG(present) present&0x00000002?1:0
#define RATE(present) present&0x00000004?1:0
#define CHANNEL(present) present&0x00000008?4:0 //2*2 frequency,type
#define FHSS(present) present&0x00000010?2:0
#define RSSI(present) present&0x00000020?1:0

pcap_t *handle;
int count;
FILE *file;
int s_sum=0;
int r_num=0,r_sum=0;
struct timeval ts;
int rssi;

int radiotap_parser(u_int32_t present){
	int pos=0;//initialize it!!!but I don't know why
	pos+=TSFT(present);
	pos+=FLAG(present);
	pos+=RATE(present);
	pos+=CHANNEL(present);
	pos+=FHSS(present);
	return pos;
}

struct radiotap{
	u_int8_t it_version;
	u_int8_t it_pad;
	u_int16_t it_len;
	u_int32_t it_present;
}__attribute__((__packed__));

void ctrl_c(){
	printf("Exiting\n");
	fprintf(file,"r_sum:%d,s_sum:%d\n",r_sum,s_sum);
	pcap_breakloop(handle);
	pcap_close(handle);
	exit(0);
}

void packet_process(u_char *user,const struct pcap_pkthdr *header,const u_char *packet){
	int pos;
	char *pkt;
	//printf("packet length:%d\n",header->caplen);
	struct radiotap *head;
	//struct timeval ts;
	head=(struct radiotap *)packet;
	//printf("version:%d\n",head->it_version);
	//printf("pad:%d\n",head->it_pad);
	//printf("header_length:%d\n",head->it_len); //pay attention to byte order,not need to transform
	//printf("header_length:%d\n",ntohs(head->it_len)); //pay attention to byte order
	//printf("present_flag:%d\n",ntohl(head->it_present));
	//printf("present_flag:0x%08x\n",head->it_present);
	pos=radiotap_parser(head->it_present);
	pkt=(char *)(head+1);
	//printf("point_postion:%d\n",pos);
	pkt+=pos;
	//printf("postion:0x%02lx",pkt-packet);
	//145---ping request,158---ping reply
	//receive request,the caplen may corresponding to network driver because the different radiotap header
	if(header->caplen == 109)
	{
		r_sum++;
		//printf("RSSI:%ddm\n",*pkt);
		//pkt=(char *)(packet+26);	
		//printf("type:%d\n",((*pkt)>>2));
		//printf("len:%d\n",header->caplen);
		ts=header->ts;	
		rssi=*pkt;
		printf("timestamp:%ld.%06ld\n",(long int)ts.tv_sec,(long int)ts.tv_usec);
		//write to file seq
	}
	//send ping request
	if(header->caplen == 94)
	{
		s_sum++;
		//s_num++;
		fprintf(file,"%d:%ld.%06ld:%ddm\n",r_num++,(long int)ts.tv_sec,(long int)ts.tv_usec,rssi);
	}
}

int main(int argc,char **argv){
	char errbuf[PCAP_ERRBUF_SIZE]; //256
	char *device;
	int c,verbose=0;
	int num;
	int ret;
	bpf_u_int32 netp,maskp;
	struct in_addr netaddr;
	struct bpf_program fp;
	//char *filter="";
	//char *filter="wlan src c0:cb:38:87:f9:bb and wlan dst 00:1e:65:d1:16:fa";
	char *filter="wlan host 00:1e:65:d1:16:fa";
	file=fopen("./seq","w");
	if(file==NULL)
	{
		printf("open file error\n");
		return -1;
	}
	fprintf(file,"%s\n","#seqnum:timestamps:rssi");
	while((c=getopt(argc,argv,"i:v"))!=-1){
		switch(c){
			case 'i':
				device=optarg;
				break;
			case 'v':
				verbose=1;
				break;
		}
	}
	signal(SIGINT,ctrl_c);
	if(device==NULL){
		if((device=pcap_lookupdev(errbuf))==NULL)
		{
			fprintf(stderr,"lookupdev:%s\n",errbuf);
			return -1;
		}
	}
	handle=pcap_open_live(device,BUFSIZ,1,0,errbuf);
	if(handle==NULL)
	{
		fprintf(stderr,"open_live:%s\n",errbuf);
		return -1;
	}
	if(verbose){
		printf("using device:%s\n",device);
		num=pcap_datalink(handle);
		printf("%s\n",pcap_datalink_val_to_name(num));
		printf("%s\n",pcap_datalink_val_to_description(num));
	}
	//used when broadcast
	/*if(pcap_lookupnet(device,&netp,&maskp,errbuf)==-1)
	{
		fprintf(stderr,"lookupnet:%s\n",errbuf);
		return -1;
	}
	netaddr.s_addr=netp;
	printf("addr:%s\n",inet_ntoa(netaddr));*/
	if(pcap_compile(handle,&fp,filter,0,0)==-1)
	{
		printf("pcap_compile:%s\n",pcap_geterr(handle));
		return -1;
	}
	if(pcap_setfilter(handle,&fp)==-1)
	{
		printf("pcap_setfilter:%s\n",pcap_geterr(handle));
		return -1;
	}
	pcap_freecode(&fp);	
	
	if((ret=pcap_loop(handle,-1,packet_process,NULL))<0)
	{
		if(ret==-1)
		{
			printf("pcap_loop:%s\n",pcap_geterr(handle));
			return -1;
		}
	}
	//printf("r_sum:%d,s_sum:%d\n",r_sum,s_sum);
	pcap_close(handle);
	return 0;
}
