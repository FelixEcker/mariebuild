#!/usr/bin/python
# This python buildscript is intended to build mariebuild
# if there is no build already present,

# CONFIGURATION

flags = "-Wall -ggdb"
libs = "-Llib/ -lmcfg"
target = "mb"
source_dir = "src/"
obj_dir = "out/"
includes = "-Iinclude/ -Isrc/"
sources = [
            "mb_utils.c",
            "mb_execute.c",
            "mb.c"
          ]

# BUILDING

import os

def do_cmd(cmd):
    print("=> "+cmd)
    os.system(cmd)

def main():
    objs = ""
    for source in sources:
        do_cmd("gcc {0} {1} -c -o {2} {3}".format(flags, includes, 
                                                   obj_dir+source+".o",
                                                   source_dir+source))
        objs += obj_dir+source+".o "

    do_cmd("gcc {0} -o {1} {2} {3}".format(flags, target, objs, libs))

if __name__ == "__main__":
    main()
