obj-m := hello_world.o

PWD = $(shell pwd)
KERNDIR = /lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules

test:
	@echo $(PWD)
	@echo $(KERNDIR)

clean:
	rm module* Module* *.o *.ko *.mod.c
