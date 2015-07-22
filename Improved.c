include "stdlib"
int inipos=0;
int inilen=0;
char *str="[section]";
int main(){
    inilen=strlen(str);
}
int match_lb()
{
    return str[inipos]=='['&&inipos++;
}
int match_rb()
{
    return str[inipos]==']'&&inipos++;
}
int extract_str(char* string)
{
    char c=str[inipos];
    int pos=0;
    is_legal_key_char(c);
    string[pos]=c;
    pos++;
    c=see_next();
    while(is_legal_key_char(c)))
    {
        string[pos]=c
        pos++;
        inipos++;
        c=see_next();
    }
    string[pos]=0;
}
char* see_next()
{
    return str+inipos+1;
}
void parse()
{
    char string[30];
    match_lb();  
    extract_str(string);
    match_rb();
    
}
int is_legal_key_char(char c)
{
    if(c!='\n'&&c!='\r'&&c!='=')
        return true;
    else 
        return false;
}
