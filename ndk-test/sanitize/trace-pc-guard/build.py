#!/usr/bin/python

import os
import subprocess
import sys
import shutil

pre_dir = os.path.abspath(os.path.dirname(os.getcwd()))
config_dir = os.path.join(os.path.dirname(pre_dir),'script')
sys.path.append(config_dir)

from config_args import linux_args

# Add asan compile options
asan_flags = [ "-DCMAKE_CXX_FLAGS=-fsanitize-coverage=trace-pc-guard,indirect-calls" ]

build_args=[]
build_args.extend(linux_args)
build_args.extend(asan_flags)

def rm_build():
    if os.path.exists("build") and os.path.isdir("build"):
        shutil.rmtree("build")
    while True:
        if not os.path.exists("build"):
            break

def build_linux():
    rm_build()
    os.mkdir("build")
    build_cmd = "cmake " + (" ".join(build_args))+ " .."
    print(build_cmd)
    res1 = subprocess.Popen(build_cmd,cwd='build',shell=True).wait()
    # print(res1)
    res2 = subprocess.Popen('make',cwd='build',shell=True).wait()
    # print(res2)
    rm_build()

if __name__ == '__main__':
    build_linux()
