##
## windows.mk for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Sat Oct 08 00:01:06 2005 texane
## Last update Wed Apr 05 14:48:26 2006 texane
##

!ifndef __WINDOWS_MK
__WINDOWS_MK	=	1


#
# [*] Windows environmement inferred rules


# - tools


# .compiler
CC	=	cl -nologo
CCPATHS	=	-I"d:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Include/"	\
		-I"d:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/Include/" \
		-I"$(PROJ_ROOT)/src/include/" \
		-I"$(PROJ_ROOT)/src/include/sys" \
		-I"src/include/perl" \
		-I"src/include"
CCDEFS	=	-D "_WIN32" -D "_CONSOLE" -D "_MBCS" -D "_MT" -D "_SYSAPI"=2 -MD -DTIXML_USE_STL
CCGEN	=	-O2 -GX -GR
CCWARNS	=	-W3
CCOUT	=	-c
CCFLAGS	=	$(CCPATHS) $(CCDEFS) $(CCGEN) $(CCWARNS)

# .linker
LD	=	link -nologo
LDPATHS	=	-libpath:"d:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Lib/" \
		-libpath:"d:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/Lib/" \
		-libpath:"$(PROJ_ROOT)/src/lib" \
		-libpath:"lib"
LDLIBS	=	-defaultlib:Perl58.lib \
		-defaultlib:PerlEz.lib \
		-defaultlib:pthreadVSE1.lib
LDGEN	=	-machine:x86 -subsystem:console -opt:noref
LIBOUT	=	-lib
DLLOUT	=	-dll
LDOUT	=	-out:
LDFLAGS	=	$(LDPATHS) $(LDLIBS) $(LDGEN)

# .misc
RM	=	@del
CP	=	@copy
MV	=	@move
ECHO	=	@echo
CD	=	@cd
MAKE	=	@nmake -nologo
SLASH	=	\\


# - inferred rules


.SUFFIXES	: .lo .c .cc .hh .h .lib .o .x .mk
force		:
.c.o		:
		$(CC) $(CCFLAGS) $(CCOUT) $<
		$(MV) $(<F:.c=.obj) $@
.cc.o		:
		$(CC) $(CCFLAGS) $(CCOUT) $<
		$(MV) $(<F:.cc=.obj) $@
.o.lib		:
.o.lo		:
		$(LD) $(LDFLAGS) $(DLLOUT) $(LDOUT)$@ $**
.o.x		:
		$(LD) $(LDFLAGS) $(LDOUT)$@ $**

!endif # ! __WINDOWS_MK