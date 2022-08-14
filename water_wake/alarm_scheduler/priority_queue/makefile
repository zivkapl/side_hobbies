CC       = gcc
CFLAGS   = -ansi -pedantic-errors -Wall -Wextra -g 
SRCFILES = p_queue.c p_queue_main.c ./sorted_linked_list/sorted_dl_list.c ./doubly_linked_list/dl_list.c
HPATH	 = -iquote ../sorted_linked_list -iquote ../doubly_linked_list

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