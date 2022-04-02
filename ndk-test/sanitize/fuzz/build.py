#!/bin/env python

import os
import sys
import subprocess
import shutil

pre_dir = os.path.abspath(os.path.dirname(os.getcwd()))
config_dir = os.path.join(os.path.dirname(pre_dir),'script')
sys.path.append(config_dir)

from config_args import linux_args

# Add libFuzz compile options
fuzz_flags = [ "-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=fuzzer" ]

build_args=[]
build_args.extend(linux_args)
# build_args.extend(fuzz_flags)

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
    subprocess.Popen(build_cmd,cwd='build',shell=True).wait()
    subprocess.Popen('make',cwd='build',shell=True).wait()
    rm_build()

if __name__ == '__main__':
    build_linux()