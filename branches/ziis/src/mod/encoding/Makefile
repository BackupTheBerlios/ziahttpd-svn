##
## Makefile for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 26 18:43:53 2005 texane
## Last update Mon Apr 03 19:03:15 2006 texane
##

# ! Always include this file
!include "$(PROJ_ROOT)/build/$(PROJ_ENV).mk" #windows
# include $(PROJ_ROOT)/build/$(PROJ_ENV).mk #unix

# Give your own variables here
name	= mod_encoding.lo
srcs	= src$(SLASH)main.cc		\
	  src$(SLASH)modimpl.cc		\
	  src$(SLASH)compimpl.cc	\
	  src$(SLASH)compimpl_zlib.cc	\
	  src$(SLASH)compimpl_gzip.cc	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)adler32.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)compress.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)crc32.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)deflate.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)gzio.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)infback.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)inffast.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)inflate.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)inftrees.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)trees.c	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)zutil.c
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)bits.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)crypt.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)deflate.c	\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)getopt.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)gzip.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)inflate.c	\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)lzw.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)trees.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unlzw.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unlzh.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unpack.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unzip.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)utils.c		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)zip.c

objs	= src$(SLASH)main.o \
	  src$(SLASH)modimpl.o \
	  src$(SLASH)compimpl.o \
	  src$(SLASH)compimpl_zlib.o \
	  src$(SLASH)compimpl_gzip.o \
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)adler32.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)compress.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)crc32.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)deflate.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)gzio.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)infback.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)inffast.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)inflate.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)inftrees.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)trees.o	\
	  src$(SLASH)lib$(SLASH)zlib-1.2.3$(SLASH)zutil.o	\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)bits.o
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)crypt.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)deflate.o	\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)getopt.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)gzip.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)inflate.o	\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)lzw.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)trees.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unlzw.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unlzh.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unpack.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)unzip.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)utils.o		\
# 	  src$(SLASH)lib$(SLASH)gzip124$(SLASH)zip.o

trash	= src$(SLASH)*.cc~ \
	  src$(SLASH)*.o~ \
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
