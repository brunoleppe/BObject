#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <argp.h>
#include <stdbool.h>
#include "helper_string.h"
#include "helper_file.h"

#define STR_SIZE        1024


const char *argp_program_version = "programname programversion";
const char *argp_program_bug_address = "<your@email.address>";
static char doc[] = "Your program description.";
static char args_doc[] = "[NAMESPACE] [TYPE] [PARENT]";
static struct argp_option options[] = { 
    { "derivable", 'd', 0, 0, "Define derivable class. Final by default"},
    { "private", 'p', 0, 0, "Declare class with private fields. No private fields by default"},
    { "implements", 'i', "NAMESPACE", 0, "Compare case insensitive instead of case sensitive."},
    { 0 } 
};

struct arguments {
    bool private;
    bool derivable;
    bool implements;
    char *args[3];
    char **interfaces;
    int interface_count;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'd': arguments->derivable = true; break;
    case 'p': arguments->private = true; break;
    case 'i': 
        arguments->implements = true; 
        arguments->interfaces[arguments->interface_count++] = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 4)
        {
            argp_usage(state);
            free(arguments->interfaces);
        }
        arguments->args[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 3)
	    {
	        argp_usage (state);
            free(arguments->interfaces);
	    }
        break;
    default:
      return ARGP_ERR_UNKNOWN;
    }   
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };


int main(int argc, char *argv[]){

    char namespace[STR_SIZE];
    char type[STR_SIZE];
    char parent[STR_SIZE];

    struct arguments arguments;
    arguments.derivable = false;
    arguments.implements = false;
    arguments.private = false;
    arguments.interfaces = malloc(argc * sizeof(char*));
    arguments.interface_count = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    puts(arguments.args[0]);
    puts(arguments.args[1]);
    puts(arguments.args[2]);


    strcpy(namespace,arguments.args[0]);
    strcpy(type,arguments.args[1]);
    strcpy(parent,arguments.args[2]);

    file_header(
        namespace,
        type,
        parent,
        arguments.derivable
    );
    file_source(
        namespace,
        type,
        parent,
        arguments.private,
        arguments.implements,
        arguments.interfaces,
        arguments.interface_count
    );

    free(arguments.interfaces);

    return 0;
}