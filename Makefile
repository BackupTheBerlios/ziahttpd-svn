##
## Makefile<3> for  in 
## 
## Made by texane
## Login   <texane@gmail.com>
## 
## Started on  Wed Oct 05 09:28:50 2005 texane
## Last update Sun Oct 09 16:36:55 2005 texane
##

include <$(PROJ_ROOT)/build/$(PROJ_ENV).mk>


force	:
all	:
	$(ECHO) ca roule
#	@for /D %d in ($(PROJ_CORE)) do $(CD) "%d" && $(MAKE) all
clean	: force
fclean	: clean
re	: fclean all
force	:

