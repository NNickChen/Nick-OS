FNT_CHINESE = chinese.org

TOOLPATH = ../../z_tools/
INCPATH  = ../../z_tools/nick/

BIM2BIN = $(TOOLPATH)bim2bin.exe -osacmp
MAKE     = $(TOOLPATH)make.exe -r
DEL      = del

default :
	$(MAKE) src_only
	$(MAKE) chinese.fnt
	
chinese.fnt : Makefile
	$(BIM2BIN) in:$(FNT_CHINESE) out:chinese.fnt
	
src_only :
	-$(DEL)	chinese.fnt
	
tek2 : Makefile
	$(BIM2BIN) in:$(FNT_CHINESE) out:chinese.fnt -tek2