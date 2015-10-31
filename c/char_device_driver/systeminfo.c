#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string.h>
#include "memdev.h"


struct option long_options[] =
{
	{"processtree",0,NULL,'p'},
	{"threadgroup",0,NULL,'t'},
	{"memstat",1,NULL,'m'},
	{"processdetail",1,NULL,'d'},
	{"help",0,NULL,'h'}
};


char *short_options = "ptm:d:h";  


void help()
{
	printf("\n\
			Usage:systeminfo [options]\n\
			Options:\n\
			-p,--processtree    	xxxxx\n\
			-t,--threadgroup    	xxxxx\n\
			-m,--memstat pid    	xxxxx\n\
			-d,--processdetail pid 	xxxxx\n\
			-h,--help               xxxxx\n");
}

char buffer[81920];

int main(int argc,char **argv)
{
	int c;
	int pid = 0;
	if(argc == 1)
	{
		help();
		exit(0);
	}
	while((c = getopt_long(argc,argv,short_options,long_options,NULL)) != -1)
	{
		switch(c)
		{
			case 'p':
				printf("processtree infomation\n");
				{
					int fd = open("/dev/memdev", O_RDONLY,0);
					if (fd < 0) 
					{
						perror("open");
        				return -1;
    				}
    				ioctl(fd,CHARDEV_PSTREE,buffer);
    				printf("%s\n", buffer);
				}
				break;
			case 't':
				printf("threadgroup infomation\n");
				{
					int fd = open("/dev/memdev", O_RDONLY,0);
					if (fd < 0) 
					{
						perror("open");
        				return -1;
    				}
    				ioctl(fd,CHARDEV_THREADGRP,buffer);
    				printf("%s\n", buffer);
  
				}				
				break;
			case 'm':
				pid = atoi(optarg);
				printf("memstat %d\n",pid);
				{
					struct type2_data  *pdata = (struct type2_data*)malloc(sizeof(struct type2_data));
					int fd = open("/dev/memdev", O_RDONLY,0);
					pdata->pid = pid;
					pdata->data = buffer;
					if (fd < 0) 
					{
						perror("open");
        				return -1;
    				}
    				ioctl(fd,CHARDEV_MEMSTAT,pdata);
    				printf("%s\n", pdata->data);
				}							
				break;
			case 'd':
				pid = atoi(optarg);
				printf("processdetail %d\n",pid);
				{
					struct type2_data  *pdata = (struct type2_data*)malloc(sizeof(struct type2_data));
					int fd = open("/dev/memdev", O_RDONLY,0);
					pdata->pid = pid;
					pdata->data = buffer;
					if (fd < 0) 
					{
						perror("open");
        				return -1;
    				}
    				ioctl(fd,CHARDEV_PROCDETAIL,pdata);
    				printf("%s\n", pdata->data);
				}					
				break;
			case 'h':
				printf("help\n");
				help();
				break;
		}
	}
	if(optind != argc)
	{
		help();
		exit(0);
	}
	exit(0);
}
