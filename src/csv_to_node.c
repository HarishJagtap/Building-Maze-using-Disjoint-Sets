#include "csv_to_node.h"

/* Returns the no of rows of csv file, excluding heading
 * On Error, returns -1 */
int no_of_lines_in_file (const char * filename)
{
    FILE * fp = open_csv_file(filename);

    if (fp == NULL)
        return -1;

    read_csv_fileheading(fp);

    int temp1, temp2;
    int count = 0;

    while (read_csv_file(fp, &temp1, &temp2) != EOF)
        count++;

    close_csv_file(fp);

    return count;
}

/* reads length, width from maze_size.csv
 * and assigns value to argument maze.length, maze.width
 * On Error, returns EOF */
int read_maze_size (struct maze * pmaze)
{
    FILE * fp = open_csv_file(MAZE_SIZE_FILE);

    if (fp == NULL)
        return EOF;

    read_csv_fileheading(fp);

    int value = read_csv_file(fp, &pmaze->length, &pmaze->width);

    close_csv_file(fp);

    return value;
}

/* Reads all the edges from maze_edges.csv
 * and add them to the argument maze.edges array
 * On Error, returns EOF */
int read_maze_edges (struct maze * pmaze)
{
    FILE * fp = open_csv_file(MAZE_EDGES_FILE);

    if (fp == NULL)
        return EOF;

    read_csv_fileheading(fp);
    
    int i = 0;

    while (read_csv_file(fp, &pmaze->edges[i].node1, &pmaze->edges[i].node2) != EOF)
        i++;

    close_csv_file(fp);

    return 0;
}

/* Reads all the edges of the path from start node to 
 * end node from maze_edges.csv
 * and add them to the argument maze.path array
 * On Error, returns EOF */
int read_maze_path (struct maze * pmaze)
{
    FILE * fp = open_csv_file(MAZE_PATH_FILE);

    if (fp == NULL)
        return EOF;

    read_csv_fileheading(fp);
    
    int i = 0;

    while (read_csv_file(fp, &pmaze->path[i].fromnode, &pmaze->path[i].tonode) != EOF)
        i++;

    close_csv_file(fp);

    return 0;
}

/* Allocates a maze structure in heap memory
 * and fills it with data (size, edges, path)
 * from csv files
 * On Error, returns NULL */
struct maze * read_maze ()
{
    struct maze * rmaze = malloc(sizeof(struct maze));

    int no_of_edges = no_of_lines_in_file(MAZE_EDGES_FILE);
    int no_of_path_edges = no_of_lines_in_file(MAZE_PATH_FILE);

    if ((no_of_edges <= 0) || 
        (no_of_path_edges <= 0)) {

        free(rmaze);
        return NULL;
    }

    rmaze->no_of_edges = no_of_edges;
    rmaze->no_of_path_edges = no_of_path_edges;

    rmaze->edges    = malloc(sizeof(struct maze_edge) * no_of_edges);
    rmaze->path     = malloc(sizeof(struct maze_path) * no_of_path_edges);

    if ((read_maze_size(rmaze) == EOF) ||
        (read_maze_edges(rmaze) == EOF) ||
        (read_maze_path(rmaze) == EOF)) {

        freeup_maze(rmaze);
        return NULL;
    }

    return rmaze;
}

// Cleanup function to deallocate heap memory used by maze structure
void freeup_maze(struct maze * pmaze)
{
    if (pmaze == NULL)
        return;

    free(pmaze->path);
    free(pmaze->edges);
    free(pmaze);
}