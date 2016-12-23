# To crosscompile for Dreamcast:
#   /path/to/tools/bin/nbmake-dreamcast 
CFLAGS+= -g -Wall
PROG=	mmem
SRCS=	mmem.c 
MAN=	mmem.1

.include <bsd.prog.mk>
