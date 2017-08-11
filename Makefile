OBJS := start.o main.o mem.o boot.o dev/dev.o lib/lib.o net/net.o 
	
CFLAGS := -fno-builtin -I$(shell pwd)/include
export CFLAGS

gboot.bin : gboot.elf	
	arm-linux-objcopy -O binary gboot.elf gboot.bin

gboot.elf : $(OBJS)
	arm-linux-ld -Tgboot.lds -o gboot.elf $^

%.o : %.S
	arm-linux-gcc -g -c $^

%.o : %.c
	arm-linux-gcc $(CFLAGS) -c $^

lib/lib.o :
	make -C lib all
	
dev/dev.o :
	make -C dev all	
	
net/net.o :
	make -C net all	
	
.PHONY: clean
clean:
	rm *.o *.elf *.bin
	make -C lib clean
	make -C dev clean


