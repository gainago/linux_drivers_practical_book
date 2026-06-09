CONFIG_MODULE_SIG=n

obj-m += char_dev.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
load:
	sudo insmod char_dev.ko
	sudo dmesg -c
unload:
	sudo rmmod char_dev.ko
	sudo dmesg -c

