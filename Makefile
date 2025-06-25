OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj cmos.obj console.obj window.obj file.obj disk.obj
		
OBJS_API = api001.obj api002.obj api003.obj api004.obj api005.obj api006.obj\
		api007.obj api008.obj api009.obj api010.obj api011.obj api012.obj api013.obj\
		api014.obj api015.obj api016.obj api017.obj api018.obj api019.obj api020.obj\
		api021.obj api022.obj

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
GOLIB	 = $(TOOLPATH)golib00.exe
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

apilib.lib : $(OBJS_API) Makefile
	$(GOLIB) $(OBJS_API) out:apilib.lib
	
a.bim : a.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:a.bim map:a.map a.obj apilib.lib
		
a.nck : a.bim Makefile
	$(BIM2NCK) a.bim a.nck 0
	
hello3.bim : hello3.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello3.bim map:hello3.map hello3.obj apilib.lib
	
hello3.nck : hello3.bim Makefile
	$(BIM2NCK) hello3.bim hello3.nck 0
	
hello4.bim : hello4.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello4.bim stack:1k map:hello4.map \
	hello4.obj apilib.lib
	
hello4.nck : hello4.bim Makefile
	$(BIM2NCK) hello4.bim hello4.nck 0
	
hello5.bim : hello5.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello5.bim stack:1k map:hello5.map\
	hello5.obj
	
hello5.nck : hello5.bim Makefile
	$(BIM2NCK) hello5.bim hello5.nck 0
	
winhello.bim : winhello.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhello.bim stack:1k map:winhello.map\
	winhello.obj apilib.lib
	
winhello.nck : winhello.bim Makefile
	$(BIM2NCK) winhello.bim winhello.nck 0
	
winhelo1.bim : winhelo1.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo1.bim stack:1k map:winhelo1.map\
	winhelo1.obj apilib.lib
	
winhelo1.nck : winhelo1.bim Makefile
	$(BIM2NCK) winhelo1.bim winhelo1.nck 40k
	
	
stars.bim : stars.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:stars.bim stack:1k map:stars.map stars.obj apilib.lib	
	
stars.nck : stars.bim Makefile
	$(BIM2NCK) stars.bim stars.nck 47k
	
about.bim : about.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:about.bim stack:1k map:about.map about.obj apilib.lib	
	
about.nck : about.bim Makefile
	$(BIM2NCK) about.bim about.nck 47k
	
lines.bim : lines.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:lines.bim stack:1k map:lines.map \
		lines.obj apilib.lib

lines.nck : lines.bim Makefile
	$(BIM2NCK) lines.bim lines.nck 48k
	
walk.bim : walk.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:walk.bim stack:1k map:walk.map \
		walk.obj apilib.lib

walk.nck : walk.bim Makefile
	$(BIM2NCK) walk.bim walk.nck 48k
	
swatch.bim : swatch.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:swatch.bim stack:1k map:swatch.map\
		swatch.obj apilib.lib
		
swatch.nck : swatch.bim Makefile
	$(BIM2NCK) swatch.bim swatch.nck 50k
	
beepdown.bim : beepdown.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:beepdown.bim stack:1k map:beepdown.map\
		beepdown.obj apilib.lib
		
beepdown.nck : beepdown.bim Makefile
	$(BIM2NCK) beepdown.bim beepdown.nck 0
	
beepup.bim : beepup.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:beepup.bim stack:1k map:beepup.map\
		beepup.obj apilib.lib
		
beepup.nck : beepup.bim Makefile
	$(BIM2NCK) beepup.bim beepup.nck 0
	
atimer.bim : atimer.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:atimer.bim stack:1k map:atimer.map\
		atimer.obj apilib.lib
		
timer.nck : atimer.bim Makefile
	$(BIM2NCK) atimer.bim timer.nck 48k
	
draw.bim : draw.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:draw.bim stack:40k map:draw.map\
		draw.obj apilib.lib
		
draw.nck : draw.bim Makefile
	$(BIM2NCK) draw.bim draw.nck 48k
	
color.bim : color.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:color.bim stack:24k map:color.map\
		color.obj apilib.lib
		
color.nck : color.bim Makefile
	$(BIM2NCK) color.bim color.nck 48k

textbox.bim : textbox.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:textbox.bim stack:20k map:textbox.map\
		textbox.obj apilib.lib
		
textbox.nck : textbox.bim Makefile
	$(BIM2NCK) textbox.bim textbox.nck 48k
	
nick.sys : asmhead.bin bootpack.nck Makefile
	copy /B asmhead.bin+bootpack.nck nick.sys

nick.img : ipl10.bin nick.sys hello5.nck winhello.nck\
	a.nck hello3.nck winhelo1.nck hello4.nck stars.nck\
	about.nck lines.nck walk.nck swatch.nck beepdown.nck\
	beepup.nck timer.nck draw.nck color.nck textbox.nck \
	Makefile 
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
