##
## Makefile<4> for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 20:39:38 2005 texane
## Last update Sat Feb 11 17:51:04 2006 
##

# ! Always include this file
# !include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= ziafs.x
srcs	= src$(SLASH)core$(SLASH)main.cc \
	  src$(SLASH)core$(SLASH)server.cc \
	  src$(SLASH)core$(SLASH)server_sendfile.cc \
	  src$(SLASH)core$(SLASH)config.cc \
	  src$(SLASH)core$(SLASH)buffer.cc \
	  src$(SLASH)core$(SLASH)error.cc \
	  src$(SLASH)core$(SLASH)session.cc \
	  src$(SLASH)core$(SLASH)iores_manager.cc \
	  src$(SLASH)core$(SLASH)iores_file.cc \
	  src$(SLASH)core$(SLASH)iores_insock.cc \
	  src$(SLASH)core$(SLASH)utils_line.cc \
	  src$(SLASH)core$(SLASH)mod_manager.cc \
	  src$(SLASH)core$(SLASH)http.cc \
	  src$(SLASH)core$(SLASH)zia_stringmanager.cc \
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc \
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)time.cc \
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc \
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)insock.cc \
	  src$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)module.cc \
	  src$(SLASH)lib$(SLASH)tinyxml$(SLASH)tinyxmlparser.cc \
	  src$(SLASH)lib$(SLASH)tinyxml$(SLASH)tinyxmlerror.cc \
	  src$(SLASH)lib$(SLASH)tinyxml$(SLASH)tinyxml.cc \
	  src$(SLASH)lib$(SLASH)tinyxml$(SLASH)tinystr.cc
# 	  src$(SLASH)core$(SLASH)iores_proc.cc
# 	  src$(SLASH)core$(SLASH)iores_mem.cc
# 	  src$(SLASH)core$(SLASH)iores_insock.cc
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
