import os
class FileGetter:
    """
    File getter utility class.
    """

    def get_files(path, tree=False):
        """
        Wraps the get_files_in_dir and get_files_in_tree methods
        :param directory: input directory
        :param tree: if True then use get_files_in_tree generator,
        otherwise use get_files_in_dir
        :yield: filename of a file in the given directory
        """
        if(tree):
            file_generator = FileGetter.get_files_in_tree(path)
        else:
            file_generator = FileGetter.get_files_in_dir(path)
        for file in file_generator:
            yield file

    def get_files_in_dir(directory):
        """
        Yields the filenames from the given directory
        :param directory: input directory
        :yield: filename of a file in the given directory
        """
        # save cwd
        tmpdir = os.getcwd()
        # go to given directory
        os.chdir(directory)
        for file in os.listdir(directory):
            if not os.path.isdir(file):
                # yield files only, not directories
                yield file
        # restore cwd
        os.chdir(tmpdir)

    def get_files_in_tree(directory):
        """
        Yields the filenames from the tree that starts from the initial directory
        :param directory: starting directory
        :yield: filename of a file in the given directory tree
        """
        # save cwd
        tmpdir = os.getcwd()
        # go to given directory
        # os.chdir(starting_directory)
        for dir_name, subdir_list, file_list in os.walk(directory):
            for file in file_list:
                yield os.path.join(dir_name, file)
        # restore cwd
        os.chdir(tmpdir)