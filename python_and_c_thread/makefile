CC			= clang
ARMCC 		= arm-linux-gnueabihf-g++

CCFLAGS		= -ansi -pedantic-errors -Wall -Wextra -g -pthread -fPIC -shared
ARMCCFLAGS	= -ansi -pedantic-errors -Wall -Wextra -g -pthread

HPATH		= -iquote ./

DEP_HEADERS	= 
DEP_SRC		= 

SRC_HEADERS	= $(wildcard *.h)
MAIN_SRC	= $(wildcard *.c)
SOURCES		= $(MAIN_SRC) $(DEP_SRC)
OBJECTS		= $(SOURCES:.c=.o) $(DEP_SRC:.c=.o)
DEB_EXE		= deb.out
RAS_EXE		= raspi.out
SO_FILE		= $(MAIN_SRC:c=.so)
EXECUTABLES = $(DEB_EXE) $(RAS_EXE) $(SO_FILE)

.PHONY : all clean rebuild obj_files

all: $(SO_FILE)

$(SO_FILE): $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(CC) $(CCFLAGS) $(HPATH) -o $@ $(SOURCES)

$(DEB_EXE): $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(CC) $(CCFLAGS) $(HPATH) -o $@ $(SOURCES)

$(RAS_EXE): $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(ARMCC) $(ARMCCFLAGS) $(HPATH) -o $@ $(SOURCES)

obj_files: $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(CC) $(CCFLAGS) $(HPATH) -c $(SOURCES)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(HPATH) $(CCFLAGS) -c $< -o $@ 

clean:
	rm -rf $(EXECUTABLES) $(OBJECTS)


rebuild: clean all
