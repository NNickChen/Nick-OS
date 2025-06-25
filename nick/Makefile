OBJS_BOOTPACK = bootpack.obj naskfunc.obj english.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj cmos.obj console.obj api.obj cmd.obj window.obj file.obj tek.obj
		
TOOLPATH = ../../z_tools/
INCPATH  = ../../z_tools/nick/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2NCK  = $(TOOLPATH)bim2nck.exe
RULEFILE = ../nick.rul
COPY     = copy
DEL      = del

default :
	$(MAKE) ipl10.bin
	$(MAKE) -C ../rfunc
	$(MAKE) nick.sys

ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst
	

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

english.bin : english.txt Makefile
	$(MAKEFONT) english.txt english.bin

english.obj : english.bin Makefile
	$(BIN2OBJ) english.bin english.obj _english

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.nck : bootpack.bim Makefile
	$(BIM2NCK) bootpack.bim bootpack.nck 0
	
nick.sys : asmhead.bin bootpack.nck Makefile
	copy /B asmhead.bin+bootpack.nck nick.sys
	
%.gas : %.c bootpack.h Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst
	
	
clean :
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.map
	-$(DEL) *.bim
	-$(DEL) *.nck
	-$(DEL) asmhead.bin
	-$(DEL) english.bin
	
	
src_only :
	$(MAKE) clean
	-$(DEL) nick.sys
	-$(DEL) ipl10.bin
	