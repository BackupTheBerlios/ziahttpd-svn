##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 18:43:53 2005 texane
## Last update Wed Oct 26 20:58:06 2005 
##

# ! Always include this file
# !include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix	

# Give your own variables here
name	= ziad.x
srcs	= core$(SLASH)main.cc \
	  core$(SLASH)core.cc \
	  core$(SLASH)exception.cc \
	  core$(SLASH)channel.cc \
	  core$(SLASH)session.cc \
	  core$(SLASH)session_helper.cc \
	  core$(SLASH)dataman.cc \
	  core$(SLASH)uri.cc \
	  core$(SLASH)resource.cc \
	  core$(SLASH)buffer.cc \
	  core$(SLASH)cstring_helper.cc \
	  core$(SLASH)session_body.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)process.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)thread.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)mutex.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)socket_in.cc \
	  sysapi$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc \
	  message$(SLASH)header.cc \
	  message$(SLASH)message.cc \
	  message$(SLASH)statusline.cc \
	  message$(SLASH)httpstringmanager.cc \
	  message$(SLASH)response.cc \
	  message$(SLASH)error_code.cc \
	  stringmanager$(SLASH)stringmanager.cc \
	  tinyxml$(SLASH)tinystr.cc \
	  tinyxml$(SLASH)tinyxml.cc \
	  tinyxml$(SLASH)tinyxmlerror.cc \
	  tinyxml$(SLASH)tinyxmlparser.cc \
	  confmanager$(SLASH)ConfManager.cc


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
