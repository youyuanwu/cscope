#ifndef CSCOPE_SORT_FILE_READER_WRITER_H
#define CSCOPE_SORT_FILE_READER_WRITER_H

#define _GNU_SOURCE // for getline in stdio
#include <stdio.h>
#include "lines.h"


#ifdef __cplusplus
extern "C"{
#endif 

// parse file
typedef struct{
    FILE * f;
} FileReader;

int FileReaderInit(FileReader *frdr, char* fname);

// read file content into lines
int FileReaderRead(FileReader *frdr, Lines *lines);

int FileReaderUninit(FileReader *frdr);

typedef struct{
    FILE * f;
} FileWriter;

int FileWriterInit(FileWriter *fwtr, char* fname);

int FileWriterWrite(FileWriter *fwtr, Lines *lines);

int FileWriterUnInit(FileWriter *fwtr);

#ifdef __cplusplus
}
#endif

#endif // CSCOPE_SORT_FILE_READER_WRITER_H