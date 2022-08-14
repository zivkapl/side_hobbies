CC			= clang
ARMCC 		= arm-linux-gnueabihf-g++

CCFLAGS		= -ansi -pedantic-errors -Wall -Wextra -g -pthread
ARMCCFLAGS	= -ansi -pedantic-errors -Wall -Wextra -g -pthread

DOUBLY_LINKED_LIST_HDR	= ./priority_queue/doubly_linked_list/dl_list.h
DOUBLY_LINKED_LIST_SRC	= ./priority_queue/doubly_linked_list/dl_list.c

SORTED_DL_LIST_HDR		= ./priority_queue/sorted_linked_list/sorted_dl_list.h
SORTED_DL_LIST_SRC		= ./priority_queue/sorted_linked_list/sorted_dl_list.c

PRIORITY_QUEUE_HDR	= ./priority_queue/p_queue.h
PRIORITY_QUEUE_SRC	= ./priority_queue/p_queue.c

HPATH	 = -iquote ./priority_queue/ \
			-iquote ./priority_queue/doubly_linked_list \
			-iquote ./priority_queue/sorted_linked_list

DEP_HEADERS	= $(DOUBLY_LINKED_LIST_HDR) $(SORTED_DL_LIST_HDR) $(PRIORITY_QUEUE_HDR)
DEP_SRC		= $(DOUBLY_LINKED_LIST_SRC) $(SORTED_DL_LIST_SRC) $(PRIORITY_QUEUE_SRC)


SRC_HEADERS	= $(wildcard *.h)
MAIN_SRC	= $(wildcard *.c)
SOURCES		= $(MAIN_SRC) $(DEP_SRC)
OBJECTS		= $(notdir / $(SOURCES:.c=.o)) $(notdir / $(DEP_SRC:.c=.o))
DEB_EXE		= deb.out
RAS_EXE		= raspi.out
SO_FILE		= $(MAIN_SRC:.c=.so)
EXECUTABLES = $(DEB_EXE) $(RAS_EXE)
OUT_FILES	= $(SO_FILE) $(EXECUTABLES) $(OBJECTS)

.PHONY : all clean rebuild obj

all: $(SO_FILE)

$(SO_FILE): $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(CC) $(CCFLAGS) -fPIC -shared $(HPATH) -o $@ $(SOURCES)

$(DEB_EXE): $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(CC) $(CCFLAGS) $(HPATH) -o $@ $(SOURCES)

$(RAS_EXE): $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(ARMCC) $(ARMCCFLAGS) $(HPATH) -o $@ $(SOURCES)

obj: $(SOURCES) $(SRC_HEADERS) $(DEP_HEADERS)
	$(CC) $(CCFLAGS) $(HPATH) -c $(SOURCES)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(HPATH) $(CCFLAGS) -c $< -o $@ 

clean:
	rm -rf $(OUT_FILES)

rebuild: clean all
