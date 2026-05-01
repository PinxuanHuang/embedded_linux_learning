obj-m:= pcd_device_setup.o

MAKE=make
ARCH=arm64
CROSS_COMPILE=arm-linux-gnueabihf-
KERN_DIR=/
HOST_KERN_DIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd)

all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean
help:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help
host:
	$(MAKE) -C $(HOST_KERN_DIR) M=$(PWD) modules