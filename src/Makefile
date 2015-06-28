obj-m := sys_call_hijack.o
#obj-m += livepatch.o
KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
all:
	 $(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD)
clean:
	 rm -rf *.o *.ko *.symvers *.mod.* *.order
