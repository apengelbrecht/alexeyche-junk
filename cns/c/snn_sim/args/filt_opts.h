#ifndef ARG_OPT_FILT_H
#define ARG_OPT_FILT_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    int jobs;
    const char *input_file;
    const char *output_file;
} ArgOptionsFilt;

void usageFilt(void) {
    printf("Usage: \n");
    printf("\t-i - file for input ts\n");
    printf("\t-o - file for output\n");
    printf("\t-? - print this message\n");
    exit(8);
}

ArgOptionsFilt parseFiltOptions(int argc, char **argv) {
    ArgOptionsFilt args;
    args.jobs = 1;
    args.input_file = NULL;
    args.output_file = NULL;
    if(argc == 1) usageFilt();
    while ((argc > 1) && (argv[1][0] == '-')) {
        if(strcmp(argv[1], "-i") == 0) {                
            if(argc == 2) { 
                printf("No options for -i\n");
                usageFilt();
            }
            args.input_file = strdup(argv[2]);
            ++argv; --argc;
        } else
        if(strcmp(argv[1], "-o") == 0) {                
            if(argc == 2) { 
                printf("No options for -o\n");
                usageFilt();
            }
            args.output_file = strdup(argv[2]);
            ++argv; --argc;
        } else
        if(strcmp(argv[1], "-j") == 0) {                
            if(argc == 2) { 
                printf("No options for -j\n");
                usageFilt();
            }
            args.jobs = atoi(argv[2]);
            ++argv; --argc;
        } else 
        {            
            printf("Wrong Argument: %s\n", argv[1]);
            usageFilt();
        }

        ++argv; --argc;
    }
}
#endif
