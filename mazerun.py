#!/usr/bin/python3

import os           # (environ) to access environment variables and (system) to call the executable
import platform     # Check for windows or linux
import argparse
import src.maze_as_csv as maze

# Minimum value for length and width of maze should be 4
def check_positive(value):
    if int(value) < 4:
        raise argparse.ArgumentTypeError("%s should be minimum 4" % value)
    return int(value)

def main():

    parser = argparse.ArgumentParser()

    # Take length and width as command line arguments for maze size
    # Ex: --create-data 10 20
    parser.add_argument('--create-data', nargs=2, type=check_positive, help='Enter length width, Ex: --create-data 10 20')
    
    # There are three ways of displaying the maze
    # 1. Display maze without path      (--dont-show-maze-path)
    # 2. Display maze with filled path  (default)
    # 3. Display maze with dotted path  (--dotted-maze-path)
    parser.add_argument('--dont-show-maze-path', action='store_true', help='Display maze without maze path')
    parser.add_argument('--dotted-maze-path', action='store_true', help='Display maze path as dotted rather than filled')
    
    args = parser.parse_args()

    # The 'mazerun' executable looks for environment variables
    # to configure the way of displaying
    if args.dont_show_maze_path:
        os.environ['MAZERUN_DONT_SHOW_PATH'] = 'TRUE'

    if args.dotted_maze_path:
        os.environ['MAZERUN_SHOW_PATH_DOTTED'] = 'TRUE'

    # If command line arg is --create-data
    if args.create_data:

        length = args.create_data[0]
        width = args.create_data[1]

        maze.create_maze_as_csv(length, width)

        print('Done')

        return

    # Launch the mazerun executable
    if platform.system() == 'Linux':
        os.system('./bin/mazerun')

    elif platform.system() == 'Windows':
        os.system("bin\mazerun.exe")

    else:
        print('Error: Could not run the executable from script.')
        # The executable should be launched from same prompt where script is exectued
        # to retain the environment variables
        print('Solution: run \'mazerun\' executable from bin directory in this prompt manually.')
        print('')

if __name__ == "__main__":
    main()