PRIFIX  =
AS	= nasm
CC	= $(PRIFIX)gcc -O2 -std=c17
LD 	= $(PRIFIX)ld
OBJCOPY = $(PRIFIX)objcopy

CFLAGS  = -c -g -Wall -Wextra -I../include   \
          -mcmodel=large -fno-hosted -fno-stack-protector

OBJS	= $(patsubst %.s,%.o ,$(wildcard *.s)) \
          $(patsubst %.c,%.o ,$(wildcard *.c))

.s.o:
	nasm  -felf64 -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -o $@ $<

kernel.bin: $(OBJS) kernel.lds Makefile
	$(LD) -static -T kernel.lds -Map kernel.map -o kernel.dbg $(OBJS)
	$(OBJCOPY) -S -O binary kernel.dbg kernel.bin

dep:
	sed -n 1,/\#\#\#Depend/p Makefile > makefile.tmp
	for file in $(wildcard *.c); do \
		$(CC) $(CFLAGS) -M $$file >> makefile.tmp; \
	done
	mv makefile.tmp Makefile

clean:
	-rm kernel.bin kernel.dbg kernel.map $(wildcard *.o)

###Depend
clock.o: clock.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/traps.h ../include/cnix/asm.h ../include/cnix/desc.h \
 ../include/cnix/lapic.h
cpu.o: cpu.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/desc.h ../include/cnix/traps.h ../include/cnix/sched.h
exceptions.o: exceptions.c ../include/cnix/config.h \
 ../include/cnix/kernel.h ../include/cnix/traps.h ../include/cnix/asm.h \
 ../include/cnix/sched.h
ide.o: ide.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/asm.h ../include/cnix/lapic.h ../include/cnix/traps.h \
 ../include/cnix/desc.h
ioapic.o: ioapic.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/asm.h ../include/cnix/traps.h
keyboard.o: keyboard.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/asm.h ../include/cnix/traps.h ../include/cnix/desc.h \
 ../include/cnix/lapic.h
lapic.o: lapic.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/asm.h ../include/cnix/lapic.h ../include/cnix/traps.h \
 ../include/cnix/desc.h
memory.o: memory.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/asm.h ../include/cnix/desc.h ../include/cnix/traps.h \
 ../include/cnix/spinlock.h
mktime.o: mktime.c
printk.o: printk.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/spinlock.h
sched.o: sched.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/sched.h
smp.o: smp.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/lapic.h
startup.o: startup.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/asm.h ../include/cnix/desc.h ../include/cnix/traps.h \
 ../include/cnix/sched.h
vga.o: vga.c ../include/cnix/config.h ../include/cnix/kernel.h \
 ../include/cnix/asm.h
vsprintf.o: vsprintf.c ../include/cnix/kernel.h
