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
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>

/*
 *TODO
 *关闭接收端beacon_responser的socket的buffer
 *关于buffer的思考:首先要确信的是write和read是non-buffer的,所以这两个系统调用只要socket的buffer中有数据就会
 *去取.而socket也是存在buffer的.所以如果使用read和write,一般情况下缓存对时间的影响不是很严重
 *使用libpcap时,libpcap也是存在缓存的,pcap_loop去读数据,使用了BPF时,对于BSD系(包括OS X),可以使用ioctl(BIOCIMMEDIATE)
 *来关闭bfp的buffer
 *设置buffer大小:
 *setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (int[]){YOUR_BUF_LIMIT}, sizeof(int));
 *setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (int[]){YOUR_BUF_LIMIT}, sizeof(int));
 *
 */

//这里pos都是从零开始!!!
#define DST_POS 30
#define SRC_POS 36
#define BSSID_POS 42
#define SEQ_POS 48
#define RADIOTAP_LEN 26
/*
 * 这里不同的网卡rss的位置可能不同,所以还是要通过present_flags
 * 计算出rss的位置,当然现在为了方便可以先写固定值
 */
#define RSSI_POS 22

typedef unsigned char u_char;
typedef struct timeval tarray[1024];
pcap_t *handle;
char *iface = "mon0";
int packet_socket;
int recv_socket;
struct sockaddr_ll sa_ll;
struct bpf_program fp;
uint16_t seq=0;
FILE *file;
//两个全局数组,记录发送和接收的时间戳
tarray s_tarray;
tarray r_tarray;
struct timeval sdtime;
struct timeval rtime;
int warning=0;
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

int send_beacon(int tseq){
    //修改seq字段,seq计算方法:低4位不使用,即seq左移4位得到
   uint16_t temp_seq = tseq << 4;
   ssize_t len;
   buf[SEQ_POS] = (uint8_t)(temp_seq & 0xff);
   buf[SEQ_POS+1] = (uint8_t)((temp_seq >> 8) & 0xff);
   // *((uint16_t *)buf) = temp_seq; //应该是等价的
   //记录发送时间
   if(gettimeofday(&sdtime,NULL)<0){
       perror("gettimeofday send time error\n");
   }
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
    printf("recv_socket:%d\n",recv_socket);
    /*
     * 设置为nonblock模式,注意nonblock模式下只能使用pcap_dispatch,如果没有数据，
     * pcap_dispatch立即返回0
     * 注意handle->fd就是socket描述符,也可以使用fcntl(handle->fd,F_SETFL,flags)
     * 设置非阻塞模式
     */
    //这里没啥用
    //if(pcap_setnonblock(handle,1,errbuf)<0){
    //    pcap_perror(handle,"pcap_setnonblock error:");
    //    return -1;
    //}
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
    //使用BPF时,要enable "immediate mode"防止系统buffer for us
    //好像只有BSD系列的支持
    //on Linux, this is currently not necessary - what buffering is done
    //doesn't have a timeout for the delivery of packets
    //unsigned int on=1;
    //ioctl(recv_socket,BIOCIMMEDIATE,&on);
    return 0;
}

void stop_capture(int sig)
{
    printf("send SIGINT signal to terminate packet capture\n");
    printf("warning:%d\n",warning);
    //free resources
    pcap_freecode(&fp);
    close(packet_socket);
    pcap_close(handle);
    fclose(file);
    exit(0);
}

void break_loop(int sig){
    //timeout,关闭pcap_breakloop
    pcap_breakloop(handle);
    printf("timeout,break paploop\n");
}

//a-b,假设a比b靠后,c中结构体的赋值是语言支持的,而数组的直接赋值不支持
//数组不支持直接赋值是因为数组是一个地址,赋值时长度不容易控制
struct timeval timesub(struct timeval a,struct timeval b){
    struct timeval ret;
    if(a.tv_usec<b.tv_usec){
        ret.tv_usec=a.tv_usec+1000000-b.tv_usec;
        ret.tv_sec=a.tv_sec-1-b.tv_sec;
    }else{
        ret.tv_sec=a.tv_sec-b.tv_sec;
        ret.tv_usec=a.tv_usec-b.tv_usec;
    }
    return ret;
}

/*
 * 设计两种包交互协议:
 * 1.不重传,每隔50ms发送包,若存在一方seq丢失则该seq的包不使用
 * 2.处理重传,稍微复杂一些
 * 3.两种协议都实现一下,且做成可配置的方式
 */
