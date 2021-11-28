#ifndef CSCOPE_LINES_H
#define CSCOPE_LINES_H

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif 

typedef struct{
    char ** m_lines;
    int m_cap;
    // int m_count;
    int m_index; // points to the empty bucket
} Lines;


void LinesInit(Lines * lines);

bool LinesIsFull(Lines * lines);

int LinesSize(Lines * lines);

void LinesResize(Lines * lines);

void LinesAdd(Lines * lines, char* line);

void LinesUninit(Lines * lines);

void LinesSort(Lines * lines);

#ifdef __cplusplus
}
#endif

#endif // CSCOPE_LINES_H