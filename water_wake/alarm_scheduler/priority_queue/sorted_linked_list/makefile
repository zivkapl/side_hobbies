CC       = gcc
CFLAGS   = -ansi -pedantic-errors -Wall -Wextra -g 
SRCFILES = sorted_dl_list.c sorted_dl_list_main.c ../doubly_linked_list/dl_list.c
HPATH	 = -iquote ../doubly_linked_list

run_tests: $(SRCFILES)
	$(CC) $(CFLAGS) $(HPATH) $(SRCFILES) 

memory: $(SRCFILES)
	$(CC) $(CFLAGS) -fsanitize=address $(HPATH) $(SRCFILES) 

show_pre: $(SRCFILES)
	$(CC) -DNDEBUG -E $(HPATH) $(SRCFILES)

build: $(SRCFILES)
	$(CC) -DNDEBUG $(HPATH) $(SRCFILES)

clean:
	rm a.out