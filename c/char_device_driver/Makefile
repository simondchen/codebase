chardev-objs := function.o memdev.o
obj-m := chardev.o 

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc  systeminfo.c -o systeminfo

clean:
	rm -f *.o *.ko modules.order Module.symvers memdev.mod.c
