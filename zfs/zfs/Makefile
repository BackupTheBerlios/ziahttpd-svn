##
## Makefile<4> for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 20:39:38 2005 texane
## Last update Sun Jan 22 03:11:37 2006 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= ziafs.x
srcs	= src$(SLASH)core$(SLASH)main.cc \
	  src$(SLASH)core$(SLASH)buffer.cc \
	  src$(SLASH)core$(SLASH)error.cc \
	  src$(SLASH)core$(SLASH)ioman.cc \
	  src$(SLASH)core$(SLASH)session.cc \
	  src$(SLASH)core$(SLASH)iores.cc \
	  src$(SLASH)core$(SLASH)iores_file.cc
# 	  src$(SLASH)core$(SLASH)iores_proc.cc
# 	  src$(SLASH)core$(SLASH)iores_mem.cc
# 	  src$(SLASH)core$(SLASH)iores_insock.cc
objs	= $(srcs:.cc=.o)
trash	= $(srcs:.cc=.cc~) \
	  $(objs:.o=.o~) \
	  Makefile~

CCDEFS	= $(CCDEFS) -DZIAFS_DEBUG_ERROR -DZIAFS_LOG_ERROR

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
