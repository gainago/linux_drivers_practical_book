obj-m += sleep.o

PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules V=1
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
