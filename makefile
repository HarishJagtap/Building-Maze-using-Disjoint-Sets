IDIR = src/include
GLUTDIR = glut
ODIR = bin
RDIR = result
CDIR = src

DEPS = $(wildcard $(IDIR)/*.h) $(wildcard $(GLUTDIR)/*.h) 
CFILE = $(wildcard $(CDIR)/*.c)
_OBJ = $(CFILE:.c=.obj)
OBJ = $(_OBJ:$(CDIR)/%=$(ODIR)/%)

ifeq ($(OS),Windows_NT)
CC = cl
CFLAG = /c
OFLAG = /Fo
IFLAG = /I $(IDIR) /I $(GLUTDIR)
ONAME = /Fe
DEBUG = /Zi
DLIB = $(GLUTDIR)/glut32.lib
MISC = /nologo
$(info Windows OS detected)
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CC = gcc
CFLAG = -c
OFLAG = -o
IFLAG = -I $(IDIR)
ONAME = -o
DEBUG = -g
DLIB = -lglut -lGL -lGLU
MISC =
$(info Linux OS detected)
else
$(error Program does not support this operating system)
endif
endif

$(ODIR)/mazerun : directories $(OBJ)
	$(CC) $(ONAME)$@ $(OBJ) $(IFLAG) $(DLIB) $(MISC)

directories :
	mkdir -p $(ODIR) $(RDIR)

$(ODIR)/%.obj : $(CDIR)/%.c $(DEPS)
	$(CC) $(CFLAG) $< $(OFLAG)$@ $(IFLAG) $(MISC)

.PHONY: clean directories

clean:
	rm -f $(ODIR)/*.obj
	rm -f $(ODIR)/*.exe