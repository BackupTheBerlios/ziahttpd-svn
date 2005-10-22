##
## windows.mk for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Sat Oct 08 00:01:06 2005 texane
## Last update Fri Oct 21 13:20:06 2005 texane
##

!ifndef __WINDOWS_MK
__WINDOWS_MK	=	1


#
# [*] Windows environmement inferred rules


# - tools


# .compiler
CC	=	cl -nologo
CCPATHS	=	-I"c:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Include/"	\
		-I"c:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/Include/" \
		-I"$(PROJ_ROOT)/src/include/"
CCDEFS	=	-D "_WIN32" -D "_CONSOLE" -D "_MBCS" -D "_MT" -D "_SYSAPI"=2 -TP -MD
CCGEN	=	-O2 -Wp64 -EHsc
CCWARNS	=	-W3
CCOUT	=	-c
CCFLAGS	=	$(CCPATHS) $(CCDEFS) $(CCGEN) $(CCWARNS)

# .linker
LD	=	link -nologo
LDPATHS	=	-libpath:"c:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Lib/" \
		-libpath:"c:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/Lib/"
LDLIBS	=	-implib:user32.lib kernel32.lib gdi32.lib WS2_32.lib
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


.SUFFIXES	: .lo .cc .hh .lib .o .x .mk
force		:
.cc.o		:
		$(CC) $(CCFLAGS) $(CCOUT) $<
		$(MV) $(<F:.cc=.obj) $@
.o.lib		:
.o.lo		:
.o.x		:
		$(LD) $(LDFLAGS) $(LDOUT)$@ $**

!endif # ! __WINDOWS_MK
