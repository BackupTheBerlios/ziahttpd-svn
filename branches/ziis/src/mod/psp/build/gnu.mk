##
## gnu.mk for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Sat Oct 08 00:03:55 2005 texane
## Last update Fri Apr  7 01:52:07 2006 
##


ifndef __GNU_MK
__GNU_MK	=	1


#
# [*] GNU environmement makefile

# - tools


# .compiler
CC	:=	g++ -rdynamic
CCPATHS	:=	-I$(PROJ_ROOT)/src/include/	\
		-I$(PROJ_ROOT)/src/include/sys \
		-I/usr/pkg/lib/perl5/5.8.0/i386-netbsd-thread-multi/CORE/ \
		-Isrc/include
CCDEFS	:=	-D_SYSAPI=1 -D_REENTRANT
CCGEN	:=	-O2 -ggdb
CCWARNS	:=	-Wall
CCOUT	:=	-c
CCFLAGS	:=	$(CCPATHS) $(CCDEFS) $(CCGEN) $(CCWARNS) -DTIXML_USE_STL

# .linker
osname	:=	$(shell uname -s)
LD	:=	g++ -rdynamic		\
		-Xlinker -rpath -Xlinker /usr/pkg/lib/perl5/5.8.0/i386-netbsd-thread-multi/CORE/	\
		-lperl
LDPATHS	:=	-L/usr/pkg/lib/perl5/5.8.0/i386-netbsd-thread-multi/CORE/
LDLIBS	:=
ifeq ($(osname), Linux)
LDLIBS	+=	-ldl
endif
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
