#include "INI_Reader.h"
#include <stdlib.h>

unsigned int ini_pos = 0;
unsigned int ini_len = 0;
int ini_eof = 0;
char* line_buffer;
int line_buffer_pos = 0;
int ini_read_line(char* ini)
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

	strncpy(line_buffer, line_start, line_len);
	line_buffer[line_len] = 0;
	return 1;
}
INI_Section parse_ini(char* ini, int page)
{
	ini_pos = 0;
	ini_eof = 0;
	ini_len = strlen(ini);

}
int skip_space()
{
	while (line_buffer[line_buffer_pos] == ' ')
		line_buffer_pos++;
}
int is_kv_line()
{
	return strchr(line_buffer, '=') != NULL;
}
int is_section_line()
{
	return strchr(line_buffer, '[') != NULL;
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
int extract_key(char* dest)
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
int extract_string(char* dest)
{
	char c = line_buffer[line_buffer_pos];
	int pos = 0;
	is_legal_char(c);
	dest[pos] = c;
	pos++;
	c = see_next();
	while (is_legal_char(c))
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
	extract_key(string);
	match_rb();

}
void parse_kv(char *key, char* value)
{
	extract_key(key);
	match_eq();
	extract_string(value);
}
int is_legal_key_char(char c)
{
	if ((c>96 && c<123) || (c>64 && c<91) || (c>47 && c<58) || c == 95)
		return 1;
	else
		return 0;
}
int is_legal_char(char c)
{
	if (c != '\n'&&c != '\r'&&c != 0)
		return 1;
	else
		return 0;
}


int ini_parse_line(char* ini_str, INI_Line *line)
{
	line_buffer = ini_str;
	line_buffer_pos = 0;
	if (is_kv_line())
	{
		parse_kv(line->key, line->value);
		line->is_key_value = 1;
		line->is_section = 0;
	}
	else if(is_section_line())
	{
		parse_section(line->section);
		line->is_key_value = 0;
		line->is_section = 1;
	}

}

INI_Section * get_ini_section_by_name(INI_File * ini, char* section_name)
{
	int i;
	for (i = 0; i <ini->size; i++)
	{
		if (strcmp(ini->sections[i]->name, section_name) == 0)
		{
			return ini->sections[i];
		}
	}
	return NULL;
}
int ini_load(char* ini_str, INI_File *ini)
{
	ini_pos = 0;
	ini_eof = 0;
	INI_KeyValue *kv;
	int kv_size = 0;
	ini_len = strlen(ini_str);
	int sec_num = 0;
	line_buffer = malloc(sizeof(char)* 100);
	while (ini_read_line(ini_str))
	{
		if (is_section_line(line_buffer))
		{
			ini->sections[sec_num] = ini_malloc(sizeof(INI_Section));
			parse_section(ini->sections[sec_num]->name);
			ini->sections[sec_num]->kv_size = 0;
			sec_num++;
		}
		else if (is_kv_line(line_buffer))
		{
			kv = ini_malloc(sizeof(INI_KeyValue));
			parse_kv(kv->key, kv->value);
			ini->sections[sec_num - 1]->kv[ini->sections[sec_num - 1]->kv_size] = kv;
			ini->sections[sec_num - 1]->kv_size++;
		}

	}
	ini->size = sec_num;
	return 1;
}
int ini_unload(INI_File * ini)
{
	int i;
	int j;
	for (i = 0; i < ini->size; i++)
	{
		for (j = 0; j < ini->sections[i]->kv_size; j++)
		{
			ini_free(ini->sections[i]->kv[j]);
		}
		ini_free(ini->sections[i]);
	}
	free(line_buffer);
}
