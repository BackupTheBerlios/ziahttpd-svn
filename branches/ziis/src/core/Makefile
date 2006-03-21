##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Tue Feb 14 01:04:44 2006 texane
## Last update Tue Mar 21 11:45:51 2006 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= zfs.x
srcs	= main.cc			\
	  core.cc			\
	  net_server.cc			\
	  thr_pool.cc			\
	  thr_io.cc			\
	  thr_system.cc			\
	  thr_server.cc			\
	  resource_manager.cc		\
	  resource_handle.cc		\
	  resource_file.cc		\
	  resource_byfly.cc		\
	  resource_process.cc		\
	  resource_fake.cc		\
	  config.cc			\
	  buffer.cc			\
	  http.cc			\
	  uri.cc			\
	  utils_line.cc			\
	  zia_stringmanager.cc		\
	  ziafs_request_header_validation.cc	\
	  http_response.cc		\
	  http_data.cc			\
	  error_code.cc			\
	  error.cc			\
	  ..$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinystr.cc \
	  ..$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinyxmlerror.cc \
	  ..$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinyxmlparser.cc \
	  ..$(SLASH)lib$(SLASH)src$(SLASH)tinyxml$(SLASH)tinyxml.cc \
	  ..$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)error.cc \
	  ..$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)file.cc \
	  ..$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)insock.cc \
	  ..$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)module.cc \
	  ..$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)time.cc \
	  ..$(SLASH)sys$(SLASH)$(PROJ_SYSAPI)$(SLASH)process.cc
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
	$(CP) $(name) $(PROJ_ROOT)
clean	: force
	$(ECHO) deleting $(objs)
	$(RM) $(objs)
	$(RM) $(trash)
fclean	: clean
	$(RM) $(name)
re	: clean all
