#ifndef CSCOPE_SORT_H
#define CSCOPE_SORT_H

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

// returns a new temp file with content in fin.
// user should close the returned file when finished.
FILE * SortFile(FILE * fin);

#ifdef __cplusplus
}
#endif

#endif // CSCOPE_SORT_H