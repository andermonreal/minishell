#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "fragmenta.h"

#define AZUL "\x1b[34m"
#define VERDE "\x1b[32m"
#define ROJO "\x1b[31m"
#define AMARILLO "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define CIAN "\x1b[36m"

pid_t iden;
int status;

void salida (int signum) {
	kill (getppid(), SIGINT);
	wait(&status);
	printf (VERDE"\n\n\t[!] Saliendo de la minishell...\n");
	exit (1);
}

int main () {
	printf("\n");
	char buffer[50];
	int i = 0;
	char **fragmentado;
	signal (SIGINT, salida);

	while (1) {
		printf(AMARILLO"minishell\\> "CIAN);
		 // Reviso si el buffer esta vacio o no (TIENE QUE ESTAR VACIO IMPORTANTISIMO!!!) con fgetc
		scanf("%[^\n]", buffer);
		getchar(); // si no, se imprime en bucle

		// Para salir del programa
		if (strcmp(buffer, "exit") == 0) {
			salida(-3);
		}

		// ERROR EN LA CREACION DEL PRIMER HIJO
		if ((iden = fork()) == -1) {
			perror("\n\nERROR EN LA CREACION DEL PRIMER HIJO\n\n");
			exit(-1);
		}
		else if (iden == 0) { // HIJO

			fragmentado = fragmenta(buffer);
			i = 0;
			while (fragmentado[i] != NULL) {
				//printf("Estoy trabajando con el string %s\n", fragmentado[i]);
				// Es necesario hacer un strcmp para comparar strings, en este caso fragmentado[i] con un string que creamos mediante las dos comillas

				if (strcmp(fragmentado[i], "|") == 0) {
					pid_t iden2;
					int tuberia[2];

					//ERROR EN LA CREACION DE LA TUBERIA
					if (pipe(tuberia) == -1) {
						perror ("\n\nERROR EN LA CREACION DE LA TUBERIA\n\n");
						exit(-1);
					}

					// ERROR EN LA CREACION DEL SEGUNDO HIJO
					if ((iden2 = fork()) == -1) {
						perror ("\n\nERROR EN LA CREACION DEL SEGUNOD HIJO\n\n");
						exit(-1);
					}

					fragmentado[i] = NULL;
	                                if (iden2 == 0) { // HIJO DEL HIJO
						close(tuberia[0]);
						dup2(tuberia[1], STDOUT_FILENO);
						execvp(fragmentado[0], fragmentado);

					}
					else { // HIJO INTERMEDIO
						close (tuberia[1]);
						wait(&status);
						dup2(tuberia[0], STDIN_FILENO);
						execvp(fragmentado[i+1], &fragmentado[i+1]);
					}

				// Escribir en un fichero sin sobreescribirlo (FUNCIONA)
				} else if (strcmp(fragmentado[i], ">>") == 0) {
					fragmentado[i] = NULL;
					int fichero;
					fichero = open(fragmentado[i+1], O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
					dup2(fichero, STDOUT_FILENO);
					close (fichero);
					i++; // te ahorras una vuelta del while

				// Escribir en un fichero sobreescribiendolo

				}else if (strcmp(fragmentado[i], ">") == 0) {
					fragmentado[i] = NULL;
					int fichero;
					fichero = open(fragmentado[i+1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
					dup2(fichero, STDOUT_FILENO);
					close (fichero);
					i++; // te ahorras una vuelta del while

				// realiza el primer comando con el fichero ya existente
				} else if (strcmp(fragmentado[i], "<") == 0) {
					fragmentado[i] = NULL;
					int fichero;
					fichero = open(fragmentado[i+1], O_RDONLY, S_IRUSR | S_IWUSR);
					dup2(fichero, STDIN_FILENO);
					close(fichero);
					i++; // te ahorras una vuelta del while
				}
				i++;
			}
			execvp(fragmentado[0], fragmentado); // v porque no tengo ni idea de cuantos argumentos hay en el buffer y, el p, busca la ruta absoluta en el PATH
			// Si supiera el tamaño seria "l" en vez de "v"
			printf(ROJO"\n[?] No existe el comando introducido\n\n");
			exit(-1);

		} else { // PADRE
			wait(&status);
			printf("\n");
		}
	}
}
