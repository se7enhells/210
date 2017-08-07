objs := button.o  interrupt.o led.o mmu.o uart.o nand.o

all :  $(objs)
	arm-linux-ld -r -o dev.o $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

%.o : %.c
	arm-linux-gcc $(CFLAGS) -c $^

clean:
	rm -f *.o	


