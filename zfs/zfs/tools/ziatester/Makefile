##
## Makefile<4> for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 20:39:38 2005 texane
## Last update Wed Dec 07 21:49:59 2005 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= zt.x
srcs	= src$(SLASH)main.cc	\
	  src$(SLASH)ztclient.cc \
	  src$(SLASH)ztspool.cc \
	  src$(SLASH)ztthread.cc \
	  src$(SLASH)ztrequest.cc \
	  src$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)thread.cc \
	  src$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)socket_in.cc \
	  src$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc \
	  src$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc

objs	= $(srcs:.cc=.o)
trash	= $(srcs:.cc=.cc~) \
	  $(objs:.o=.o~) \
	  Makefile~

# Override default variables here

# Give your own rules here
all	: $(name)
$(name)	: $(objs)
	$(LD) $(LDFLAGS) $(LDOUT)$(name) $(objs)
clean	: force
	$(ECHO) deleting $(objs)
	$(RM) $(objs)
	$(RM) $(trash)
fclean	: clean
	$(RM) $(name)
re	: clean all
