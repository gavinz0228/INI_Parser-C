#include "stdlib.h"
#ifndef INI_READER_H
#define INI_READER_H

#define MAX_SECTION_SIZE			50
#define ini_malloc(size)			malloc(size)
#define ini_free(ptr)				free(ptr)
typedef struct
{
	char key[50];
	char val_str[50];
	long val_int;
	int val_type;
}INI_KeyValue;

typedef struct
{
	char name[50];
	INI_KeyValue* kv[50];
	int kv_size;
}INI_Section;

int ini_load(char* ini, INI_Section ** sections, int *section_num);
INI_Section * get_ini_section_by_name(INI_Section** sections, int section_num, char* section_name);
int ini_unload(INI_Section ** sections, int section_num);

#endif
