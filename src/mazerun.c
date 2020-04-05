#include <stdlib.h> // For malloc(), free()
#include <stdio.h>
#include <string.h>

#ifdef __linux__ 
#include <GL/glut.h>
#elif _WIN32
#include "glut.h"   // local header file
// For windows, the libraries of glut are included in repo
// But for linux, install mesa-utils, freeglut3-dev packages
#endif

#include "node_to_coordinates.h"

//#define DEBUG
#define MARGIN      40
#define EDGE_LENGTH 40

/* Global variables used only for glutloop() function
 * since it is a function pointer argument of
 * glutDisplayFunc() taking only void as argument */
struct maze *               gMaze       = NULL;
struct edge_coordinates *   gEdges      = NULL;
struct edge_coordinates *   gPathEdges  = NULL;

// Ways of displaying maze, read from environment variables
int dont_show_maze_path      = 0;
int show_maze_path_dotted    = 0;

void glutloop (void)
{
    // Clear the buffers currently enabled for color writing
    glClear(GL_COLOR_BUFFER_BIT);

    // Use black color to draw lines
    glColor4f(0.0, 0.0, 0.0, 1.0);

    glLineWidth(3);

     // Draw the rectangular boundary of maze
    glBegin(GL_LINE_LOOP);
        glVertex2i(MARGIN                               , MARGIN);
        glVertex2i(gMaze->length * EDGE_LENGTH + MARGIN , MARGIN);
        glVertex2i(gMaze->length * EDGE_LENGTH + MARGIN , gMaze->width * EDGE_LENGTH + MARGIN);
        glVertex2i(MARGIN                               , gMaze->width * EDGE_LENGTH + MARGIN);
    glEnd();

    // Draw the visible edges of maze one by one, except boundary edges
    for (int i = 0; i < gMaze->no_of_edges; i++) {

         glBegin(GL_LINES);
            glVertex2i(gEdges[i].x1, gEdges[i].y1);
            glVertex2i(gEdges[i].x2, gEdges[i].y2);
        glEnd();
    }

    // Use white color to draw lines (equivalent to erasing lines)
    glColor3f(1.0, 1.0, 1.0);

    // Erase the line above start node, since it is the entrance to the maze
    glBegin(GL_LINES);
        glVertex2i(MARGIN               , gMaze->width * EDGE_LENGTH + MARGIN);
        glVertex2i(MARGIN + EDGE_LENGTH , gMaze->width * EDGE_LENGTH + MARGIN);
    glEnd();

    // Erase the line below last node, since it is the exit from maze
    glBegin(GL_LINES);
        glVertex2i((gMaze->length - 1) * EDGE_LENGTH + MARGIN   , MARGIN);
        glVertex2i(gMaze->length * EDGE_LENGTH + MARGIN         , MARGIN);
    glEnd();

    //Draw path between start node and last node
    if(!dont_show_maze_path) {

        for (int i = 0; i < gMaze->no_of_path_edges; i++) {

            int is_vertical_edge = (gPathEdges[i].x1 == gPathEdges[i].x2);

            int x_max = MAX(gPathEdges[i].x1, gPathEdges[i].x2) + EDGE_LENGTH / 2;
            int y_max = MAX(gPathEdges[i].y1, gPathEdges[i].y2) + EDGE_LENGTH / 2;
            int x_min = MIN(gPathEdges[i].x1, gPathEdges[i].x2) - EDGE_LENGTH / 2;
            int y_min = MIN(gPathEdges[i].y1, gPathEdges[i].y2) - EDGE_LENGTH / 2;

            // Draw path with red color of high transperancy
            glColor4f(1.0, 0.0, 0.0, 0.2);

            if (show_maze_path_dotted) {

                // use opaque red color for dots
                glColor4f(1.0, 0.0, 0.0, 1.0);

                // dot size
                glPointSize(5.0);

                // The path is between two nodes, draw 2 points one at node1 and node2
                glBegin(GL_POINTS);
                    glVertex2i(gPathEdges[i].x1, gPathEdges[i].y1);
                    glVertex2i(gPathEdges[i].x2, gPathEdges[i].y2);
                glEnd();
            }
            else {

                /* Draw rectangle to fill the 2 nodes
                 * The rectangle can be vertically larger for vertical path
                 * , or horizontally larger for horizontal path */
                if (is_vertical_edge) {

                    glBegin(GL_POLYGON);
                        glVertex2i(gPathEdges[i].x1 - EDGE_LENGTH / 2, y_max);
                        glVertex2i(gPathEdges[i].x1 + EDGE_LENGTH / 2, y_max);
                        glVertex2i(gPathEdges[i].x1 + EDGE_LENGTH / 2, y_min);
                        glVertex2i(gPathEdges[i].x1 - EDGE_LENGTH / 2, y_min);
                    glEnd();
                }
                else {

                    glBegin(GL_POLYGON);
                        glVertex2i(x_min, gPathEdges[i].y1 + EDGE_LENGTH / 2);
                        glVertex2i(x_max, gPathEdges[i].y1 + EDGE_LENGTH / 2);
                        glVertex2i(x_max, gPathEdges[i].y1 - EDGE_LENGTH / 2);
                        glVertex2i(x_min, gPathEdges[i].y1 - EDGE_LENGTH / 2);
                    glEnd();
                }
            }
        }
    }
    
    // force execute any pending GL commands
    glFlush();
}

