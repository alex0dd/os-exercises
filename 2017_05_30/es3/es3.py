#!/usr/bin/env python3
from file_getter import FileGetter
import hashlib
import os
import sys

def compute_md5_hash(path):
    BUF_SIZE = 1024
    # initialize the md5 instance
    md5 = hashlib.md5()
    # open the file in read-binary mode
    with open(path, 'rb') as f:
        while True:
            # read BUF_SIZE amount of data
            data = f.read(BUF_SIZE)
            if not data:
                break
            # update the md5 buffer
            md5.update(data)
    return str(md5.hexdigest())

def print_result(files_list):
    for filename in files_list:
        print(filename)

if __name__ == "__main__":
    # check if a directory path is passed as argument
    if len(sys.argv) > 2:
        filename = sys.argv[1]
        main_file_hash = compute_md5_hash(filename)
        print("{} hash: {}".format(filename, main_file_hash))
        if(os.path.exists(filename)):
            dirname = sys.argv[2]
            files_list = [filename]
            # check if the path is a directory
            if os.path.isdir(dirname):
                # foreach file in the given directory
                for filename in FileGetter.get_files(dirname, True):
                    # check if the file is a regular file
                    if os.path.isfile(filename):
                        filehash = compute_md5_hash(filename)
                        if filehash == main_file_hash:
                            files_list.append(filename)
                print_result(files_list)
            else:
                print("The given path is not a directory: {}".format(dirname))
        else:
            print("The given file does not exist: {}".format(filename))
    else:
        print("No directory and/or file was given.")
