#include "INI_Reader.h"
#include <stdlib.h>

unsigned int ini_pos = 0;
unsigned int ini_len = 0;
int ini_eof = 0;
static char line_buffer[50];
static int line_buffer_pos=0;
int ini_read_line(char* ini, char* line_buf)
{
	int  line_len = 0;
	char* line_start;
	//reach the end of the file
	if (ini_pos == ini_len)
		return 0;

	line_start = ini + ini_pos;
	while (ini_pos<ini_len)
	{
		if (ini[ini_pos] == '\n')
		{
			ini_pos++;
			line_len++;
			break;
		}
		else if (ini[ini_pos] == '\r')
		{
			ini_pos++;
			if (ini[ini_pos] == '\n')
			{
				ini_pos++;
			}
			break;
		}
		line_len++;
		ini_pos++;
		line_buffer_pos = 0;
	}

	strncpy(line_buf, line_start, line_len);
	line_buf[line_len] = 0;
	return 1;
}
INI_Section parse_ini(char* ini, int page)
{
	ini_pos = 0;
	ini_eof = 0;
	ini_len = strlen(ini);

}

int is_kv_line(char * line)
{
	return strchr(line, '=') != NULL;
}
int is_section_line(char* line)
{
	return strchr(line, '[') != NULL;
}

int match_lb()
{
	return line_buffer[line_buffer_pos] == '['&&line_buffer_pos++;
}
int match_rb()
{
	return line_buffer[line_buffer_pos] == ']'&&line_buffer_pos++;
}
int match_eq()
{
	return line_buffer[line_buffer_pos] == '='&&line_buffer_pos++;
}
int is_legal_key_char(char c);
char see_next();
int extract_str(char* dest)
{
	char c = line_buffer[line_buffer_pos];
	int pos = 0;
	is_legal_key_char(c);
	dest[pos] = c;
	pos++;
	c = see_next();
	while (is_legal_key_char(c))
	{
		dest[pos] = c;
		pos++;
		line_buffer_pos++;
		c = see_next();
	}
	dest[pos] = 0;
	line_buffer_pos++;
}
char see_next()
{
	return *(line_buffer + line_buffer_pos + 1);
}
void parse_section(char* string)
{

	match_lb();
	extract_str(string);
	match_rb();
	printf(" %s ", string);

}
void parse_kv(char *key, char* value)
{
	extract_str(key);
	match_eq();
	extract_str(value);
}
int is_legal_key_char(char c)
{
	if (c != '\n'&&c != '\r'&&c != '='&&c != ']')
		return 1;
	else
		return 0;
}

INI_Section * get_ini_section_by_name(INI_Section** sections, int section_num, char* section_name)
{
	int i;
	for (i = 0; i < section_num; i++)
	{
		if (strcmp(sections[i]->name, section_name) == 0)
		{
			return sections[i];
		}
	}
	return NULL;
}
int ini_load(char* ini, INI_Section ** sections, int *section_num)
{
	ini_pos = 0;
	ini_eof = 0;
	INI_KeyValue *kv;
	int kv_size = 0;
	ini_len = strlen(ini);
	*sections = ini_malloc(sizeof(void*)*MAX_SECTION_SIZE);
	int sec_num = 0;
	while (ini_read_line(ini, line_buffer))
	{
		if (is_section_line(line_buffer))
		{
			sections[sec_num] = ini_malloc(sizeof(INI_Section));
			parse_section(sections[sec_num]->name);
			sections[sec_num]->kv_size = 0;
			sec_num++;

		}
		else if (is_kv_line(line_buffer))
		{
			kv = ini_malloc(sizeof(INI_KeyValue));
			parse_kv(kv->key,kv->val_str);
			sections[sec_num - 1]->kv[sections[sec_num - 1]->kv_size] = kv;
			sections[sec_num - 1]->kv_size++;
		}

	}
	*section_num = sec_num;
	return 1;
}
int ini_unload(INI_Section ** sections, int section_num)
{
	int i;
	int j;
	for (i = 0; i < section_num; i++)
	{
		for (j = 0; j < sections[i]->kv_size; j++)
		{
			ini_free(sections[i]->kv[j]);
		}
		ini_free(sections[i]);
	}
	ini_free(*sections);
}