void opengl_setup_view_region (int region_length, int region_width)
{
    /* specifies the red, green, blue, and alpha values used by glClear 
     * to clear the color buffers. Range of values [0.0 to 1.0].
     * Set up white background */
    glClearColor(1.0, 1.0, 1.0, 1.0);

    /* Enable mixing the color values with the values in the color buffers
     * Refer https://learnopengl.com/Advanced-OpenGL/Blending */
    glEnable(GL_BLEND);
    /* let the destination object have a contribution equal to the 
     * remainder of the alpha value, (1  - source alpha value) */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We are using only 2D, no need of projection
    glMatrixMode(GL_MODELVIEW);

    /* Orthographic projection(objects appear the same
     * size, no matter the distance)
     * set up a two-dimensional orthographic viewing region
     * (left, right, bottom, top) */
    gluOrtho2D(0.0, (float) region_length, 0.0, (float) region_width);

    // mark the current window as needing to be redisplayed
    glutPostRedisplay();
}

int main (int argc, char **argv)
{
    struct maze * rmaze = read_maze();

    if (rmaze == NULL) {

        printf ("\nError reading maze csv files.\n"
                "First create maze data, mazerun.py --create-data [length of maze] [width of maze].\n");
        return 0;
    }

    #ifdef DEBUG
    printf("Maze no of edges = %d\nMaze no of path edges = %d\n\n", rmaze->no_of_edges, rmaze->no_of_path_edges);
    #endif

    #ifdef DEBUG
    for (int i = 0; i < rmaze->no_of_edges; i++)
        printf("edge[%d] = (%d, %d)\n", i, rmaze->edges[i].node1, rmaze->edges[i].node2);

    printf("\n");

    for (int i = 0; i < rmaze->no_of_path_edges; i++)
        printf("path_from_to[%d] = (%d, %d)\n", i, rmaze->path[i].fromnode, rmaze->path[i].tonode);

    printf("\n");

    #endif

    int screen_length   = 2 * MARGIN + rmaze->length * EDGE_LENGTH;
    int screen_width    = 2 * MARGIN + rmaze->width * EDGE_LENGTH;

    struct edge_coordinates * edges         = edges_to_coordinates(rmaze, EDGE_LENGTH, MARGIN);
    struct edge_coordinates * path_edges    = path_edges_to_coordinates(rmaze, EDGE_LENGTH, MARGIN);

    gMaze       = rmaze;
    gEdges      = edges;
    gPathEdges  = path_edges;

    // Get the environment variables set by the python script mazerun.py

    char * envvar = getenv("MAZERUN_DONT_SHOW_PATH");

    if (envvar && !strcmp(envvar, "TRUE"))
        dont_show_maze_path = 1;

    envvar = getenv("MAZERUN_SHOW_PATH_DOTTED");

    if (envvar && !strcmp(envvar, "TRUE"))
        show_maze_path_dotted = 1;


    // initialize the GLUT library
    glutInit(&argc, argv);

    /* set the initial display mode
     * single buffered window instead of double buffer
     * RGBA color mode window */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // set the initial window size, height and Width in pixels.
    glutInitWindowSize(screen_length, screen_width);

    /* desired position for window to create in future, measured 
     * in pixels from the upper left hand corner of the screen
     * with "x" increasing to the right and "y" increasing towards 
     * the bottom of the screen */
    glutInitWindowPosition(0, 0);

    // Create window with title
    glutCreateWindow("Maze");

    /* set the display callback for the current window.
     * When the normal plane for the window needs to be 
     * redisplayed, the display callback is called. */
    glutDisplayFunc(glutloop);

    opengl_setup_view_region(screen_length, screen_width);

    // enter the window event loop
    glutMainLoop();

    // Let OS free the memory at exit, since heap objects used in glutloop function
    //freeup_maze(rmaze);
}
