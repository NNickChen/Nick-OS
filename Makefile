TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/nick/

MAKE     = $(TOOLPATH)make.exe -r
EDIMG    = $(TOOLPATH)edimg.exe
WRITE    = $(TOOLPATH)img_writer.exe
COPY     = copy
DEL      = del

default :
	$(MAKE) nick.img
	
	
nick.img : nick/nick.sys nick/ipl10.nas Makefile\
	a/a.nck about/about.nck beepdown/beepdown.nck\
	beepup/beepup.nck color/color.nck draw/draw.nck\
	hello/hello.nck lines/lines.nck stars/stars.nck\
	swatch/swatch.nck textbox/textbox.nck timer/timer.nck\
	walk/walk.nck winhello/winhello.nck sosu/sosu.nck\
	typeipl/typeipl.nck
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:nick/ipl10.bin len:512 from:0 to:0 \
		copy from:nick/nick.sys to:@: \
		copy from:nick/hankaku.txt to:@: \
		copy from:nick/ipl10.nas to:@: \
		copy from:welcome.txt to:@: \
		copy from:apilib.h to:@:\
		copy from:app_make.txt to:@:\
		copy from:a/a.nck to:@: \
		copy from:hello/hello.nck to:@: \
		copy from:winhello/winhello.nck to:@:\
		copy from:stars/stars.nck to:@:\
		copy from:about/about.nck to:@:\
		copy from:lines/lines.nck to:@:\
		copy from:walk/walk.nck to:@:\
		copy from:swatch/swatch.nck to:@:\
		copy from:beepdown/beepdown.nck to:@:\
		copy from:beepup/beepup.nck to:@:\
		copy from:timer/timer.nck to:@:\
		copy from:draw/draw.nck to:@:\
		copy from:color/color.nck to:@:\
		copy from:textbox/textbox.nck to:@:\
		copy from:sosu/sosu.nck to:@:\
		copy from:typeipl/typeipl.nck to:@:\
		imgout:nick.img
		
run :
	$(MAKE) nick.img
	$(COPY) nick.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu
	
install :
	$(MAKE) nick.img
	copy nick.img J:\nick.img
	
full :
	$(MAKE) -C nick
	$(MAKE) -C apilib
	$(MAKE) -C a
	
	$(MAKE) -C about
	$(MAKE) -C beepdown
	$(MAKE) -C beepup
	$(MAKE) -C color
	$(MAKE) -C draw
	$(MAKE) -C hello
	$(MAKE) -C lines
	$(MAKE) -C stars
	$(MAKE) -C swatch
	$(MAKE) -C textbox
	$(MAKE) -C timer
	$(MAKE) -C walk
	$(MAKE) -C winhello
	$(MAKE) -C sosu
	$(MAKE) -C typeipl
	$(MAKE) nick.img
	
run_full :
	$(MAKE) full
	$(MAKE) run
	
install_full :
	$(MAKE) full
	$(MAKE) install
	
run_os :
	$(MAKE) -C nick
	$(MAKE) run
	
clean :


src_only :
	$(MAKE) clean
	-$(DEL) nick.img
	
clean_full :
	$(MAKE) -C nick		clean
	$(MAKE) -C apilib	clean
	$(MAKE) -C a		clean
	
	$(MAKE) -C about	clean
	$(MAKE) -C beepdown	clean
	$(MAKE) -C beepup	clean
	$(MAKE) -C color	clean
	$(MAKE) -C draw		clean
	$(MAKE) -C hello	clean
	$(MAKE) -C lines	clean
	$(MAKE) -C stars	clean
	$(MAKE) -C swatch	clean
	$(MAKE) -C textbox	clean
	$(MAKE) -C timer	clean
	$(MAKE) -C walk		clean
	$(MAKE) -C winhello	clean
	$(MAKE) -C sosu		clean
	$(MAKE) -C typeipl	clean
	
src_only_full :
	$(MAKE) -C nick		src_only
	$(MAKE) -C apilib	src_only
	$(MAKE) -C a		src_only
	
	$(MAKE) -C about	src_only
	$(MAKE) -C beepdown	src_only
	$(MAKE) -C beepup	src_only
	$(MAKE) -C color	src_only
	$(MAKE) -C draw		src_only
	$(MAKE) -C hello	src_only
	$(MAKE) -C lines	src_only
	$(MAKE) -C stars	src_only
	$(MAKE) -C swatch	src_only
	$(MAKE) -C textbox	src_only
	$(MAKE) -C timer	src_only
	$(MAKE) -C walk		src_only
	$(MAKE) -C winhello	src_only
	$(MAKE) -C sosu 	src_only
	$(MAKE) -C typeipl	src_only
	-$(DEL) nick.img
	
refresh :
	$(MAKE) full
	$(MAKE) clean_full
	-$(DEL) nick.img