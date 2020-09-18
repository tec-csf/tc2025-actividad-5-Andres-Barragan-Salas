[![Work in Repl.it](https://classroom.github.com/assets/work-in-replit-14baed9a392b3a25080506f3b7b6d57f295ec2978f6f33ec97e36a161684cbe9.svg)](https://classroom.github.com/online_ide?assignment_repo_id=3130118&assignment_repo_type=AssignmentRepo)
# Actividad *5*: *IPC*  

##### Estudiante:  
*Andrés Barragán Salas* - *A01026567* - *CSF*  

##### Programación Avanzada
*Profesor: Vicente Cubells Nonell*

## Instrucciones
Implemente un programa en C que resuelva el problema siguiente: 

Se quiere crear una red Token Ring formada por N procesos, donde el valor de n se pasa como un argumento en la línea de comando (opción -n).  

* Cada proceso en el anillo se enlaza de forma unidireccional con su antecesor y su sucesor mediante un pipe.  
* El proceso que crea el anillo genera un único carácter ('T') que hará de testigo.  
* Este testigo recorrerá el anillo indefinidamente de la siguiente forma:
  * cada proceso en el anillo espera la recepción del testigo;
  * cuando un proceso recibe el testigo lo conserva durante 5 segundos;
  * una vez transcurridos los 5 segundos, lo envía al siguiente proceso en el anillo.
* Cada proceso debe mostrar:
  * Al recibir el testigo, el mensaje: “—-> Soy el proceso con PID <su_pid>  y recibí el testigo <testigo>, el cual tendré por 5 segundos”
  * Al pasar el testigo, el mensaje:  “<—- Soy el proceso con PID <su_pid>  y acabo de enviar el testigo <testigo>”
  * Donde:
    * <su_pid> debe sustituirse por el PID del proceso
    * <testigo> debe sustituirse por el caracter que recibe el proceso mediante el pipe. No se puede imprimir directamente el caracter 'T'. 
