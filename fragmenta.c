#include <stdlib.h>
#include <string.h>


char **fragmenta(const char *cadena){
    char *copia, *palabra, **arg;
    int num_palabras, i;

    num_palabras = 1;    // recuérdese que se necesita una posición adicional para al macenar el NULL del delimitador final
    i = 0;

    copia = (char *) malloc( (strlen(cadena)+1) * sizeof(char)); // Reservo memoria para copiarme la cadena, pues estoy trabajando con una cadena que es una constante
    strcpy(copia, cadena);

    palabra = strtok(copia, " ");
    while (palabra != NULL) { // Contar el número de palabras
        if (strlen(palabra) > 0) num_palabras++;
        palabra = strtok(NULL, " ");
    }

    arg = (char **) malloc( num_palabras * sizeof(char *) );

    palabra = strtok((char *) cadena, " ");
    while (palabra != NULL) {
        if (strlen(palabra) > 0) {
            arg[i] = (char *) malloc((strlen(palabra)+1) * sizeof(char));
            strcpy(arg[i], palabra);
            i++;
        }
        palabra = strtok(NULL, " ");
    }

    return arg;
}

void borrarg(char **arg){
    int i;

    i = 0;
    while(arg[i] != NULL){
        free(arg[i]);
        i++;
    }
    free(arg);
}
