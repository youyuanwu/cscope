#include "filereaderwriter.h"
#include "string.h"
#include <stdlib.h>


int FileReaderInit(FileReader *frdr, char* fname)
{
    frdr->f = fopen(fname, 'r');
    if(frdr->f) 
    {
        return 0;
    }
    return -1;
}

// read file content into lines
// line new line char is stored
int FileReaderRead(FileReader *frdr, Lines *lines)
{
    char buff[1024+1];

    while(1){
        if ( fgets (buff , sizeof(buff) , frdr->f) != NULL )
        {
            buff[sizeof(buff) - 1] = '\0';
            int len = strlen(buff) + 1; // save space for null terminator
            char* line = malloc(len * sizeof(char));
            strcpy(line, buff);
            LinesAdd(lines, line);
        }else{
            break;
        }
    }

    return 0;
}

int FileReaderUninit(FileReader *frdr)
{
    int ret = 0;
    if(frdr->f)
    {
        ret = fclose(frdr->f);
        frdr->f = NULL;
    }
    return ret;
}

// file writer

int FileWriterInit(FileWriter *fwtr, char* fname)
{
    fwtr->f = fopen(fname, 'w');
    if(fwtr->f) 
    {
        return 0;
    }
    return -1;
}

int FileWriterWrite(FileWriter *fwtr, Lines *lines)
{
    for(int i = 0; i < lines->m_index; i++)
    {
        char * line = lines->m_lines[i];
        int ret = fputs(line, fwtr->f);
        // debug
        // puts (line);
        if(ret == EOF)
        {
            return ret;
        }
        // assume line already has new line at the end
        // ret = fputc('\n', fwtr->f);
        // if(ret == EOF)
        // {
        //     return ret;
        // }
    }
    return 0;
}

int FileWriterUnInit(FileWriter *fwtr)
{
    int ret = 0;
    if(fwtr->f)
    {
        ret = fclose(fwtr->f);
        fwtr->f = NULL;
    }
    return ret;
}