##
## windows.mk for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Sat Oct 08 00:01:06 2005 texane
## Last update Sun Jan 29 11:59:36 2006 texane
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
		-I"$(PROJ_ROOT)/src/lib/tinyxml/" \
		-I"$(PROJ_ROOT)/src/include/"
CCDEFS	=	-DTIXML_USE_STL -D "_WIN32" -D "_CONSOLE" -D "_MBCS" -D "_MT" -D "_SYSAPI"=2 -MD
CCGEN	=	-O2 -Wp64 -EHsc
CCWARNS	=	-W3
CCOUT	=	-c
CCFLAGS	=	$(CCPATHS) $(CCDEFS) $(CCGEN) $(CCWARNS)

# .linker
LD	=	link -nologo
LDPATHS	=	-libpath:"c:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Lib/" \
		-libpath:"c:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/Lib/" \
		-libpath:"lib"
LDLIBS	=	-defaultlib:Ws2_32.lib -defaultlib:psapi.lib
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


.SUFFIXES	: .lo .c .hh .lib .o .x .mk
force		:
.c.o		:
		$(CC) $(CCFLAGS) $(CCOUT) $<
		$(MV) $(<F:.c=.obj) $@
.o.lib		:
.o.lo		:
		$(LD) $(LDFLAGS) $(DLLOUT) $(LDOUT)$@ $**
.o.x		:
		$(LD) $(LDFLAGS) $(LDOUT)$@ $**

!endif # ! __WINDOWS_MK
