obj-m: main.o

MAKE=make
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERN_DIR=/
HOST_KERN_DIR=/lib/modules/$(shell uname -r)/build/
all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean
help:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help
host:
	$(MAKE) -C $(HOST_KERN_DIR) M=$(PWD) modules