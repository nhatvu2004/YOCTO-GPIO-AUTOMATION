obj-m += gpio-driver.o
KERNEL_SRC = ${STAGING_KERNEL_DIR}

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
	

