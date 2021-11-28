#include "cscopesort.h"
#include "filereaderwriter.h"

FILE * SortFile(FILE * fin)
{
    //fprintf(stderr, "sorting file \n");
    int ret = 0;
    FILE * retf = NULL;
    
    FileReader frdr;
    frdr.f = fin;

    FileWriter frwtr;
    frwtr.f = tmpfile();

    if(!frwtr.f)
    {
        goto finish;
    }

    Lines lines;
    LinesInit(&lines);

    ret = FileReaderRead(&frdr, &lines);
    if (ret != 0)
    {
        goto finish;
    }

    LinesSort(&lines);

    ret = FileWriterWrite(&frwtr,&lines);
    if(ret != 0)
    {
        goto finish;
    }
    retf = frwtr.f;

    fflush(frwtr.f);
    rewind(frwtr.f);

    //fprintf(stderr, "wrote %d lines", lines.m_index);

finish:    

    LinesUninit(&lines);
    // no need to clean up reader wirters since no ownership
    return retf;
}