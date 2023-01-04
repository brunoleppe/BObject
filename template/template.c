#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <argp.h>
#include <stdbool.h>
#include "helper_string.h"
#include "helper_file.h"

#define STR_SIZE        1024


const char *argp_program_version = "BObject Template Generator 1.0";
const char *argp_program_bug_address = "<your@email.address>";
static char doc[] = "Generate file templates for BObject library. Provide the ObjectName "
    "to inherit form base (BObject). Provide ParentName to inherit form specified parent.\n"
    "Implement multiple interfaces providing -i InterfaceName1 -i InterfaceName2 ...";
static char args_doc[] = "[ObjectName] [OPTIONAL ParentName]";
static struct argp_option options[] = { 
    { "derivable", 'd', 0, 0, "Define derivable class. Final by default."},
    { "private", 'p', 0, 0, "Declare class with private fields. No private fields by default."},
    { "implements", 'i', "InterfaceName", 0, "Interface name in camel case."},
    { "interface", 'I', 0, 0, "Generate template for Interfaces"},
    { 0 } 
};

struct arguments {
    bool private;
    bool derivable;
    bool implements;
    bool isInterface;
    char *args[3];
    char **interfaces;
    int interface_count;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'I': arguments->isInterface = true; break;
    case 'd': arguments->derivable = true; break;
    case 'p': arguments->private = true; break;
    case 'i': 
        arguments->implements = true; 
        arguments->interfaces[arguments->interface_count++] = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 3)
        {
            free(arguments->interfaces);
            argp_usage(state);
        }
        arguments->args[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1)
	    {
	        free(arguments->interfaces);
            argp_usage (state);
        }
        break;
    default:
      return ARGP_ERR_UNKNOWN;
    }   
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };


int main(int argc, char *argv[]){

    char namespace[STR_SIZE]="";
    char type[STR_SIZE]="";
    char parent[STR_SIZE]="";

    struct arguments arguments;
    arguments.derivable = false;
    arguments.implements = false;
    arguments.private = false;
    arguments.isInterface = false;
    arguments.interfaces = malloc(argc * sizeof(char*));
    arguments.interface_count = 0;
    arguments.args[1] = "BObject"; //default parent


    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    printf("Generating files for %s with parent %s\n",arguments.args[0],arguments.args[1]);

    str_separate(arguments.args[0],namespace,type);
    str_tolower(namespace);
    str_tolower(type);

    strcpy(parent,arguments.args[1]);

    if(arguments.isInterface){
        printf("  Generating interface header file\n");
        interface_header(namespace,type,parent);
        printf("  Generating interface source file\n");
        interface_source(namespace,type,parent);
        goto DONE;
    }
    

    printf("  Generating header file\n");
    file_header(
        namespace,
        type,
        parent,
        arguments.derivable
    );
    printf("  Generating source file\n");
    file_source(
        namespace,
        type,
        parent,
        arguments.private,
        arguments.implements,
        arguments.interfaces,
        arguments.interface_count
    );

DONE:
    printf("Done!\n");
    free(arguments.interfaces);

    return 0;
}