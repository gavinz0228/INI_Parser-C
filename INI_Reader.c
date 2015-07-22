#include "INI_Reader.h"
#include <stdlib.h>



unsigned int ini_pos=0;
unsigned int ini_len=0;
int ini_eof=0;
static char line_buffer[50];
int ini_read_line(char* ini,char* line_buf)
{
	int  line_len=0;
	char* line_start;
	//reach the end of the file
	if(ini_pos==ini_len)
		return 0;

	line_start=ini+ini_pos;
	while(ini_pos<ini_len)
	{
		if(ini[ini_pos]=='\n')
		{
			ini_pos++;
			line_len++;
			break;
		}
		else if(ini[ini_pos]=='\r')
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
	}

	strncpy(line_buf,line_start,line_len);
	line_buf[line_len] = 0;
	return 1;
}
INI_Section parse_ini(char* ini,int page)
{
	ini_pos=0;
	ini_eof=0;
	ini_len=strlen(ini);

}
int is_kv_line(char * line)
{
	return strchr(line, '=') != NULL;
}
int is_section_line(char* line)
{
	return strchr(line, '[') != NULL;
}
int get_key_value(char*line,INI_KeyValue *kv)
{
	int pos=0;
	char c;
	int copy_key,copy_val,copy_str,key_pos,val_pos;
	//start to copy key
  copy_key=1;
	copy_val=0;
	key_pos=0;
	val_pos=0;
	c=line_buffer[0];
	
	while(c!=0)
	{
		if(c=='#')
		{
			break;
		}
		else if(c=='=')
		{
			copy_key=0;
			copy_val=1;
			//stop copying key
			kv->key[key_pos]=0;
		}
		else if(c==' ')
		{
			if(copy_key)
			{
				continue;
			}
			//start copying the value,
			else if(copy_val)
			{
				kv->val_str[val_pos]=line[pos];
				val_pos++;
			}
		}
		
		else if(c=='"')
		{
			
		}
		else
		{
			if(copy_key)
			{
				kv->key[key_pos]=line[pos];
				key_pos++;
			}
			else if(copy_val)
			{
				kv->val_str[val_pos]=line[pos];
				val_pos++;
			}
		}
		pos++;
		c=line_buffer[pos];
	}
	if(pos==0)
	{
		kv=NULL;
	} 
	kv->val_str[val_pos]=0;
	
	return 1;
}
int get_section_name(char* line, char* name)
{
	int pos=0;
	char c;
	int open_brac,is_section,copy_str,copy_pos;

	open_brac=0;
	is_section=0;
	copy_str=0;
	copy_pos=0;
	c=line_buffer[0];
	 while(c!=0)
	 {
		 if(c==' ')
		 {
		 }
		 if(c=='#')
		 {
			 break;
		 }
		 else if(c=='[')
		 {
			 open_brac=1;
			 is_section=1;
			 copy_str=1;
		 }
		 else if(c==']')
		 {
			 open_brac=0;
			 copy_str=0;
			 name[copy_pos]=0;
		 }
		 //legal character
		 else
		 {
			 if(copy_str==1)
			 {
			 name[copy_pos]=line[pos];
			 copy_pos++;
			 }
		 }
		 
		 pos++;
		 c=line_buffer[pos];
	 }
	 //the end of a line
	 //unclosed bracket
	 if(open_brac==1)
	 {
		 return 0;
	 }
	 return 1;
}
INI_Section * get_ini_section_by_name(INI_Section** sections,int section_num, char* section_name)
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
int ini_load(char* ini,INI_Section ** sections,int *section_num)
{
	ini_pos=0;
	ini_eof=0;
	INI_KeyValue *kv;
	int kv_size = 0;
	ini_len=strlen(ini);
	*sections = ini_malloc(sizeof(void*)*MAX_SECTION_SIZE);
	int sec_num = 0;
	while(ini_read_line(ini,line_buffer))
	{
		if(is_section_line(line_buffer))
		{
			sections[sec_num] = ini_malloc(sizeof(INI_Section));
			get_section_name(line_buffer,sections[sec_num]->name);
			sections[sec_num]->kv_size = 0;
			sec_num++;
			
		}
		else if(is_kv_line(line_buffer))
		{
			
			kv=ini_malloc(sizeof(INI_KeyValue));
			get_key_value(line_buffer, kv);
			sections[sec_num - 1]->kv[sections[sec_num - 1]->kv_size] = kv;

			sections[sec_num- 1]->kv_size++;
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
}
