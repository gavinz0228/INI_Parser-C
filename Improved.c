#include "stdlib.h"
#include "stdio.h"
#include "string.h"
int inipos=0;
int inilen=0;
char *str="[section]\r\n";
void parse_section(char* string);
int main(){
    char string[30];
    inilen=strlen(str);
    parse_section(string);
    
    return 0;
}
int match_lb()
{
    return str[inipos]=='['&&inipos++;
}
int match_rb()
{
    return str[inipos]==']'&&inipos++;
}
int is_legal_key_char(char c);
char see_next();
int extract_str(char* dest)
{
    char c=str[inipos];
    int pos=0;
    is_legal_key_char(c);
    dest[pos]=c;
    pos++;
    c=see_next();
    while(is_legal_key_char(c))
    {
        dest[pos]=c;
        pos++;
        inipos++;
        c=see_next();
    }
    dest[pos]=0;
    inipos++;
}
char see_next()
{
    return *(str+inipos+1);
}
void parse_section(char* string)
{

    match_lb();  
    extract_str(string);
    match_rb();
    printf(" %s ",string);
    
}
int is_legal_key_char(char c)
{
    if(c!='\n'&&c!='\r'&&c!='='&&c!=']')
        return 1;
    else 
        return 0;
}
