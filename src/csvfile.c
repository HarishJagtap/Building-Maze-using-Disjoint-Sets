#include "csvfile.h"

FILE* open_csv_file (const char * filename)
{
    return fopen(filename,"r");
}

void read_csv_fileheading (FILE * fp)
{
    char heading[100];
    fgets(heading, 100, fp);
}

// Returns EOF for End of file
int read_csv_file (FILE * fp, int * column1, int * column2)
{
    char string[100];

    if (fgets(string, 100, fp) == NULL)
        return EOF;

    return sscanf(string, "%d,%d\n", column1, column2);
}

int close_csv_file (FILE *fp)
{
    return fclose(fp);
}