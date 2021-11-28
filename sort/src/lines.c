#include <stdbool.h>
#include <stdlib.h>
#include "lines.h"
#include "string.h"

// void sort()
// {

// }

// typedef struct
// {
//     char* filename;
// } FileReader;

// void FileReaderRead()
// {
//     char** lines;
//     int line_max = 1000;
// }

void LinesInit(Lines * lines)
{
    lines->m_cap = 11;
    lines->m_lines = malloc(lines->m_cap * sizeof(char*));
    if(!lines->m_lines){
        // todo: error
    }
    // lines->m_count = 0;
    lines->m_index = 0;
}

bool LinesIsFull(Lines * lines)
{
    return lines->m_index == lines->m_cap;
}

int LinesSize(Lines * lines)
{
    return lines->m_index;
}

void LinesResize(Lines * lines)
{
    int new_size = lines->m_cap * 2;
    lines->m_lines = realloc(lines->m_lines, new_size * sizeof(char*));
    if(!lines->m_lines)
    {
        // todo: error
    }
    lines->m_cap = new_size;
}

// Lines takes ownership of line char.
void LinesAdd(Lines * lines, char* line)
{
    if(LinesIsFull(lines))
    {
        LinesResize(lines);
    }
    lines->m_lines[lines->m_index] = line;
    lines->m_index++;
}

// frees all mems
void LinesUninit(Lines * lines)
{
    if(!lines->m_lines){
        return;
    }
    for(int i = 0; i < lines->m_index; i++)
    {
        char* line = lines->m_lines[i];
        free(line);
    }
    free(lines->m_lines);
}

int linesSortString( const void *str1, const void *str2 )
{
    char *const *pp1 = str1;
    char *const *pp2 = str2;
    return strcmp(*pp1, *pp2);
}

void LinesSort(Lines * lines)
{
    qsort(lines->m_lines, lines->m_index, sizeof(char*), linesSortString);
}




