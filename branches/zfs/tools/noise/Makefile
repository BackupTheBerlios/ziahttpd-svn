##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Tue Feb 14 01:04:44 2006 texane
## Last update Thu Feb 23 11:10:49 2006 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= noise.x
srcs	= src$(SLASH)main.cc			\
	  src$(SLASH)buffer.cc			\
	  src$(SLASH)proxy.cc			\
	  src$(SLASH)proxy_thr.cc		\
	  src$(SLASH)noise.cc			\
	  src$(SLASH)noise_rand.cc		\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)insock.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)module.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)time.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)process.cc
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
