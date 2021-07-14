shell: shell.o built_in.o direction_and_pipe.o parse.o external.o
	gcc -o shell shell.o built_in.o direction_and_pipe.o parse.o external.o -lreadline
shell.o: shell.c built_in.h direction_and_pipe.h parse.h external.h
	gcc -c shell.c
external.o: external.c external.h
	gcc -c external.c
built_in.o: built_in.c built_in.h
	gcc -c built_in.c
direction_and_pipe.o: direction_and_pipe.c direction_and_pipe.h parse.h
	gcc -c direction_and_pipe.c
parse.o: parse.c parse.h
	gcc -c parse.c
clean:
	rm -rm *.o shell
