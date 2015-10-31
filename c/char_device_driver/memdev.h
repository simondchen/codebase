#ifndef _MEMDEV_H_
#define _MEMDEV_H_

#define MAJOR_DEVICE_NUM 30
#define MAX_MINOR_NUM 2

#define MEMDEV_NUM 2
#define MEMDEV_SIZE 4096

struct mem_dev
{
	char *data;
	unsigned long size;
};

struct type1_data
{
	char *data;
};

struct type2_data
{
	int pid;
	char *data;
};
#define CHARDEV_IOC_MAGIC  'k'


#define CHARDEV_PSTREE  _IOR(CHARDEV_IOC_MAGIC, 1,struct type1_data)
#define CHARDEV_THREADGRP _IOR(CHARDEV_IOC_MAGIC, 2, struct type1_data)
#define CHARDEV_MEMSTAT _IOWR(CHARDEV_IOC_MAGIC,3, struct type2_data)
#define CHARDEV_PROCDETAIL _IOWR(CHARDEV_IOC_MAGIC, 4,struct type2_data)



#endif
