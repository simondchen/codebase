obj-m=window.o

PWD=$(shell pwd)
KERNDIR=/lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules

clean:
	rm *.o *.mod.c [Mm]odule* *.ko
