#make: shell.c
#	gcc -g -Wall -pthread shell.c -lpthread -o ./shell

make: shell.c
	gcc shell.c -o shell

clean: ./shell
	rm -f ./shell

test: test.c
	gcc test.c -o test

cleanTest: test
	rm -f ./test