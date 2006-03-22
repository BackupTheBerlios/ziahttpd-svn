##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 18:43:53 2005 texane
## Last update Wed Mar 22 10:39:54 2006 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= mod_resource.lo
srcs	= src$(SLASH)main.cc \
	  src$(SLASH)buffer.cc \
	  src$(SLASH)resource_handle.cc \
	  src$(SLASH)resource_manager.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)module.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)insock.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)process.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)time.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc
objs	= $(srcs:.cc=.o)
trash	= $(srcs:.cc=.cc~) \
	  $(objs:.o=.o~) \
	  Makefile~

# Override default variables here

# Give your own rules here
all	: $(name)
$(name)	: $(objs)
	$(LD) $(LDFLAGS) $(DLLOUT) $(LDOUT)$(name) $(objs)
clean	: force
	$(ECHO) deleting $(objs)
	$(RM) $(objs)
	$(RM) $(trash)
fclean	: clean
	$(RM) $(name)
re	: clean all
