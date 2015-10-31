#include <pcap/pcap.h>
#include <stdio.h>
#include <string.h>
int main(){
	int *dlt_buf;
	int num;
	int i;
	//char *device="mon0";
	//char *device="wlan0";
	char *device="eth0";
	char errbuf[1024];
	pcap_t *handle;
	handle=pcap_open_live(device,BUFSIZ,1,0,errbuf);
	if(handle==NULL)
	{
		printf("open device error:%s\n",errbuf);
		return -1;
	}
	if(strlen(errbuf)>0)
	{
		printf("err:%s\n",errbuf);
		return -1;
	}
	if(pcap_datalink(handle)==DLT_IEEE802_11_RADIO)
	{
		printf("BSD wireless with Radiotap header\n");
	}
	num=pcap_list_datalinks(handle,&dlt_buf);
	for(i=0;i<num;i++)
	{
		printf("%d - %s - %s\n",dlt_buf[i],pcap_datalink_val_to_name(dlt_buf[i]),pcap_datalink_val_to_description(dlt_buf[i]));
	}
	return 0;
}

