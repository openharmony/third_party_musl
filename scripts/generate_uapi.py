#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (c) 2025 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import subprocess
import argparse
import shutil


def check_uapi_dir_exists(uapi_dir):
    """Check if the UAPI directory exists"""
    return os.path.isdir(uapi_dir)


def make_uapi_headers(kernel_dir, target_cpu, output_dir):
    """Generate UAPI header file"""
    uapi_dir = os.path.join(output_dir, "usr/include")
    
    try:
        # Clean up and generate header files
        subprocess.run(['rm', '-rf', str(uapi_dir)], check=True)
        cmd = [ 'make',
                '-C', 
                str(kernel_dir),
                '-sj',
                'headers', 
                'O=%s' % output_dir,
                'ARCH=%s' % target_cpu ]
        subprocess.run(cmd, check=True)

        # Special handling of individual files
        ashmem_src = os.path.join(kernel_dir, "drivers/staging/android/uapi/ashmem.h")
        ashmem_dst = os.path.join(uapi_dir, "linux/ashmem.h")
        if os.path.exists(ashmem_src):
            subprocess.run(['cp', '-f', str(ashmem_src), str(ashmem_dst)], check=True)
        
        input_h = os.path.join(uapi_dir, "linux/input.h")
        if os.path.exists(input_h):
            input_cmd = [ 'sed',
                           '-i',
                           r'/#define _INPUT_H/i#define _UAPI_INPUT_H',
                           str(input_h) ]
            subprocess.run(input_cmd, check=True)
        
        socket_h = os.path.join(uapi_dir, "linux/socket.h")
        if os.path.exists(socket_h):
            sockaddr_cmd = [ 'sed',
                             '-i',
                             r'/struct __kernel_sockaddr_storage/i#define sockaddr_storage __kernel_sockaddr_storage',
                             str(socket_h) ]
            subprocess.run(sockaddr_cmd, check=True)
        
        return True
    except subprocess.CalledProcessError as e:
        print("Failed to make UAPI headers: {e}")
        return False


def list_uapi_files(uapi_dir, exclude_pattern):
    """List UAPI files and filter them"""
    try:
        list_files = subprocess.run(['ls', str(uapi_dir)], capture_output=True, text=True, check=True)
        result = subprocess.run(['grep', '-vE', str(exclude_pattern)], 
            input=list_files.stdout, capture_output=True, text=True, check=True)
        return result.stdout.strip().split('\n')
    except subprocess.CalledProcessError as e:
        print(f"Failed to list UAPI files: {e}")
        return []


def copy_filtered_uapi_dirs(uapi_dir, final_uapi_dir, output_uapi):
    copied_files = []
    try:
        os.makedirs(output_uapi, exist_ok=True)

        for item in uapi_dir:
            src_path = os.path.join(final_uapi_dir, item)
            dst_path = os.path.join(output_uapi, item)

            if os.path.isdir(src_path):
                for root, dirs, files in os.walk(src_path):
                    for file in files:
                        src_file = os.path.join(root, file)
                        rel_path = os.path.relpath(src_file, final_uapi_dir)
                        dst_file = os.path.join(output_uapi, rel_path)
                        
                        os.makedirs(os.path.dirname(dst_file), exist_ok=True)
                        shutil.copy2(src_file, dst_file)
                        copied_files.append(dst_file)
                        print(f"Copied: {rel_path} -> {dst_file}")
            else:
                # Processing individual files
                os.makedirs(os.path.dirname(dst_path), exist_ok=True)
                shutil.copy2(src_path, dst_path)
                copied_files.append(dst_path)
                print("Copied: {item} -> {dst_path}")
    except Exception as e:
        print(f"Error copying: {e}")

    return copied_files


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--musl-uapi-dir", required=True)
    parser.add_argument("--musl-linux-kernel-dir", required=True)
    parser.add_argument("--target-cpu", required=True)
    parser.add_argument("--uapi-file-list", required=True)
    parser.add_argument("--output-uapi", required=True)
    args = parser.parse_args()

    # Determine the source of UAPI
    if not check_uapi_dir_exists(args.musl_uapi_dir):
        print("UAPI directory not found, generating from kernel...")
        kernel_out_dir = os.path.join(args.musl_linux_kernel_dir, "make_output")
        success = make_uapi_headers(
            args.musl_linux_kernel_dir,
            args.target_cpu,
            kernel_out_dir
        )
        if not success:
            exit(1)
        uapi_from = "make"
        final_uapi_dir = os.path.join(kernel_out_dir, "usr/include")
    else:
        print("Using existing UAPI directory")
        uapi_from = "local"
        final_uapi_dir = args.musl_uapi_dir

    exclude_pattern = "^asm$|^scsi$" if uapi_from == "make" else "^asm-arm$|^asm-arm64$|^scsi$"

    # List UAPI
    uapi_files = list_uapi_files(final_uapi_dir, exclude_pattern)

    copied_files = copy_filtered_uapi_dirs(uapi_files, final_uapi_dir, args.output_uapi)

    # Write output file
    with os.fdopen(os.open(args.uapi_file_list, os.O_WRONLY | os.O_CREAT, mode=0o640), 'w') as f:
        f.write(f"UAPI_FROM:{uapi_from}\n")
        f.write(f"UAPI_DIR:{final_uapi_dir}\n")
        f.write("UAPI_FILES:\n")
        f.write("\n".join(copied_files) + "\n")


if __name__ == "__main__":
    main()