CONFIG_MODULE_SIG=n

obj-m += kprobe-example.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
load:
	sudo insmod kprobe-example.ko
	sudo dmesg -c
unload:
	sudo rmmod kprobe_example.ko
	sudo dmesg -c

