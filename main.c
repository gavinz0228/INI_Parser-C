#include "stdio.h"
#include "INI_Reader.h"
char * iniFile = "[PageSettings]\r\nwidth=123\r\nheight=100\r\nname=pagename";
int main(int argc, char* argv[])
{

	INI_Section** sections;
	INI_Section* settings;
	char c;
	int i, j;
	int size;
	char ini_str[5000];
	int len = 0;
	int pos = 0;
	int res;
	//FILE* file;
	//file = fopen("1.ini", "r+");
	//fread_s(ini_str,5000, 5000/2, 2, &file);
	//while (fgets(ini_str + len, 1, &file) != NULL)
	//{
	//	len += 1;
	//}
	//ini_str[len] = 0;
	//fclose(&file);
	//ini_load(iniFile, &sections, &size);
	////settings = get_ini_section_by_name(&sections, size, "PageSettings");
	//for (i = 0; i < size; i++)
	//{
	//	printf("Section: %s \n-----------------------------------\n", sections[i]->name);
	//	for (j = 0; j < sections[i]->kv_size; j++)
	//	{
	//		printf("%s -> %s \n", sections[i]->kv[j]->key, sections[i]->kv[j]->val_str);
	//	}
	//}
	//ini_unload(&sections, size);
	//c = scanf_s("%s");
	int linepos = 0;
	INI_Line line;
	len = strlen(iniFile);
	while (pos < len)
	{
		ini_str[linepos] = iniFile[pos];
		linepos++;

		if (iniFile[pos] == '\n')
		{
			ini_parse_line(ini_str, &line);
			if (line.is_section)
				printf("%s", line.section);
			else
				printf("%s, %s", line.key, line.value);
			linepos = 0;
		}
		pos++;
	}
	c = scanf_s("%s");
	return 0;
}
