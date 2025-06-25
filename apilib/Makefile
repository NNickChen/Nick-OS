OBJS_API = api001.obj api002.obj api003.obj api004.obj api005.obj api006.obj\
		api007.obj api008.obj api009.obj api010.obj api011.obj api012.obj api013.obj\
		api014.obj api015.obj api016.obj api017.obj api018.obj api019.obj api020.obj\
		api021.obj api022.obj api023.obj api024.obj api025.obj api026.obj api027.obj \
		api028.obj api029.obj alloca.obj
		
TOOLPATH = ../../z_tools/
INCPATH  = ../../z_tools/nick/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
GOLIB	 = $(TOOLPATH)golib00.exe
COPY     = copy
DEL      = del


default :
	$(MAKE) apilib.lib
	
apilib.lib : $(OBJS_API) Makefile
	$(GOLIB) $(OBJS_API) out:apilib.lib
	
%.gas : %.c bootpack.h Makefile
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
	-$(DEL) apilib.lib