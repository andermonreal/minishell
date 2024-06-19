fragmenta: fragmenta.c
	gcc -c fragmenta.c

minishell: minishell.c fragmenta.o
	gcc -o minishell minishell.c fragmenta.o

fragmenta_test: fragmenta_test.c fragmenta.o
	gcc -o fragmenta_test fragmenta_test.c fragmenta.o

all: fragmenta minishell
