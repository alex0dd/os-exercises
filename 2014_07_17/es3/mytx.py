#!/usr/bin/env python3
import os
import sys


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


def dump_to_file(filename, gen):
    """
    Writes the yielded information inside the file
    :param filename: output file's name
    :param gen: line generator(what to write on every line)
    :return None
    """
    # open the file in write mode
    with open(filename, 'w') as file:
        # foreach line from the generator
        for line in gen:
            # write the line to the file
            file.write(line+"\n")


def file_generator(files_dict):
    # foreach file and it's length
    for file, length in files_dict.items():
        # yield it's string representation, "length filename"
        yield "{} {}".format(length, file)


def get_file_length(filename):
    with open(filename, 'r') as file:
        length = 0
        for line in file:
            length += len(line)
        return length

if __name__ == "__main__":
    # check if a directory path is passed as argument
    if len(sys.argv) > 1:
        dirname = sys.argv[1]
        # dictionary containing filenames as keys and their length as values
        files_dict = {}
        # check if the path is a directory
        if os.path.isdir(dirname):
            # foreach file in the given directory
            for filename in get_files_in_dir(dirname):
                files_dict[filename] = get_file_length(filename)
            # output the file, the file will be named as dirname.xnumber_of_files
            dump_to_file("{}.t{}".format(os.path.basename(dirname), len(files_dict)), gen=file_generator(files_dict))
        else:
            print("The given path is not a directory: {}".format(dirname))
    else:
        print("No directory was given.")
