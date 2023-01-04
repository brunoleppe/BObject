#include "helper_file.h"
#include "helper_string.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void file_header(char *namespace, char *type, char *parent, bool derivable)
{

    char object_name[1024];
    strcpy(object_name,str_first_upper(namespace));
    strcat(object_name,str_first_upper(type));
    char filename_h[1024];
    strcpy(filename_h,str_tolower(namespace));
    strcat(filename_h,str_tolower(type));
    strcat(filename_h,".h");

    FILE *file;
    file = fopen(filename_h,"w");

    fprintf(file,"#ifndef %1$s\n#define %1$s\n\n",str_toupper(str_replace(filename_h,'.','_')));
    
    char aux[1024];
    strcpy(aux,parent);
    fprintf(file,"#include \"%s.h\"\n\n",str_tolower(aux));

    fprintf(file,"#ifdef __cplusplus\nextern \"C\"{\n#endif\n\n\n");

    fprintf(file,"#define %s_TYPE_%s()\t\t",str_toupper(namespace),str_toupper(type));
    fprintf(file,"%s_%s_get_type()\n",str_tolower(namespace),str_tolower(type));

    char *der = "B_DECLARE_FINAL_TYPE";
    if(derivable)
        der = "B_DECLARE_DERIVABLE_TYPE";

    fprintf(file,"%s(%s, %s_%s, %s)\n\n",der,object_name,str_tolower(namespace),str_tolower(type),parent);

    fprintf(file,"struct %s{\n"
        "\t%s parent_instance;\n"
        "\t/*public fields*/\n"
        "};\n\n",object_name,parent);
    
    if(derivable){
        fprintf(file,"struct %sClass{\n"
            "\t%sClass parent_class;\n"
            "\t/*virual methods*/\n"
            "};\n\n",object_name,parent);
    }

    fprintf(file,"%s* %s_%s_new( void );\n",object_name,str_tolower(namespace),str_tolower(type));
    fprintf(file,"/*Class Methods*/\n\n\n");
    fprintf(file,"#ifdef __cplusplus\n}\n#endif\n\n\n");
    fprintf(file,"#endif /*%s*/",filename_h);

    fclose(file);
}
void file_source(char *namespace, char *type, char *parent, bool private, bool implements, char *interfaces[], int interface_count)
{
    char object_name[1024]="";
    strcpy(object_name,str_first_upper(str_tolower(namespace)));
    strcat(object_name,str_first_upper(str_tolower(type)));
    char filename_c[1024]="";
    strcpy(filename_c,str_tolower(namespace));
    strcat(filename_c,str_tolower(type));
    strcat(filename_c,".c");
    char filename_h[1024]="";
    strcpy(filename_h,str_tolower(namespace));
    strcat(filename_h,str_tolower(type));
    strcat(filename_h,".h");


    

    FILE *file;
    file = fopen(filename_c,"w");

    fprintf(file,"#include \"%s\"\n",str_tolower(str_replace(filename_h,'_','.')));
    int i;
    for(i=0;i<interface_count;i++){
        char aux[1024];
        strcpy(aux,interfaces[i]);
        fprintf(file,"#include \"%s.h\"\n",str_tolower(aux));
    }
    fprintf(file,"\n\n\n");
    
    
    if(private){
        fprintf(file,"typedef struct{\n"
            "\t/*private fields*/\n"
            "}%sPrivate;\n\n",object_name);
    }


    char parent_namespace[1024]="";
    char parent_type[1024]="";
    str_separate(parent,parent_namespace,parent_type);

    if(!private && !implements)
        fprintf(file,"B_DEFINE_TYPE(%s, %s_%s, %s_TYPE_%s())\n\n",object_name,str_tolower(namespace),str_tolower(type),
            str_toupper(parent_namespace),str_toupper(parent_type));
    else if(private && !implements)
        fprintf(file,"B_DEFINE_TYPE_WITH_PRIVATE(%s, %s_%s, %s_TYPE_%s())\n\n",object_name,str_tolower(namespace),str_tolower(type),
            str_toupper(parent_namespace),str_toupper(parent_type));
    else{

        char **init = malloc(interface_count*sizeof(char*));
        for(int i=0;i<interface_count;i++){
            init[i] = malloc(1024);
            char inamespace[1024]="";
            char itype[1024]="";
            init[i][0] = 0;
            str_separate(interfaces[i],inamespace,itype);
            sprintf(init[i],"%s_%s_interface_init",str_tolower(inamespace),str_tolower(itype));
            fprintf(file,"static void %s(%sInterface *iface);\n",init[i],interfaces[i]);
        }
        fprintf(file,"\n\n");

        fprintf(file,"B_DEFINE_TYPE_EXTENDED(%s, %s_%s, %s_TYPE_%s(),\n",object_name,str_tolower(namespace),str_tolower(type),
            str_toupper(parent_namespace),str_toupper(parent_type));
        if(private)
            fprintf(file,"\tB_DEFINE_PRIVATE(%s, %s_%s)\n",object_name,str_tolower(namespace),str_tolower(type));
        fprintf(file,"\tB_IMPLEMENT_INTERFACES(\n");
        int i;
        for(i=0;i<interface_count;i++){
            char inamespace[1024]="";
            char itype[1024]="";
            str_separate(interfaces[i],inamespace,itype);
            fprintf(file,"\t\tB_INTERFACE(%s_TYPE_%s(),%s)\n",str_toupper(inamespace),str_toupper(itype),init[i]);
        }
        fprintf(file,"\t)\n)\n\n");

        for(i=0;i<interface_count;i++){
            fprintf(file, "static void %s(%sInterface *iface)\n{\n\t/*Implementation*/\n}\n",init[i],interfaces[i]);
            free(init[i]);
        }
        free(init);
    }

    fprintf(file,"static void %s_%s_class_initialize(%sClass *class)\n{\n\t/*Implementation*/\n}\n\n",str_tolower(namespace),str_tolower(type),object_name);
    fprintf(file,"static void %s_%s_instance_initialize(%s *self)\n{\n\t/*Implementation*/\n}\n\n",str_tolower(namespace),str_tolower(type),object_name);
    fprintf(file,"static void %s_%s_destructor(%s *self)\n{\n\t/*Implementation*/\n"
        "\tBObjectClass* class = b_type_parent_class_get(type_id);\n"
        "\tclass->destructor((BObject*)self);\n"
        "\t\n}\n\n",str_tolower(namespace),str_tolower(type),object_name);

    fprintf(file,"%1$s* %2$s_%3$s_new(void){\n"
        "\t%1$s *obj = (%1$s*)b_object_new(%2$s_%3$s_get_type());\n"
        "\treturn obj;\n"
        "}",object_name,str_tolower(namespace),str_tolower(type));

    fclose(file);
}

