##
## gnu.mk for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Sat Oct 08 00:03:55 2005 texane
## Last update Mon Oct 17 18:53:48 2005 
##


ifndef __GNU_MK
__GNU_MK	=	1


#
# [*] GNU environmement makefile

# - tools


# .compiler
CC	:=	g++
CCPATHS	:=	-I$(PROJ_ROOT)/src/include/
CCDEFS	:=	-D_SYSAPI=1
CCGEN	:=	-O2
CCWARNS	:=	-Wall
CCOUT	:=	-c
CCFLAGS	:=	$(CCPATHS) $(CCDEFS) $(CCGEN) $(CCWARNS)

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