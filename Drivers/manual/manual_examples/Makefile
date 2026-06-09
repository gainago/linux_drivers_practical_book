obj-m += hello-1.o
obj-m += hello_2.o
obj-m += hello-3.o
obj-m += hello-4.o
obj-m += hello-5.o
obj-m += startstop.o
startstop-objs := start.o stop.o

PWD := $(shell pwd)
VERSION := $(shell uname -r)
ccflags-y := -g -O0 -DDEBUG
all:
	make -C /lib/modules/$(VERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(VERSION)/build M=$(PWD) clean
