##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 18:43:53 2005 texane
## Last update Thu Nov 24 15:12:54 2005 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= resource.lo
srcs	= src$(SLASH)main.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)dataman$(SLASH)resource.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)dataman$(SLASH)resource_cgi.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)dataman$(SLASH)resource_file.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)dataman$(SLASH)resource_report.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)dataman$(SLASH)buffer.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)http$(SLASH)uri.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)dataman$(SLASH)stringmanager.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)http$(SLASH)msgdata.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)dataman$(SLASH)cstring_helper.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)socket_in.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc \
	  $(PROJ_ROOT)$(SLASH)src$(SLASH)sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc


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
