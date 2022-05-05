#!/bin/env python

import os
import shutil
import stat
import subprocess

target_list = [
    "asan",
    # "cfi",  #no expection
    # "fuzz", #undefined symbol, not build
    "scudo",
    "ubsan",
    # "shadowcallstack", # __aarch64__ 
    # "trace-pc-guard", # build_error
]

def check_out_dir():
    if os.path.exists("output") and os.path.isdir("output"):
        # print("Directory 'out' exist.")
        shutil.rmtree("output")
    else:
        os.mkdir("output")
        shutil.copy("runtest.sh",os.path.join(os.getcwd(),"output"))

def copy_output(tar):
    print("Start copying output executable...")
    tar_out = os.path.join(tar, "exe")
    copy_tar_path = os.path.join("output", tar)
    ret = os.path.exists(tar_out) and os.path.isdir(tar_out)
    if not ret:
        print(tar_out + " not exists")
    else:
        shutil.move(tar_out,copy_tar_path)

def start_build():
    check_out_dir()
    for target in target_list:
        tar_dir = os.path.join(os.getcwd(), target)
        build_file = os.path.join(tar_dir, "build.py")
        # build_file = 'build.py'
        if os.path.exists(build_file) and os.path.isfile(build_file):
            if os.access(build_file, os.X_OK):
                # print("[Permission Check]" + build_file + " X_OK")
                p = subprocess.Popen(build_file, cwd=tar_dir).wait()
                # if p == 0:
                copy_output(target)
            else:
                print(build_file + " permission denied.")
                break
        else:
            print(build_file + " not found.")
            break

if __name__ == "__main__":
    start_build()
