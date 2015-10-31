#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <string.h>
#include "md5/md5.h"
/*unsigned char *getMd5(unsigned char id,unsigned char *value){
	//init
	unsigned char data[64]="";
	unsigned char *hashvalue=NULL;
	int len=0;
	//char *passwd="109445";
	printf("%02x\n",value[15]);
	data[0]=id;
	len=1;
	//memcpy(data+len,passwd,6);
	data[1]=0x01;
	data[2]=0x00;
	data[3]=0x09;
	data[4]=0x04;
	data[5]=0x04;
	data[6]=0x05;
	len+=6;
	memcpy(data+len,value,16);
	len+=16;
	hashvalue=ComputeHash(data,len);
	return hashvalue;
}*/

int main(){
	int sock,size;
	struct sockaddr_ll addr;
	unsigned char buffer[1024];
	unsigned char hwaddr[6]={0xf0,0x4d,0xa2,0xc4,0xda,0x09}; 
	unsigned char dstaddr[6]={0x01,0x80,0xc2,0x00,0x00,0x03};
	unsigned char start_pkt[18];
	unsigned char id_pkt[31];
	sock=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_PAE));
	if(sock<0)
		printf("sock error\n");
	bzero(&addr,sizeof(struct sockaddr_ll));
	addr.sll_family=PF_PACKET;
	addr.sll_protocol=htons(ETH_P_PAE);
	addr.sll_ifindex=2;
	if(bind(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr_ll))<0)
		printf("bind error\n");
	memcpy(start_pkt,dstaddr,6);
	memcpy(start_pkt+6,hwaddr,6);
	start_pkt[12]=0x88;
	start_pkt[13]=0x8e;
	start_pkt[14]=0x01;
	start_pkt[15]=0x01;
	memset(start_pkt+16,0,2);
	if(sendto(sock,start_pkt,sizeof(start_pkt),0,(struct sockaddr *)&addr,sizeof(struct sockaddr_ll))<0)
		printf("send error\n");
	size=recvfrom(sock,buffer,1024,0,NULL,NULL);
	printf("%d\n",size);
	int id=buffer[19];
	int type=buffer[22];
	printf("type:%d\n",type);
	memcpy(id_pkt,dstaddr,6);
	memcpy(id_pkt+6,hwaddr,6);
	id_pkt[12]=0x88;
	id_pkt[13]=0x8e;
	id_pkt[14]=0x01;
	id_pkt[15]=0x00;
	id_pkt[16]=0x00;
	id_pkt[17]=0x0d;
	id_pkt[18]=0x02;
	id_pkt[19]=id;
	id_pkt[20]=0x00;
	id_pkt[21]=0x0d;
	id_pkt[22]=0x01;
	id_pkt[23]=0x30;
	id_pkt[24]=0x30;
	id_pkt[25]=0x31;
	id_pkt[26]=0x30;
	id_pkt[27]=0x31;
	id_pkt[28]=0x33;
	id_pkt[29]=0x32;
	id_pkt[30]=0x30;
	//char *name="00101320";
	//memcpy(start_pkt+23,name,strlen(name));
	//printf("%2x\n",start_pkt[29]);
	if(sendto(sock,id_pkt,sizeof(id_pkt),0,(struct sockaddr *)&addr,sizeof(struct sockaddr_ll))<0)
		printf("send id error\n");
	size=recvfrom(sock,buffer,1024,0,NULL,NULL);
	printf("challenge-size:%d\n",size);
	unsigned char id2=buffer[19];
	type=buffer[22];
	printf("challenge-type:%d\n",type);
	if(type!=4)
		return -1;
	//compute challenge ret
	unsigned char hash[16];
	unsigned char value[23];
	value[0]=id2;
	value[1]=0x31;
	value[2]=0x30;
	value[3]=0x39;
	value[4]=0x34;
	value[5]=0x34;
	value[6]=0x35;
	memcpy(value+7,buffer+24,16);
	int i;
	for(i=0;i<23;i++){
		printf("%02x",value[i]);
	}
	printf("\n");
	/*unsigned char challenge[40];
	memcpy(chal,dstaddr,6);
	memcpy(chal,hwaddr,6);
	chal[12]=0x88;
	chal[13]=0x8e;
	chal[14]=0x01;
	chal[15]=0x00;
	chal[16]=0x00;
	chal[17]=0x16;
	chal[18]=0x02;
	chal[19]=0x07;
	chal[20]=0x00;
	chal[21]=0x16;
	chal[22]=0x04;
	chal[23]=0x10;*/

	return 0;
}










