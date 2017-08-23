obj-m := vkeyboard.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install
clean:
	rm -f *.mod.c *.ko *.o Module.markers  modules.order  Module.symvers
