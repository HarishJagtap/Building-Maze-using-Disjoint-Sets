#ifndef NODE_TO_COORDINATES_H
#define NODE_TO_COORDINATES_H

#include "csv_to_node.h"     // for maze structure

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

struct edge_coordinates {

    int x1, y1;
    int x2, y2;
};

int node_to_xcoordinate (struct maze * pmaze, int node, int edge_length);

int node_to_ycoordinate (struct maze * pmaze, int node, int edge_length);

/* Takes all the maze edges and converts their
 * values to coordinates suitable for opengl drawing.
 * The result values are stored in a structure in heap memory and 
 * its pointer is returned */
struct edge_coordinates * edges_to_coordinates (struct maze * pmaze, int edge_length, int margin);

/* Takes all the maze edges of the path connecting start node and last node
 * and converts their values to coordinates suitable for opengl drawing.
 * The result values are stored in a structure in heap memory and 
 * its pointer is returned */
struct edge_coordinates * path_edges_to_coordinates (struct maze * pmaze, int edge_length, int margin);

#endif