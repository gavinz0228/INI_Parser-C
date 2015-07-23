#include "stdlib.h"
#ifndef INI_READER_H
#define INI_READER_H

#define MAX_SECTION_SIZE			100
#define LINE_BUFFER_SIZE			100
#define MAX_KEY_VALUE				50
#define ini_malloc(size)			malloc(size)
#define ini_free(ptr)				free(ptr)
typedef struct
{
	char key[50];
	char value[50];
	char section[50];
	int is_section;
	int is_key_value;
	int is_empty;
}INI_Line;
typedef struct
{
	char key[50];
	char value[50];
}INI_KeyValue;

typedef struct
{
	char name[50];
	INI_KeyValue* kv[MAX_KEY_VALUE];
	int kv_size;
}INI_Section;
typedef struct
{
	INI_Section* sections[MAX_SECTION_SIZE];
	int size;
}INI_File;

int ini_load(char* ini, INI_File *ini_file);
INI_Section * get_ini_section_by_name(INI_Section** sections, int section_num, char* section_name);
int ini_unload(INI_File *ini);

int ini_parse_line(char* ini_str,INI_Line *line);
#endif
