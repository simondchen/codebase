#include <stdio.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <string.h>
#include <errno.h>
/*
 *2015年 10月 27日 星期二 16:14:26 CST
 *<nicksimon109445@gmail.com>
 *try to send beacon frame using pf_packet
 */
int main(int argc,char *argv[]){
	struct sockaddr_ll laddr;
	struct ifreq iface; 
	unsigned char hwaddr[6];
	int s;
	//1.利用ioctl获取网卡的hw地址和index
	bzero(&iface,sizeof(iface));
	bzero(&laddr,sizeof(laddr));
	strcpy(iface.ifr_name,"eth0");
	if((s=socket(AF_INET,SOCK_DGRAM,0))<0){
		perror("socket error\n");
		return -1;
	}
	if(ioctl(s,SIOCGIFHWADDR,&iface)){
		perror("ioctl error\n");
		close(s);
		return -1;
	}
	memcpy(hwaddr,iface.ifr_hwaddr.sa_data,6);
	//printf("hwaddr: %2x : %2x : %2x : %2x : %2x : %2x\n", hwaddr[0], hwaddr[1],hwaddr[2], hwaddr[3],hwaddr[4], hwaddr[5]);
	if(ioctl(s,SIOCGIFINDEX,&iface)){
		perror("ioctl error\n");
		close(s);
		return -1;
	}
	//printf("%d\n",iface.ifr_ifindex);
	//2.创建packet套接字
	close(s);
	if((s=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL)))<0){
		perror("packet socket error\n");
		return -1;
	}
	//3.设置sockaddr_ll,并bind
	laddr.sll_family = AF_PACKET;
	laddr.sll_ifindex = iface.ifr_ifindex;
	if(bind(s,(struct sockaddr *)&laddr,sizeof(laddr))<0){
		perror("bind error\n");
		close(s);
		return -1;	
	}
	return 0;
}
