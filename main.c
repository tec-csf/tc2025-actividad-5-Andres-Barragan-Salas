/*  Andrés Barragán Salas
    A01026567
    Actividad 5: IPC */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* Declaración de los métodos */
void print_help();
void leer(int * fd);
void escribir(int * fd, char c);
void cycleProcess(int * originTube, int * previousTube, int remainingProcesses, char witness);

/* Ejecución principal */
int main(int argc, char * const * argv) {
    char * input = NULL;
    int processAmount = 0;
    int index;
    int opt;
    int help = 0;
    int estado;
    
    opterr = 0;
    
    //  Flag para conocer si una opción fue elegida
    int optionSelected = 0;
    //  Se obtienenn los argumentos al ejecutar el programa
    while ((opt = getopt (argc, argv, "n:h")) != -1) {
        optionSelected = 1; 
        switch (opt) {
            case 'n':
                input = optarg;
                if (atoi(input))
                    processAmount = atoi(input);
                else
                    printf("Opción -%c requiere un numero entero como argumento\n", opt);
                break;
            case 'h':
                help = 1;
                break;
            case '?':
                if (optopt == 'n')
                    fprintf (stderr, "Opción -%c requiere un numero entero como argumento\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
                else
                    fprintf (stderr, "Opción desconocida '\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }
    
    // Señalizar todos los opciones ingresadas que no sol válidas
    for (index = optind; index < argc; index++)
        printf ("El argumento no es una opción válida %s\n", argv[index]);
    
    // Se imprime el menú de ayuda con la opción "-h" o a falta de opciones
    if (optionSelected == 0 || help == 1)
        print_help();

    // Si no se encontro un numero de procesos hijos a obtener salir del programa
    if (processAmount == 0)
        return 1; 

    // Crear el pipline que antecede al primer proceso
    int originTube[2];
    pipe(originTube);

    // Escribir el testigo en el ppipeline original y generar procesos recursivamente de manera cíclica 
    char witness = 'T';
    write(originTube[1], &witness, sizeof(char));
    cycleProcess(originTube, originTube, processAmount, witness);

    return 0;
}

/* Imprime un menú para conocer los argumentos que puede utilizar el programa al ser ejecutado */
void print_help() {
    printf("\nUse: ./a.out [-n integer] [-h]\n");
    printf("\nOpciones:\n");
    printf("-n : Crear una cantidad dada de procesos a unir cíclicamente\n-h : Ayuda\n\n");
}

/*  Metodo para leer un caractér de un pipeline dado y esperar 5 segundos después
    @param fd: pipe desde el cual se desea leer
*/
void leer(int * fd) {
    char c;
    
    close(fd[1]);
    read(fd[0], &c, sizeof(char));
    printf("—-> Soy el proceso con PID %d y recibí el testigo %c, el cual tendré por 5 segundos\n", getpid(), c);
    sleep(5);
}

/*  Metodo para escribir un caracter dado en un pipeline dado
    @param fd: pipe en el cual se desea escribir
*/
void escribir(int * fd, char c) {
    printf("<—- Soy el proceso con PID %d y acabo de enviar el testigo %c\n", getpid(), c);
    write(fd[1], &c, sizeof(char));
}

/*  Metodo para generar un ciclo recursivamente entre procesos hijos creados y un proceso original original, iterando sobre ellos con base en la levtura de los pipelines que los unen.
    @param originTube: pipeline que antecede al proceso original
    @param previousTube: piplene que antecede al siguiente proceso a crear
    @param remainingProcesses: numero de procesos que falta por agregar al ciclo
    @param witness: character que ha de ser transferido a través de los piplines
*/
void cycleProcess(int * originTube, int * previousTube, int remainingProcesses, char witness) {
    if (remainingProcesses == 1) {
        // Se han terminado de crear los procesos hijos, ligar el último proceso al original
        while ( 1 ) {
            leer(previousTube);
            escribir(originTube, witness);
        }
    } else {
        // Crear un pipeline que ligará al procesos actual con el siguiente en el ciclo
        int nextTube[2];
        pipe(nextTube);
        // Crear un nuevo proceso hijo
        pid_t pid;
        pid = fork();

        if (pid == 1) {
            // Señalizar un error al crear un proceeso hijo, dejar de crear processos 
            printf("Error creando procesos hijo.\n");
            exit(1);
        }
        else if (pid == 0) {
            // Recursividad para formar un ciclo entre los procesos
            cycleProcess(originTube, nextTube, remainingProcesses - 1, witness);
        }
        else {
            // Leer desde el pipline anterior y escribir en el siguiente indefinidamente
            while ( 1 ) {
                leer(previousTube);
                escribir(nextTube, witness);
            }
        }
    }
}