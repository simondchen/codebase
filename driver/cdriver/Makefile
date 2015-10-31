ifneq ($(KERNELRELEASE),)
#mymodule-objs := hello_module.o
obj-m := hello_module.o

else
PWD := $(shell pwd)
KVER ?= $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions modules.order Module.*
endif