void interface_header(char *namespace, char *type, char *parent)
{
    char object_name[1024];
    strcpy(object_name,str_first_upper(namespace));
    strcat(object_name,str_first_upper(type));
    char filename_h[1024];
    strcpy(filename_h,str_tolower(namespace));
    strcat(filename_h,str_tolower(type));
    strcat(filename_h,".h");

    FILE *file;
    file = fopen(filename_h,"w");

    fprintf(file,"#ifndef %1$s\n#define %1$s\n\n",str_toupper(str_replace(filename_h,'.','_')));
    
    char aux[1024];
    strcpy(aux,parent);
    fprintf(file,"#include \"%s.h\"\n\n",str_tolower(aux));

    fprintf(file,"#ifdef __cplusplus\nextern \"C\"{\n#endif\n\n\n");

    fprintf(file,"#define %s_TYPE_%s()\t\t",str_toupper(namespace),str_toupper(type));
    fprintf(file,"%s_%s_get_type()\n",str_tolower(namespace),str_tolower(type));

    char *der = "B_DECLARE_INTERFACE";

    fprintf(file,"%s(%s, %s_%s)\n\n",der,object_name,str_tolower(namespace),str_tolower(type));


    fprintf(file,"struct %sInterface{\n"
        "\t/*virual methods*/\n"
        "};\n\n",object_name);
    

    fprintf(file,"/*Interface Methods*/\n\n\n");
    fprintf(file,"#ifdef __cplusplus\n}\n#endif\n\n\n");
    fprintf(file,"#endif /*%s*/",filename_h);

    fclose(file);
}

void interface_source(char *namespace, char *type, char *parent)
{
    char object_name[1024]="";
    strcpy(object_name,str_first_upper(namespace));
    strcat(object_name,str_first_upper(type));
    char filename_h[1024]="";
    strcpy(filename_h,str_tolower(namespace));
    strcat(filename_h,str_tolower(type));
    strcat(filename_h,".h");
    char filename_c[1024]="";
    strcpy(filename_c,str_tolower(namespace));
    strcat(filename_c,str_tolower(type));
    strcat(filename_c,".c");

    FILE *file;
    file = fopen(filename_c,"w");

    fprintf(file,"#include \"%s\"\n\n\n",str_tolower(str_replace(filename_h,'_','.')));

    char *der = "B_DEFINE_INTERFACE";

    fprintf(file,"%s(%s, %s_%s)\n\n",der,object_name,str_tolower(namespace),str_tolower(type));

    fprintf(file,"/*Interface Methods Implementation*/\n\n\n");

    fclose(file);
}
