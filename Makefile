OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj cmos.obj console.obj window.obj file.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
WRITE    = $(TOOLPATH)img_writer.exe
COPY     = copy
DEL      = del



default :
	$(MAKE) img



ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst
	

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.nck : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.nck 0

hlt.nck	   : hlt.nas Makefile
	$(NASK)  hlt.nas hlt.nck 
	
nick.sys : asmhead.bin bootpack.nck Makefile
	copy /B asmhead.bin+bootpack.nck nick.sys

nick.img : ipl10.bin nick.sys hlt.nck Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:nick.sys to:@: \
		copy from:hankaku.txt to:@: \
		copy from:ipl10.nas to:@: \
		copy from:welcome.txt to:@: \
		copy from:makefile to:@: \
		copy from:hlt.nck to:@: \
		imgout:nick.img



%.gas : %.c bootpack.h Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst



img :
	$(MAKE) nick.img

run :
	$(MAKE) img
	$(COPY) nick.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install_flash:
	$(MAKE) img
	copy nick.img J:\nick.img
	copy nick.img F:\nick.img
	
install_floppy:
	$(MAKE) img
	$(WRITE)
	


clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) *.nck
	-$(DEL) nick.sys

src_only :
	$(MAKE) clean
	-$(DEL) nick.img
