import random
import src.adt.disjointset as disjointset

# 4 X 4 Maze Example
#
#   start of maze
#    | ________________
# | 0  | 1  | 2  | 3  |
# | 4  | 5  | 6  | 7  |
# | 8  | 9  | 10 | 11 |
# |____|____|____| 15 |
#                  | End of maze
#
# Each node in the maze is represented as a element in the disjoint sets.
# Edge is represented as (node x, node y), which show that there is 
# line(edge) between node x and node y.

# We maintain
# S = set of sets of connected cells (disjoint sets)
# E = set of edges
# Maze = set of maze edges initially empty

class Maze:

    # Create a maze with 'm X n' nodes 
    def __init__(self, m, n):

        self.m = m
        self.n = n
        self.disjointset = disjointset.DisjointSet(m * n)

        self.create_edges() # Add all edges except boundary edges
        self.build_maze()   # Remove unwanted edges to get a maze
        self.solve_maze()   # Create a path between start node and last node

    def create_edges(self):

        # Maintain list of all edges as [[x1, y1], [x2, y2]...]
        # except boundary edges
        self.edges = []
        lastnode = self.m * self.n - 1

        # There are two types of edges for each node
        # Horizontal Edge   ___
        # Vertical Edge     |
        # Not adding the boundary edges
        for i in range(lastnode):

            # Add horizontal edge ___
            if (i + self.m) <= lastnode:
                self.edges.append([i, i + self.m])

            # Add Vertical edge |
            if (i + 1) % self.m != 0:
                self.edges.append([i, i + 1])

    # the edges list and maze_edges list together form the maze
    def build_maze(self):

        self.maze_edges = []
        # Invisible edges are the edges that are not part of the maze
        self.invisible_edges = []

        # A Maze is a set of nodes
        #
        # Initially we have totally disjoint sets (each node in seperate set),
        # so each node is blocked by edges from adjacent nodes.
        #
        # When we connect a node and its adjacent node by removing the edge between them,
        # the two disjoint sets containing the nodes, form union and are no longer disjoint.
        #
        # Goal here is to make repetitive unions between a random node and its adjacent node,
        # so it removes the edge between them, until all nodes are connected to each other, ie
        # the set is no longer disjoint.

        random.seed()

        total_edges = len(self.edges)

        while self.disjointset.no_of_sets() > 1:
            
            # Take a random edge
            random_no = random.randrange(len(self.edges))
            random_edge = self.edges[random_no]

            node1 = random_edge[0]
            node2 = random_edge[1]

            # if nodes around that edge are connected, delete the edge,
            # and add it to maze_edges (since it's part of the maze)
            # if not connected, connect them by union, and delete the edge
            # (since it's not part of the maze)

            del self.edges[random_no]

            print('Removed {} of {} edges'.format(total_edges - len(self.edges), total_edges), end='\r')

            root1 = self.disjointset.find(node1)
            root2 = self.disjointset.find(node2)

            if root1 != root2:
                self.disjointset.union(root1, root2)
                self.invisible_edges.append([node1, node2])
            else:
                self.maze_edges.append([node1, node2])

        print('')

    # Creates a path from start node (0) to end node (m * n - 1)
    #   start of maze
    #   __|__________________
    #   | 0  | 1    2    3  |
    #   |    |_________     |
    #   | 4    5    6    7  |
    #   |_________          |
    #   | 8    9  | 10 | 11 |
    #   |              |    |
    #   |_12___13___14_|_15_|
    #                     | End of maze
    # 
    # Here, the path is (0 => 4 => 5 => 6 => 7 => 11 => 15)
    #
    # maze_path contains this path in list format [[0, 4], [4, 5], [5, 6], [6, 7], [7, 11], [11, 15]]
    def solve_maze(self):

        traverse_edges = self.invisible_edges

        startnode = 0
        lastnode = self.m * self.n - 1

        # Use BFS to traverse the path
        # Start from the last node (m * n - 1) and traverse to the first node (node 0) 

        path                = [0 for i in range(lastnode + 1)]
        is_node_visited     = [False for i in range(lastnode + 1)]
        visit_queue         = []
        total_nodes_visited = 0

        visit_queue.append(lastnode)

        while visit_queue:

            visit_from_node = visit_queue.pop(0)
            is_node_visited[visit_from_node] = True

            total_nodes_visited += 1

            print('Visited {} of {} nodes'.format(total_nodes_visited, self.m * self.n), end='\r')

            i = 0

            while i < len(traverse_edges):

                edge = traverse_edges[i]

                if visit_from_node in edge:

                    del traverse_edges[i]
                    i -= 1

                    if visit_from_node == edge[0]:
                        visit_to_node = edge[1]

                    if visit_from_node == edge[1]:
                        visit_to_node = edge[0]

                    if not is_node_visited[visit_to_node]:
                        visit_queue.append(visit_to_node)
                        path[visit_to_node] = visit_from_node

                i += 1

        current_node = startnode
        self.maze_path = []

        # path[node] will give next node in the path
        # Ex: If path is (0 => 4 => 5 => 6 => 7 => 11 => 15) as mentioned above
        # path[0] = 4, path[4] = 5, ..., path[11] = 15

        while current_node != lastnode:

            self.maze_path.append([current_node, path[current_node]])
            current_node = path[current_node]

        print('')
