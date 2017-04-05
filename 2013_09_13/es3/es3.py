#!/usr/bin/env python3

import os
import sys


def read_and_count_file(count_list, filename):
    line_count = 0
    # open a file without worrying about closing it
    with open(filename, 'r') as file:
        # foreach line in file
        for line in file:
            # expand the list if the current line count exceeds the list's size
            if len(count_list) <= line_count:
                count_list.append(0)
            # foreach character on the line
            for _ in line:
                count_list[line_count] += 1
            line_count += 1


def print_list(lst):
    for i, item in enumerate(lst):
        print(str(i+1)+" "+str(item))


def get_files_in_dir(directory):
    # save cwd
    start_directory = os.getcwd()
    # go to given directory
    os.chdir(directory)
    for file in os.listdir(directory):
        if not os.path.isdir(file):
            # yield files only, not directories
            yield file
    # restore cwd
    os.chdir(start_directory)

if __name__ == "__main__":
    # check if a directory path is passed as argument
    if len(sys.argv) > 1:
        dirname = sys.argv[1]
        # check if the path is a directory
        if os.path.isdir(dirname):
            count_list = []
            # foreach file in given directory
            for filename in get_files_in_dir(dirname):
                # read file and manipulate the countList
                read_and_count_file(count_list, filename)
            print_list(count_list)
        else:
            print("Given path is not a directory: "+dirname)
    else:
        print("No path was given.")
