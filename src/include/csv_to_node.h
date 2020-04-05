#ifndef CSV_TO_NODE_H
#define CSV_TO_NODE_H

#include <stdlib.h>     // for malloc(), free()
#include <stdio.h>      // for EOF constant
#include "csvfile.h"

#define MAZE_EDGES_FILE "result/maze_edges.csv"
#define MAZE_PATH_FILE  "result/maze_path.csv"
#define MAZE_SIZE_FILE  "result/maze_size.csv"

struct maze_edge {

    int node1, node2;
};

struct maze_path {

    int fromnode, tonode;
};

struct maze {

    int length, width;
    int no_of_edges, no_of_path_edges;
    struct maze_edge * edges;
    struct maze_path * path;
};

int no_of_lines_in_file (const char * filename);

int read_maze_size (struct maze * pmaze);
int read_maze_edges (struct maze * pmaze);
int read_maze_path (struct maze * pmaze);

/* Main functions
 *
 * call read_maze() to get a pointer to maze structure
 * which contains data from the csv files
 * On Error, read_maze() returns value of NULL
 * 
 * After use of maze structure, call freeup_maze() to 
 * clear the memory held the maze structure */
struct maze * read_maze ();
void freeup_maze(struct maze * pmaze);

#endif