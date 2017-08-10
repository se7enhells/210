objs := arp.o tftp.o

all :  $(objs)
	arm-linux-ld -r -o net.o $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

%.o : %.c
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o	


