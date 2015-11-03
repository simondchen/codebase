/*
 * 2015-11-01 10:31 
 *simon<nicksimon109445@gmail.com> 星期日
 *pf_packet to transmit and receive link layer packets
 *try your best,and enjoy coding
 *
 */
//#define _BSD_SOURCE
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
uint16_t seq=1;

/*
 *buf[22]是RSSI字段
 *buf[30:35]是Receiver/Destination address字段
 *buf[36:41]是Transmitter/Source address字段 
 *buf[42:47]是BSSID字段
 * buf[48:49]是seq字段
 *
 */
//设置怪异的源和目的mac地址,以便于过滤
//!!!这里dst地址不可以随便填,因为如果不存在的话,链路层会不停地询问,导致收到很多包,最好设成广播地址
//!!!如果收不到包,一定要想到是不是mon0是否在侦听同一信道
//dst mac ff:ff:ff:ff:ff:ff
//src mac 11:22:33:44:55:66
//bssid mac 11:22:33:44:55:66
unsigned char buf[]={
    0x00, 0x00, 0x1a, 0x00, 0x2f, 0x48, 0x00, 0x00, 0x7e, 0x93, 0x85, 0x3b, 0x67, 0x06, 0x00, 0x00,
    0x02, 0x18, 0x6c, 0x09, 0xc0, 0x00, 0xb3, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x00, 0x0a, 0x7e, 0x79, 0x9d, 0xd9, 0x0b, 0x00, 0x00, 0x00, 0x19, 0x00, 0x11, 0x04, 0x00, 0x0b,
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

int send_beacon(void){
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
    /*
     *获取socket描述符
     */
    recv_socket=pcap_fileno(handle);
    /*
     * 设置为nonblock模式,注意nonblock模式下只能使用pcap_dispatch,如果没有数据，
     * pcap_dispatch立即返回0
     * 注意handle->fd就是socket描述符,也可以使用fcntl(handle->fd,F_SETFL,flags)
     * 设置非阻塞模式
     */
    if(pcap_setnonblock(handle,1,errbuf)<0){
        pcap_perror(handle,"pcap_setnonblock error:");
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
    //如果包处理过程占用过多时间,会影响报的接收,但如果是关心的包按照交互协议,则不必担心???处理时间长,缓冲区溢出!!!这里还不是很清楚,要留意一下
    //printf("packets coming:%d\n",h->caplen);
    //解析包
    //不要使用int,否则无法表示有符号,转化成有符号数
    int8_t rssi=bytes[22];
    uint16_t temp_seq=*((uint16_t *)(bytes+SEQ_POS));
    temp_seq >>= 4;
    printf("temp_seq:%d,rssi:%d\n",temp_seq,rssi);
    //休眠50ms,20packets/s
    usleep(50000);
    //发送下一个数据包
    seq++;
    send_beacon();
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
    //test
    int i=1;
    while(1){
        printf("send beacon:%d\n",i++);
        send_beacon();
        seq++;
        usleep(1000000);
    }
    /*3.作为发起者,先发送第一个包开始交互流程*/
    if(send_beacon()<0){
        goto fail;
    }
    /*4.使用pcap_dispatch非阻塞模式下抓包,结合select的方式应该更好,main loop*/
    struct timeval tv;
    fd_set fdset;
    int ret;
    while(1){ 
        //设置超时时间为1s,select使用的是jiffies来实现定时,所以其精度(resolution)受限于jiffies
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        FD_ZERO(&fdset);
        FD_SET(recv_socket,&fdset);
        ret=select(recv_socket+1,&fdset,NULL,NULL,&tv);
        if(ret<0){
            perror("select error\n");
            //这里先这么处理，如果真的出现这种情况再修改
            goto fail;
        }else if(ret==0){
            /*
             * 超时,不重传,seq加1,发送下一个包
             * 测试发现丢包的发生的概率还是不低的:initiator --> responder  responder --> initiator的过程中
             * 都有可能丢包,这里将超时时间设置为1s是不是太长了
             */
            printf("timeout and receive no response,go on transmitting the next packet without retransmittion\n");
            seq++;               
            if(send_beacon()<0){
                //发送包失败,一般不会发生把,处理方法先是终止程序,如果出现了该错误在处理
                goto fail;
            }
        }else{
            /*
             * 有数据,这里调用pcap_dispatch,注意pcap_loop和pcap_next均不可
             * 用于阻塞模式,但是对pcap_dispatch还不是很熟悉,需要测试,帮助
             * 文档中说的the end of the current bufferful of packet is reached
             * 是什么意思
             */
            //这里可能会出问题
            printf("call pcap_dispatch\n");
            pcap_dispatch(handle,1,packet_process,NULL);
        }
    }
    return 0;
fail:
    pcap_freecode(&fp);
    close(packet_socket);
    pcap_close(handle);
    return -1;
}
