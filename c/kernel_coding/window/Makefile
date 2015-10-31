obj-m=window.o

PWD=$(shell pwd)
KERNDIR=/lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules
	gcc read_window.c -o read_window

install:
	sudo insmod window.ko

uninstall:
	sudo rmmod window

clean:
	rm *.o *.mod.c [Mm]odule* *.ko read_window
