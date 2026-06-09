obj-m += kobject-example.o
obj-m += kset-example.o
obj-m += hello_sysfs.o
ccflags-y := -g -DDEBUG 
PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
