OBJS_RFUNC = putchar.obj exit.obj printf.obj malloc.obj free.obj
		
TOOLPATH = ../../z_tools/
INCPATH  = ../../z_tools/nick/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
GOLIB	 = $(TOOLPATH)golib00.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -I../ -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
NASK     = $(TOOLPATH)nask.exe
COPY     = copy
DEL      = del


default :
	$(MAKE) rfunc.lib
	
rfunc.lib : $(OBJS_RFUNC) Makefile
	$(GOLIB) $(OBJS_RFUNC) out:rfunc.lib
	
%.gas : %.c ../apilib.h Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst
	
clean :
	-$(DEL) *.lst
	-$(DEL) *.obj

src_only :
	$(MAKE) clean
	-$(DEL) rfunc.lib