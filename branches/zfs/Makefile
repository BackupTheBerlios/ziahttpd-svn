##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Tue Feb 14 01:04:44 2006 texane
## Last update Tue Feb 14 22:57:01 2006 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= zfs.x
srcs	= src$(SLASH)main.cc			\
	  src$(SLASH)core.cc			\
	  src$(SLASH)net_server.cc		\
	  src$(SLASH)thr_pool.cc		\
	  src$(SLASH)thr_io.cc			\
	  src$(SLASH)thr_system.cc		\
	  src$(SLASH)thr_server.cc		\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)insock.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)module.cc	\
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)time.cc
objs	= $(srcs:.cc=.o)
trash	= $(srcs:.cc=.cc~) \
	  $(objs:.o=.o~) \
	  Makefile~

CCDEFS	= $(CCDEFS) -DZIAFS_DEBUG_MSG
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
