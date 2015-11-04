/*
 * 2015-11-01 10:31 
 *simon<nicksimon109445@gmail.com> 星期日
 *pf_packet to transmit and receive link layer packets
 *try your best,and enjoy coding
 *
 */

/*
 *响应者的处理逻辑:只是简单的接收包获取seq字段,记录rssi值并回复包
 *不处理包的丢失重传,让逻辑尽量简单
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include <netdb.h>
#include <net/if.h>
#include <errno.h>
#include <pcap/pcap.h>
#include <sys/types.h>
#include <stdint.h>
#include <signal.h>
#include <sys/select.h>

//这里pos都是从零开始!!!
#define DST_POS 30
#define SRC_POS 36
#define BSSID_POS 42
#define SEQ_POS 48
#define RADIOTAP_LEN 26
#define RSSI_POS 22

typedef unsigned char u_char;
pcap_t *handle;
char *iface = "mon0";
int packet_socket;
int recv_socket;
struct sockaddr_ll sa_ll;
struct bpf_program fp;
//uint16_t seq=1;

/*
 *buf[22]是RSSI字段
 *buf[30:35]是Receiver/Destination address字段
 *buf[36:41]是Transmitter/Source address字段 
 *buf[42:47]是BSSID字段
 * buf[48:49]是seq字段
 *
 */
//设置怪异的源和目的mac地址,以便于过滤
//dst mac 11:22:33:44:55:66
//src 66:55:44:33:22:11
//bssid 66:55:44:33:22:11
unsigned char buf[]={
    0x00, 0x00, 0x1a, 0x00, 0x2f, 0x48, 0x00, 0x00, 0x7e, 0x93, 0x85, 0x3b, 0x67, 0x06, 0x00, 0x00,
    0x02, 0x18, 0x6c, 0x09, 0xc0, 0x00, 0xb3, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
    0x00, 0x00, 0x7e, 0x79, 0x9d, 0xd9, 0x0b, 0x00, 0x00, 0x00, 0x19, 0x00, 0x11, 0x04, 0x00, 0x0b,
    0x7a, 0x68, 0x61, 0x6e, 0x67, 0x79, 0x75, 0x79, 0x61, 0x6e, 0x67, 0x01, 0x08, 0x82, 0x84, 0x8b,
    0x96, 0x0c, 0x12, 0x18, 0x24, 0x03, 0x01, 0x01, 0x05, 0x04, 0x02, 0x03, 0x00, 0x00, 0x2a, 0x01,
    0x04, 0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, 0x2d, 0x1a, 0xee, 0x19, 0x1f, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3d, 0x16, 0x01, 0x05, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x1a, 0x00, 0x50, 0xf2,
    0x01, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x02, 0x02, 0x00, 0x00, 0x50, 0xf2, 0x02, 0x00, 0x50, 0xf2,
    0x04, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x02, 0x30, 0x18, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x02,
    0x00, 0x00, 0x0f, 0xac, 0x02, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x00,
    0x00, 0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0x00, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27,
    0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32, 0x2f, 0x00, 0xdd, 0x1e, 0x00, 0x90, 0x4c,
    0x33, 0xee, 0x19, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x1a, 0x00, 0x90, 0x4c,
    0x34, 0x01, 0x05, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x06, 0x00, 0xe0, 0x4c, 0x02, 0x01, 0x60
};

void alarm_handler(int sig){
    pcap_breakloop(handle);
    printf("timeout,break pcap_loop\n");
}

int send_beacon(uint16_t seq){
    //修改seq字段,seq计算方法:低4位不使用,即seq左移4位得到
   uint16_t temp_seq = seq << 4;
   ssize_t len;
   buf[SEQ_POS] = (uint8_t)(temp_seq & 0xff);
   buf[SEQ_POS+1] = (uint8_t)((temp_seq >> 8) & 0xff);
   // *((uint16_t *)buf) = temp_seq; //应该是等价的
   len=sendto(packet_socket,buf,sizeof(buf),0,(const struct sockaddr *)&sa_ll,sizeof(sa_ll));  
    if(len<0){
        perror("send_beacon:sendto error\n");
        return -1;
    }
    return 0;    
}

