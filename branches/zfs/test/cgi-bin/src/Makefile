##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Tue Feb 14 01:04:44 2006 texane
## Last update Wed Feb 22 11:55:06 2006 texane
##

# ! Always include this file
# !include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= cgi_echo.exe
#	  cgi_loop.exe
#	  cgi_echo.x
#	  cgi_simple.x
srcs	= src$(SLASH)main_echo.cc
# src$(SLASH)main_loop.cc
#	src$(SLASH)main_simple.cc
objs	= $(srcs:.cc=.o)
trash	= $(srcs:.cc=.cc~) \
	  $(objs:.o=.o~) \
	  Makefile~

# CCDEFS	= $(CCDEFS) -DZIAFS_DEBUG_MSG
# CCDEFS	= $(CCDEFS) -DZIAFS_DUMP_OBJECTS -DZIAFS_PRINT_OBJECTS -DZIAFS_DEBUG_MSG
# CCDEFS	= $(CCDEFS) -DZIAFS_DEBUG_ERROR

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
