CC = arm-linux-gcc
CFLAGS = -Wall 
LIBS = -L /opt/arm_libs/lib -ljpeg
INCLUDE =  -I /opt/arm_libs/include
LCD:main.o list.o dir.o bmp.o jpg.o tauch.o 
	${CC} ${CFLAGS}   -o $@ $^  ${INCLUDE}  ${LIBS}
	#cp LCD /home/wanfeng/tftp/
    
%.o:%.c
	$(CC) -c $< -o $@ ${INCLUDE}  ${LIBS}

    
clean:
	rm -f main.o list.o dir.o bmp.o jpg.o tauch.o LCD
    
