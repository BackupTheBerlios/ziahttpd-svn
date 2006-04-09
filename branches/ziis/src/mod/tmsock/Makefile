##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 18:43:53 2005 texane
## Last update Mon Apr 10 00:31:52 2006 
##

# ! Always include this file
# !include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= mod_tmsock.lo
srcs	= src$(SLASH)main.cc \
	  src$(SLASH)modimpl.cc \
	  src$(SLASH)conimpl.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)insock.cc \
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
