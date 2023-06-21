target = "mb"
source_dir = "src/"
obj_dir = "out/"
includes = "include/"
sources = [
            "mb_parse.c",
            "mb_execute.c",
            "mb.c"
          ]

import os

print("I BEG YOU TO IGNORE THAT I AM USING A PYTHON FILE TO BUILD C,")
print("I JUST FUCKING HATE MAKEFILES AND ALL OTHER BUILD SYSTEMS,")
print("THAT IS THE EXACT REASON WHY IM WRITING THIS!")

objs = ""
for source in sources:
    cmd = "gcc -Wall -I{0} -c -o {1} {2}".format(includes, obj_dir+source+".o",
                                                     source_dir+source)
    objs += obj_dir+source+".o "
    print("=> "+cmd)
    os.system(cmd)

cmd = "gcc -Wall -o {0} {1}".format(target, objs)
print("=> "+cmd)
os.system(cmd)
