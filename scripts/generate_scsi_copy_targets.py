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

import argparse
import os
import shutil


def copy_tree(src, dst):
    """Recursive Copy Directory"""
    copied_files = []
        
    for item in os.listdir(src):
        src_path = os.path.join(src, item)
        dst_path = os.path.join(dst, item)
        
        if os.path.isdir(src_path):
            os.makedirs(dst_path, exist_ok=True)
            # Recursive call and expand result list
            copied_files.extend(copy_tree(src_path, dst_path))
        else:
            try:
                if os.path.exists(dst_path):
                    os.remove(dst_path)
                shutil.copy2(src_path, dst_path)
                copied_files.append(dst_path)
                print(f"Copied: {src_path} -> {dst_path}")
            except Exception as e:
                print(f"Error copying {src} to {dst}: {str(e)}")
                raise

    return copied_files


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--src-dir", required=True)
    parser.add_argument("--out-dir", required=True)
    parser.add_argument("--stamp-file", required=True)
    args = parser.parse_args()

    # Ensure that the target root directory exists
    os.makedirs(args.out_dir, exist_ok=True)

    # Recursive copying and recording of results
    copied_files = [path for path in copy_tree(args.src_dir, args.out_dir) if path]

    # If no files are copied, still create the stamp file (ensure build continues)
    with os.fdopen(os.open(args.stamp_file, os.O_WRONLY | os.O_CREAT, mode=0o640), 'w') as f:
        f.write("DONE\n")
        f.write("\n".join(copied_files))  # Record copied files


if __name__ == "__main__":
    main()