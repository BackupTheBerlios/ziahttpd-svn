##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Tue Feb 14 01:04:44 2006 texane
## Last update Thu Feb 23 01:54:38 2006 texane
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
	  src$(SLASH)resource_manager.cc	\
	  src$(SLASH)resource_handle.cc		\
	  src$(SLASH)resource_file.cc		\
	  src$(SLASH)resource_byfly.cc		\
	  src$(SLASH)resource_process.cc	\
	  src$(SLASH)resource_fake.cc		\
	  src$(SLASH)config.cc			\
	  src$(SLASH)buffer.cc			\
	  src$(SLASH)http.cc			\
	  src$(SLASH)uri.cc			\
	  src$(SLASH)utils_line.cc		\
	  src$(SLASH)zia_stringmanager.cc	\
	  src$(SLASH)ziafs_request_header_validation.cc	\
	  src$(SLASH)http_response.cc		\
	  src$(SLASH)http_data.cc		\
	  src$(SLASH)error_code.cc		\
	  src$(SLASH)error.cc			\
	  src$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinystr.cc		\
	  src$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinyxmlerror.cc	\
	  src$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinyxmlparser.cc	\
	  src$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinyxml.cc		\
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
