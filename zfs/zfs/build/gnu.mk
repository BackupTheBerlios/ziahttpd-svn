##
## gnu.mk for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Sat Oct 08 00:03:55 2005 texane
## Last update Sat Feb 11 20:52:42 2006 
##


ifndef __GNU_MK
__GNU_MK	=	1


#
# [*] GNU environmement makefile

# - tools


# .compiler
CC	:=	g++
CCPATHS	:=	-I$(PROJ_ROOT)/src/include/
CCDEFS	:=	-D_SYSAPI=1 -D_REENTRANT
CCGEN	:=	-O2
CCWARNS	:=	-Wall
CCOUT	:=	-c
CCFLAGS	:=	$(CCPATHS) $(CCDEFS) $(CCGEN) $(CCWARNS) -DTIXML_USE_STL -DZIAFS_DEBUG_MSG

# .linker
LD	:=	g++
LDPATHS	:=
LDLIBS	:=	-lpthread
LDGEN	:=
LIBOUT	:=
DLLOUT	:=	-shared
LDOUT	:=	-o
LDFLAGS	:=	$(LDPATHS) $(LDLIBS) $(LDGEN)

# .misc
RM	:=	@rm -f
CP	:=	@cp
MV	:=	@mv
ECHO	:=	@echo
CD	:=	@cd
MAKE	:=	@make
SLASH	:=	/


# - inferred rules


%.o	:	%.cc
		$(CC) $(CCFLAGS) $(CCOUT) -o $@ $<
%.lib	:	%.o
%lo	:	%.o
%.x	:	%.o
		$(CC) $(LDFLAGS) $(LDOUT) $@ $?

.PHONY	:	all clean fclean re force

endif # !__GNU_MK
