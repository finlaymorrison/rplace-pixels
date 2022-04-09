The project can be compiled without any use of a build system due to its simplicity, it is as simple as:

g++ -O3 main.cpp -o prog

An already compiled file for AMD64 Linux is already in the repository named 'prog'. If this binary will not work with your computer there are several ways to set up a C++ environment on your PC, for windows you can either download Visual Studio which comes with a compiler, or download the mingw compiler for windows.

The tool is relatively simple to run. It searches a given directory for all files and then reads them in, eliminates users if they did not place pixels in a given area and then saves the new dataset into a new file. THe program can be called from a command line as follows.

./prog {source folder} {output csv file} {top left x} {top left y} {bottom right x} {bottom right y}

For a specific example,

./prog data save.csv 100 250 150 300

This will search through all input csv files in the folder named "data", and eliminate all pixels placed by users who did not place any pixel in the rectangle defined by the points (100,250) and (150, 300), the pruned dataset will then be saved in the save.csv folder.

This newly obtained dataset can then be run again with another rect to further eliminate users.