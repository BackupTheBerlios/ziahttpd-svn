##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 18:43:53 2005 texane
## Last update Mon Dec 05 20:46:13 2005 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix	

# Give your own variables here
name	= ziad.x
srcs	= main.cc \
	  server$(SLASH)core.cc \
	  server$(SLASH)modman.cc \
	  server$(SLASH)module.cc \
	  server$(SLASH)service.cc \
	  server$(SLASH)ioselect.cc \
	  server$(SLASH)exception.cc \
	  http$(SLASH)session.cc \
	  http$(SLASH)session_manager.cc \
	  http$(SLASH)msgdata.cc \
	  http$(SLASH)uri.cc \
	  dataman$(SLASH)buffer.cc \
	  dataman$(SLASH)inet_helper.cc \
	  dataman$(SLASH)cstring_helper.cc \
	  dataman$(SLASH)stringmanager.cc \
	  dataman$(SLASH)ConfManager.cc \
	  dataman$(SLASH)tinystr.cc \
	  dataman$(SLASH)tinyxml.cc \
	  dataman$(SLASH)tinyxmlerror.cc \
	  dataman$(SLASH)tinyxmlparser.cc \
	  dataman$(SLASH)resource.cc \
	  dataman$(SLASH)resource_cgi.cc \
	  dataman$(SLASH)resource_file.cc \
	  dataman$(SLASH)resource_report.cc \
	  dataman$(SLASH)resource_bodydata.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)mutex.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)socket_in.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)process.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)thread.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)shared_object.cc

objs	= $(srcs:.cc=.o)
trash	= $(srcs:.cc=.cc~) \
	  $(objs:.o=.o~) \
	  Makefile~ \
	  $(name).core

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
