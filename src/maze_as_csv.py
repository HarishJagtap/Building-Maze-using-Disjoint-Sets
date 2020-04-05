import csv
import src.adt.maze as maze_adt

def create_maze_as_csv(length, width):

    maze = maze_adt.Maze(length, width)

    # Generate 3 csv files
    # 1. maze_size.csv  ==> columns (length, width)
    # 2. maze_edges.csv ==> columns (Node 1, Node 2)
    # 3. maze_path.csv  ==> columns (From node, To node)

    with open("result/maze_size.csv", 'w') as csvfile: 
        csvwriter = csv.writer(csvfile) 
        csvwriter.writerow(['Length', 'Width'])
        csvwriter.writerow([length, width])

    with open("result/maze_edges.csv", 'w') as csvfile: 
        csvwriter = csv.writer(csvfile) 
        csvwriter.writerow(['Node 1', 'Node 2'])
        csvwriter.writerows(maze.edges)
        csvwriter.writerows(maze.maze_edges)

    with open("result/maze_path.csv", 'w') as csvfile: 
        csvwriter = csv.writer(csvfile) 
        csvwriter.writerow(['From node', 'To node'])
        csvwriter.writerows(maze.maze_path)
