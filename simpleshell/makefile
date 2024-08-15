CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

sources = $(wildcard *.c) 
# $()為在makefile下變數引用函數的方法，wildcard為makefile一種函數:找此目錄下所有符合*.c的檔案
objects = $(sources:.c=.o) 
#sources當中所有file後有後墜.c檔都生成為*.o檔且生成後儲存在objects

all:	clean kernel8.img

# 編譯assembly
a.o:	a.S
	aarch64-linux-gnu-gcc $(CFLAGS) -c a.S -o a.o

# linking
%.o: %.c  
    # %是makefile配對變數的技巧
	aarch64-linux-gnu-gcc $(CFLAGS) -c $< -o $@

kernel8.img:	a.o $(objects)
	aarch64-linux-gnu-ld a.o $(objects) -T linker.ld -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio