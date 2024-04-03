#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "myStructs.h"

int main() {
    //por o nome dos parques na struct apenas um ponteiro a apontar para um srting que é o nome
    //criar funçoes aloca e liberta memoria de coisas
    //usar linked lists para os lugares
    Main *control_struct = malloc(sizeof(Main));
    init(control_struct);

    while (TRUE) {

        fgets(control_struct->command, COMMAND_DIM, stdin); 
        
        switch (control_struct->command[0]){
            case 'q':
                close(control_struct);
                return 0;
            case 'p':
                command_p(control_struct);
                break;
            case 'e':
                command_e(control_struct);    
                break;
            case 's':
                command_s(control_struct);
                break;
            case 'v':
                command_v(control_struct);
                break;
            case 'f':
                command_f(control_struct);
                break;
            case 'r':
                command_r(control_struct);
                break;
            case 'u':
                command_u(control_struct);
                break;
        }
    }
    return 0;
}