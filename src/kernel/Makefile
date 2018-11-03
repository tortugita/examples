obj-m := hello.o
obj-m += example01.o
obj-m += example02-1.o
obj-m += example02-2.o
obj-m := example02.o
example02-objs := example02-1.o example02-2.o
obj-m := timer.o
obj-m := example03.o
obj-m := example04.o

KDIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KDIR)  SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
