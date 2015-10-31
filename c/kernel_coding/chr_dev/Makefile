obj-m=my_chr.o

PWD=$(shell pwd)
KERNDIR=/lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules

clean:
	rm *.ko *.mod.c *.o [Mm]odule*
