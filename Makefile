OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj cmos.obj console.obj window.obj file.obj disk.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/nick/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2NCK  = $(TOOLPATH)bim2nck.exe
RULEFILE = $(TOOLPATH)nick/nick.rul
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
	$(BIM2NCK) bootpack.bim bootpack.nck 0

a.bim : a.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:a.bim map:a.map a.obj a_nask.obj
		
a.nck : a.bim Makefile
	$(BIM2NCK) a.bim a.nck 0
	
hello3.bim : hello3.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello3.bim map:hello3.map hello3.obj a_nask.obj
	
hello3.nck : hello3.bim Makefile
	$(BIM2NCK) hello3.bim hello3.nck 0
	
hello4.bim : hello4.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello4.bim stack:1k map:hello4.map \
	hello4.obj a_nask.obj
	
hello4.nck : hello4.bim Makefile
	$(BIM2NCK) hello4.bim hello4.nck 0
	
hello5.bim : hello5.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello5.bim stack:1k map:hello5.map\
	hello5.obj
	
hello5.nck : hello5.bim Makefile
	$(BIM2NCK) hello5.bim hello5.nck 0
	
winhello.bim : winhello.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhello.bim stack:1k map:winhello.map\
	winhello.obj a_nask.obj
	
winhello.nck : winhello.bim Makefile
	$(BIM2NCK) winhello.bim winhello.nck 0
	
winhelo1.bim : winhelo1.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo1.bim stack:1k map:winhelo1.map\
	winhelo1.obj a_nask.obj
	
winhelo1.nck : winhelo1.bim Makefile
	$(BIM2NCK) winhelo1.bim winhelo1.nck 40k
	
	
stars.bim : stars.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:stars.bim stack:1k map:stars.map stars.obj a_nask.obj	
	
stars.nck : stars.bim Makefile
	$(BIM2NCK) stars.bim stars.nck 47k
	
about.bim : about.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:about.bim stack:1k map:about.map about.obj a_nask.obj	
	
about.nck : about.bim Makefile
	$(BIM2NCK) about.bim about.nck 47k
	
lines.bim : lines.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:lines.bim stack:1k map:lines.map \
		lines.obj a_nask.obj

lines.nck : lines.bim Makefile
	$(BIM2NCK) lines.bim lines.nck 48k
	
walk.bim : walk.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:walk.bim stack:1k map:walk.map \
		walk.obj a_nask.obj

walk.nck : walk.bim Makefile
	$(BIM2NCK) walk.bim walk.nck 48k
	
swatch.bim : swatch.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:swatch.bim stack:1k map:swatch.map\
		swatch.obj a_nask.obj
		
swatch.nck : swatch.bim Makefile
	$(BIM2NCK) swatch.bim swatch.nck 50k
	
beepdown.bim : beepdown.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:beepdown.bim stack:1k map:beepdown.map\
		beepdown.obj a_nask.obj
		
beepdown.nck : beepdown.bim Makefile
	$(BIM2NCK) beepdown.bim beepdown.nck 0
	
beepup.bim : beepup.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:beepup.bim stack:1k map:beepup.map\
		beepup.obj a_nask.obj
		
beepup.nck : beepup.bim Makefile
	$(BIM2NCK) beepup.bim beepup.nck 0
	
atimer.bim : atimer.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:atimer.bim stack:1k map:atimer.map\
		atimer.obj a_nask.obj
		
timer.nck : atimer.bim Makefile
	$(BIM2NCK) atimer.bim timer.nck 48k
	
draw.bim : draw.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:draw.bim stack:40k map:draw.map\
		draw.obj a_nask.obj
		
draw.nck : draw.bim Makefile
	$(BIM2NCK) draw.bim draw.nck 48k
	
color.bim : color.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:color.bim stack:24k map:color.map\
		color.obj a_nask.obj
		
color.nck : color.bim Makefile
	$(BIM2NCK) color.bim color.nck 48k

textbox.bim : textbox.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:textbox.bim stack:20k map:textbox.map\
		textbox.obj a_nask.obj
		
textbox.nck : textbox.bim Makefile
	$(BIM2NCK) textbox.bim textbox.nck 48k
nick.sys : asmhead.bin bootpack.nck Makefile
	copy /B asmhead.bin+bootpack.nck nick.sys

nick.img : ipl10.bin nick.sys hello5.nck winhello.nck\
	a.nck hello3.nck winhelo1.nck hello4.nck stars.nck\
	about.nck lines.nck walk.nck swatch.nck beepdown.nck\
	beepup.nck timer.nck draw.nck color.nck textbox.nck Makefile 
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:nick.sys to:@: \
		copy from:hankaku.txt to:@: \
		copy from:ipl10.nas to:@: \
		copy from:welcome.txt to:@: \
		copy from:a.nck to:@: \
		copy from:hello3.nck to:@: \
		copy from:hello4.nck to:@: \
		copy from:hello5.nck to:@: \
		copy from:winhello.nck to:@:\
		copy from:winhelo1.nck to:@:\
		copy from:stars.nck to:@:\
		copy from:about.nck to:@:\
		copy from:lines.nck to:@:\
		copy from:walk.nck to:@:\
		copy from:swatch.nck to:@:\
		copy from:beepdown.nck to:@:\
		copy from:beepup.nck to:@:\
		copy from:timer.nck to:@:\
		copy from:draw.nck to:@:\
		copy from:color.nck to:@:\
		copy from:textbox.nck to:@:\
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

install:
	$(MAKE) img
	copy nick.img J:\nick.img


clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.map
	-$(DEL) *.bim
	-$(DEL) *.nck
	-$(DEL) nick.sys

src_only :
	$(MAKE) clean
	-$(DEL) nick.img
