#ifndef CSVFILE_H
#define CSVFILE_H

#include <stdio.h>

FILE*   open_csv_file (const char * filename);

void    read_csv_fileheading (FILE * fp);

// Returns EOF for End of file condition
int     read_csv_file (FILE * fp, int * column1, int * column2);

int     close_csv_file (FILE *fp);

#endif