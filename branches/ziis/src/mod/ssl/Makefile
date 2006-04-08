##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 18:43:53 2005 texane
## Last update Sat Apr  8 19:17:58 2006 
##

# ! Always include this file
# !include "build/$(PROJ_ENV).mk" #windows
include build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= mod_ssl.lo
srcs	= src$(SLASH)main.cc \
	  src$(SLASH)conimpl.cc \
	  src$(SLASH)modimpl.cc
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
