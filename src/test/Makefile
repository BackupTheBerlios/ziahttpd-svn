##
## Makefile<4> for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 05 12:02:12 2005 texane
## Last update Tue Oct 11 20:59:45 2005 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk"

# Give your own variables here
name	= ziad.x
srcs	= main.cc \
	  test_file.cc \
	  test_process.cc \
	  test_thread.cc \
	  ..$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc	\
	  ..$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)process.cc \
	  ..$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)thread.cc \
	  ..$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc
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
