#include "node_to_coordinates.h"

//#define DEBUG

int node_to_xcoordinate (struct maze * pmaze, int node, int edge_length)
{
    return (node % pmaze->length) * edge_length + edge_length / 2;
}

int node_to_ycoordinate (struct maze * pmaze, int node, int edge_length)
{
    return (pmaze->width - node / pmaze->length) * edge_length - edge_length / 2;
}

/* Takes all the maze edges and converts their
 * values to coordinates suitable for opengl drawing.
 * The result values are stored in a structure in heap memory and 
 * its pointer is returned */
struct edge_coordinates * edges_to_coordinates (struct maze * pmaze, int edge_length, int margin)
{
    struct edge_coordinates * edges = malloc(sizeof(struct edge_coordinates) * pmaze->no_of_edges);

    for (int i = 0; i < pmaze->no_of_edges; i++) {

        int min_node            = MIN(pmaze->edges[i].node1, pmaze->edges[i].node2);
        int is_edge_vertical    = (abs(pmaze->edges[i].node1 - pmaze->edges[i].node2) == 1);

        int x = node_to_xcoordinate(pmaze, min_node, edge_length);
        int y = node_to_ycoordinate(pmaze, min_node, edge_length);

        if (is_edge_vertical) {

            edges[i].x1 = edges[i].x2 = x + edge_length / 2 + margin;
            edges[i].y1 = y - edge_length / 2 + margin;
            edges[i].y2 = y + edge_length / 2 + margin;
        }
        else {

            edges[i].y1 = edges[i].y2 = y - edge_length / 2 + margin;
            edges[i].x1 = x - edge_length / 2 + margin;
            edges[i].x2 = x + edge_length / 2 + margin;
        }

        #ifdef DEBUG
        printf("Edge Line %d: Point(%d, %d) to Point(%d, %d)\n", i, edges[i].x1, edges[i].y1, edges[i].x2, edges[i].y2);
        #endif
    }

    #ifdef DEBUG
    printf("\n");
    #endif

    return edges;
}

/* Takes all the maze edges of the path connecting start node and last node
 * and converts their values to coordinates suitable for opengl drawing.
 * The result values are stored in a structure in heap memory and 
 * its pointer is returned */
struct edge_coordinates * path_edges_to_coordinates (struct maze * pmaze, int edge_length, int margin)
{
    struct edge_coordinates * path_edges = malloc(sizeof(struct edge_coordinates) * pmaze->no_of_path_edges);

    for (int i = 0; i < pmaze->no_of_path_edges; i++) {

        int min_node = MIN(pmaze->path[i].fromnode, pmaze->path[i].tonode);

        path_edges[i].x1 = node_to_xcoordinate(pmaze, pmaze->path[i].fromnode, edge_length) + margin;
        path_edges[i].y1 = node_to_ycoordinate(pmaze, pmaze->path[i].fromnode, edge_length) + margin;

        path_edges[i].x2 = node_to_xcoordinate(pmaze, pmaze->path[i].tonode, edge_length) + margin;
        path_edges[i].y2 = node_to_ycoordinate(pmaze, pmaze->path[i].tonode, edge_length) + margin;

        #ifdef DEBUG
        printf("Path Edge Line %d: Point(%d, %d) to Point(%d, %d)\n", i, path_edges[i].x1, path_edges[i].y1, path_edges[i].x2, path_edges[i].y2);
        #endif
    }

    #ifdef DEBUG
    printf("\n");
    #endif

    return path_edges;
}