void packet_process(u_char *user,const struct pcap_pkthdr *h,const u_char *bytes)
{
    /*
     *TODO 
     *
     *
     */
    //获取当前时间rtime,并与sdtime比较,以记录从发包到收包所消耗的时间
    if(gettimeofday(&rtime,NULL)<0){
        perror("gettimeofday error:rtime\n");
    }
    //记录rss和seq
    int8_t rssi=bytes[22];
    uint16_t temp_seq=*((uint16_t *)(bytes+SEQ_POS));
    temp_seq >>= 4;
    printf("seq:%d,rssi:%d\n",temp_seq,rssi);
    //比较temp_seq和seq,若temp_seq小于seq,说明之前的包,超时时间设的有点短,warning
    if(temp_seq<seq){
        printf("WARNING:original packet considered loss appear:%d,%d\n",temp_seq,seq);
        warning++;
        return;
    }
    //显示时间
    struct timeval tmp=timesub(rtime,sdtime);
    printf("%ld:%06ld\n",tmp.tv_sec,tmp.tv_usec);
    //记录到文件中
    int len=fprintf(file,"%d:%d\n",temp_seq,rssi);
    if(len<0){
        perror("fprintf error\n");
    }
    //休眠50ms,20packets/s
    //发包的时间间隔也有要考虑,多改改试试
    usleep(50000);
    //发送下一个数据包
    seq++;
    send_beacon(seq);
    //重置定时器
    struct itimerval new; 
    memset(&new,0,sizeof(new));
    new.it_value.tv_sec=0;
    //休眠0.1s,即100ms,即100000us
    new.it_value.tv_usec=100000;
    if(setitimer(ITIMER_REAL,&new,NULL)<0){
        perror("setitimer error:");
        exit(-1);
    }
}

int main(void)
{
    //打开样本记录文件,以时间为文件命名
    time_t t;
    time(&t);
    struct tm *tm=localtime(&t);
    char time_buf[20];
    char path[40]="sample/seq_";
    sprintf(time_buf,"%d-%d %d:%d",tm->tm_mon,tm->tm_mday,tm->tm_hour,tm->tm_min);
    strcat(path,time_buf);
    printf("%s\n",path);
    file=fopen(path,"w+");
    if(file==NULL){
        perror("open file error\n");
        return -1;
    }
    //初始化sdtime,rtime
    memset(&sdtime,0,sizeof(sdtime));
    memset(&rtime,0,sizeof(rtime));
    /*1.初始化AF_PACKET套接字packet_socket用于发送链路层数据包*/
    if((packet_socket=init_tranmit_socket())<0)
        return -1;
    //设置信号处理函数
    signal(SIGINT,stop_capture); 
    signal(SIGALRM,break_loop);
    /*2.初始化libpcap用于接收和过滤数据包,因为libpcap可以在内核中进行过滤,提高效率,抽时间研究一下是怎么实现的*/
    if(init_libpcap()<0){
        goto fail;
    }
    //test
    /*int i=1;
    while(1){
        printf("send beacon:%d\n",i++);
        send_beacon();
        seq++;
        usleep(1000000);
    }*/
    /*3.使用pcap_loop结合信号防止丢包后的一直阻塞*/
    while(1){ 
        seq++;        
        send_beacon(seq);
        //设置定时器,防止丢包发生后pcap_loop一直阻塞
        //要注意的是要去分出确实是丢包发生了!!!所以要做两件事
        //a.timeout时间大于2*ttl时间,经实验2*ttl时间大约在20ms这一级别(之内)
        //b.针对特殊情况,比如一个包经过了特殊长的时间才到达,超过20ms很多,比如100ms,
        //这种情况出现的较少,但也要做处理:通过seq控制,比较temp_seq和seq,若出现
        //temp_seq<seq的情况,警告
        //ms级别的定时,使用setitimer
        struct itimerval new; 
        memset(&new,0,sizeof(new));
        new.it_value.tv_sec=0;
        //休眠0.1s,即100ms,即100000us
        new.it_value.tv_usec=100000;
        if(setitimer(ITIMER_REAL,&new,NULL)<0){
            perror("setitimer error:");
            return -1;
        }
        //-1就是不设置抓包数,其退出不收抓包数影响
        pcap_loop(handle,-1,packet_process,NULL);
    }
    return 0;
fail:
    pcap_freecode(&fp);
    close(packet_socket);
    pcap_close(handle);
    return -1;
}
