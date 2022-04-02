#!/usr/bin/env python3

import os
import sys
import shutil

arg_path=os.path.join(os.path.abspath(os.path.dirname(os.getcwd())),'script')
sys.path.append(arg_path)

from config_args import linux_args

def build():
    if os.path.exists("build") and os.path.isdir("build"):
        shutil.rmtree("build")
    while True:
        if not os.path.exists("build"):
            break
    os.mkdir("build")
    os.chdir("build")
    build_cmd = "cmake -G 'Ninja' " + (" ".join(linux_args))+ " .."
    print(build_cmd)
    os.system(build_cmd)
    os.system("ninja -f build.ninja")
    # os.chdir(os.path.abspath(os.path.dirname(os.getcwd())))
    # if os.path.exists("build") and os.path.isdir("build"):
    #     shutil.rmtree("build")


if __name__ == '__main__':
    build()


