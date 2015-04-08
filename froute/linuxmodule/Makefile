obj-m += fr.o

fr-objs := route.o scheduler.o processor.o frouter.o

all:
	make -C /home/vikas/rack/qemu-img/uml/mods/lib/modules/3.12.38-gef1a101/build M=$(PWD) modules

clean:
	make -C /home/vikas/rack/qemu-img/uml/mods/lib/modules/3.12.38-gef1a101/build M=$(PWD) clean