int init_tranmit_socket(void)
{
    uint16_t proto = htons(ETH_P_ALL);
    int fd;
    fd=socket(AF_PACKET, SOCK_RAW, proto);
    printf("fd == %d\n",fd);
	if(fd<0){
		perror("socket error\n");
		return -1;
	}
    memset(&sa_ll, 0, sizeof(sa_ll));
    int iface_num = if_nametoindex(iface);
    sa_ll.sll_family = AF_PACKET;
    sa_ll.sll_protocol = proto;
    sa_ll.sll_ifindex = iface_num;
    //sa_ll.sll_pkttype = PACKET_OUTGOING;
    //sa_ll.sll_halen = ETHER_ADDR_LEN;
    return fd;
}

int init_libpcap(void)
{
    char errbuf[1024]={'\0',};
    handle=pcap_open_live(iface,BUFSIZ,1,0,errbuf);
    if(handle==NULL){
            printf("pcap_open_live error:%s\n",errbuf);
            return -1;
    }
    //编译和设置过滤规则
    //从文件中读取过滤规则
    FILE *rule=NULL; 
    if((rule=fopen("filter_rule.conf","r"))==NULL){
        perror("open config file error,please create the config file for filter\n");
        return -1;
    }
    char filter[100];
    if(fgets(filter,100,rule)==NULL){
        perror("fgets config file error\n");
        fclose(rule);
        return -1;
    }
    fclose(rule);
    //去掉换行符
    int len=strlen(filter); 
    filter[len-1]='\0';
    //printf("rule:%s\n",filter);
    if(pcap_compile(handle,&fp,filter,0,0)<0){
        pcap_perror(handle,"pcap_compile error:");
        return -1;
    }
    if(pcap_setfilter(handle,&fp)<0){
        pcap_perror(handle,"pcap_setfilter error:");
        return -1;
    }
    pcap_freecode(&fp);
    return 0;
}

void stop_capture(int sig)
{
    printf("send SIGINT signal to terminate packet capture\n");
    //free resources
    pcap_freecode(&fp);
    close(packet_socket);
    pcap_close(handle);
    exit(0);
}

/*
 * 设计两种包交互协议:
 * 1.不重传,每隔50ms发送包,若存在一方seq丢失则该seq的包不使用
 * 2.处理重传,稍微复杂一些
 * 3.两种协议都实现一下,且做成可配置的方式
 */
void packet_process(u_char *user,const struct pcap_pkthdr *h,const u_char *bytes)
{
    int8_t rssi=bytes[22];
    uint16_t seq=*((uint16_t *)(bytes+SEQ_POS));
    seq >>= 4;
    send_beacon(seq);
    printf("seq:%d,rssi:%d\n",seq,rssi);
    //重置定时器
    alarm(5);
}

int main(void)
{
    /*1.初始化AF_PACKET套接字packet_socket用于发送链路层数据包*/
    if((packet_socket=init_tranmit_socket())<0)
        return -1;
    //设置Ctrl+C的处理函数
    signal(SIGINT,stop_capture); 
    /*2.初始化libpcap用于接收和过滤数据包,因为libpcap可以在内核中进行过滤,提高效率,抽时间研究一下是怎么实现的*/
    if(init_libpcap()<0){
        goto fail;
    }
    
    signal(SIGALRM,alarm_handler);


    //将pcap_loop放于循环中
    while(1){
        int num=0;
        alarm(5);
        num=pcap_loop(handle,-1,packet_process,NULL);
        printf("loop ret:%d\n",num);
    }

    return 0;
fail:
    pcap_freecode(&fp);
    close(packet_socket);
    pcap_close(handle);
    return -1;
}
