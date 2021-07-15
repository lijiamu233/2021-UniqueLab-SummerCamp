shell: ./main.c ./cmds/cmd.c ./cmds/builtin.c
	gcc ./main.c ./cmds/cmd.c ./cmds/builtin.c -o ./shell -lreadline

ls:  ./builtin/ls.c ./common.h
	gcc ./builtin/ls.c -o ./compile/ls

touch: ./builtin/touch.c ./common.h
	gcc ./builtin/touch.c -o ./compile/touch

mkdir: ./builtin/mkdir.c ./common.h
	gcc ./builtin/mkdir.c -o ./compile/mkdir

rm: ./builtin/rm.c ./common.h
	gcc ./builtin/rm.c -o ./compile/rm

rmdir: ./builtin/rmdir.c ./common.h
	gcc ./builtin/rmdir.c -o ./compile/rmdir