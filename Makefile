ifneq ($(KERNELRELEASE),)
	obj-m := multiclip.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *~ *.ko *.mod.c Module.* *.order .tmp_versions \.*.cmd
	
install:
	init.sh
