#include "lines.h"
#include "filereaderwriter.h"
#include "cscopesort.h"

#include "gtest/gtest.h"

#include <string.h>

TEST(Lines, Basic)
{
    Lines lines;
    LinesInit(&lines);

    ASSERT_EQ(0, LinesSize(&lines));

    char * dest = (char *) malloc(10 * sizeof(char*));
    char *src = (char*)"hello";
    strcpy(dest,src);

    LinesAdd(&lines, dest);
    ASSERT_EQ(1, LinesSize(&lines));
    ASSERT_STREQ("hello", lines.m_lines[0]);

    LinesUninit(&lines);
}

TEST(Lines, ReadFile)
{
    // special init
    FileReader frdr;
    frdr.f = tmpfile();
    ASSERT_TRUE(frdr.f != NULL);

    // put content
    int ret = fputs("hello\nworld\n", frdr.f);
    // ASSERT_NE(0, ret);
    ASSERT_NE(EOF, ret);
    rewind(frdr.f);

    Lines lines;
    LinesInit(&lines);

    FileReaderRead(&frdr,&lines);
    FileReaderUninit(&frdr);

    ASSERT_EQ(2, LinesSize(&lines));
    ASSERT_STREQ("hello\n", lines.m_lines[0]);
    ASSERT_STREQ("world\n", lines.m_lines[1]);
    LinesUninit(&lines);
}


TEST(Lines, Sort)
{
    Lines lines;
    LinesInit(&lines);

    ASSERT_EQ(0, LinesSize(&lines));

    char * a = (char *) malloc(10 * sizeof(char*));
    *a = 'a';
    *(a + 1) = '\0';

    char * b = (char *) malloc(10 * sizeof(char*));
    *b = 'b';
    *(b + 1) = '\0';

    char * c = (char *) malloc(10 * sizeof(char*));
    *c = 'c';
    *(c + 1) = '\0';

    char * d = (char *) malloc(10 * sizeof(char*));
    *d = 'd';
    *(d + 1) = '\0';

    LinesAdd(&lines, d);
    LinesAdd(&lines, b);
    LinesAdd(&lines, c);
    LinesAdd(&lines, a);

    ASSERT_EQ(4, LinesSize(&lines));

    LinesSort(&lines);

    ASSERT_STREQ("a", lines.m_lines[0]);
    ASSERT_STREQ("b", lines.m_lines[1]);
    ASSERT_STREQ("c", lines.m_lines[2]);
    ASSERT_STREQ("d", lines.m_lines[3]);

    LinesUninit(&lines);
}

TEST(Lines, WriteFile)
{
    int ret = 0;

    Lines linesWrite;
    LinesInit(&linesWrite);
    char * a = (char *) malloc(10 * sizeof(char*));
    *a = 'a';
    *(a + 1) = '\n';
    *(a + 2) = '\0';

    char * b = (char *) malloc(10 * sizeof(char*));
    *b = 'b';
    *(b + 1) = '\n';
    *(b + 2) = '\0';
    LinesAdd(&linesWrite, a);
    LinesAdd(&linesWrite, b);

    ASSERT_EQ(2, LinesSize(&linesWrite));

    // special init
    FileWriter fwtr;
    fwtr.f = tmpfile();
    ASSERT_TRUE(fwtr.f != NULL);
    ret = FileWriterWrite(&fwtr, &linesWrite);
    ASSERT_EQ(0, ret);

    // read temp file
    FileReader frdr;
    frdr.f = fwtr.f;
    rewind(frdr.f);

    Lines linesRead;
    LinesInit(&linesRead);

    FileReaderRead(&frdr,&linesRead);
    
    frdr.f = NULL; // not owner
    FileReaderUninit(&frdr);
    FileWriterUnInit(&fwtr);

    ASSERT_EQ(2, LinesSize(&linesRead));
    ASSERT_STREQ("a\n", linesRead.m_lines[0]);
    ASSERT_STREQ("b\n", linesRead.m_lines[1]);

    LinesUninit(&linesWrite);
    LinesUninit(&linesRead);
}

TEST(Lines, EndToEnd)
{
    int ret = 0;

    FILE * tmp = tmpfile();
    ASSERT_TRUE(NULL != tmp);
    ret = fputs("b\nc\nd\na\n", tmp);
    ASSERT_NE(EOF, ret);
    ret = 0;
    ret = fflush(tmp);
    ASSERT_NE(EOF, ret);
    rewind(tmp);

    FILE * fSorted = SortFile(tmp);
    ASSERT_TRUE(NULL != fSorted);
    char buff[8+1];
    ret = fread(buff, sizeof(char), sizeof(buff) - 1, fSorted);
    buff[8] = '\0';
    ASSERT_EQ(8, ret);
    ret = 0;

    ASSERT_STREQ("a\nb\nc\nd\n", (char*)buff);

    // clean up
    fclose(tmp);
    fclose(fSorted);
}