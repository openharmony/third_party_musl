#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2025 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import os
import sys
import argparse
import subprocess
from contextlib import contextmanager


@contextmanager
def cwd(path):
    oldpwd = os.getcwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(oldpwd)


def build_sym_script(args):
    if not os.path.exists(args.lib_dir):
        print(f'adlt_tools build_sym_script: input lib dir {args.lib_dir} is not exists')
        return False

    with cwd(args.lib_dir):
        with open('adlt_build_syms.sh', 'w') as f:
            f.write(
                f'work_dir=`pwd`\n'
                f'cd {args.target_lib_dir}\n'
                f'for i in libadlt*.sh ; do\n'
                f'sh $i;\n'
                f'done\n'
                f'cd $work_dir\n'
            )

    return True


def build_sym_links(args):
    if not os.path.exists(args.lib_dir):
        print(f'adlt_tools build_sym_links: input lib dir {args.lib_dir} is not exists')
        return False

    with cwd(args.lib_dir):
        if not os.path.exists(args.adlt_lib):
            print(f'adlt_tools build_sym_links: input adlt lib {args.adlt_lib} is not exists')
            return False

        orig_libs = []
        for orig_lib in args.orig_lib_list:
            orig_lib = ''.join(orig_lib)
            orig_libs.append(orig_lib)
            if not os.path.exists(orig_lib):
                print(f'adlt_tools build_sym_links: input adlt orig lib {orig_lib} is not exists')
                return False

        orig_lib_dir = 'adlt_orig_libs'
        os.makedirs(orig_lib_dir, exist_ok=True)

        for orig_lib in orig_libs:
            os.rename(orig_lib, f'{orig_lib_dir}{os.sep}{orig_lib}')

        orig_libs = ' '.join(orig_libs)
        with open(args.adlt_lib.replace('.so', '.sh'), 'w') as f:
            f.write(
                f'work_dir=`pwd`\n'
                f'cd {args.target_lib_dir}\n'
                f'for i in {orig_libs} ; do\n'
                f'ln -s {args.adlt_lib} $i ;\n'
                f'done\n'
                f'cd $work_dir\n'
            )

    return True


def get_build_id(args):
    if not os.path.exists(args.tool_path):
        print(f'adlt_tools get_build_id: tool path {args.tool_path} is not exists')
        return False

    files = []
    for file in args.file_list:
        file = ''.join(file)
        files.append(file)
        if not os.path.exists(file):
            print(f'adlt_tools get_build_id: input file {file} is not exists')
            return False

    with open(args.output_file, 'w') as f:
        for file in files:
            ret = subprocess.run([args.tool_path, '-n', file],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding='utf-8')

            if ret.returncode != 0:
                print(f'adlt_tools get_build_id: read file {file} failed ret: {ret.returncode}')
                if ret.stderr:
                    print(ret.stderr)
                return False

            for line in ret.stdout.splitlines():
                if 'Build ID:' in line:
                    f.write(f'{line}\n')
                    break

    return True


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers()

    subparser = subparsers.add_parser('build_sym_script')
    subparser.set_defaults(func=build_sym_script)
    subparser.add_argument('--lib-dir', type=str, required=True)
    subparser.add_argument('--target-lib-dir', type=str, required=True)

    subparser = subparsers.add_parser('build_sym_links')
    subparser.set_defaults(func=build_sym_links)
    subparser.add_argument('--lib-dir', type=str, required=True)
    subparser.add_argument('--target-lib-dir', type=str, required=True)
    subparser.add_argument('--adlt-lib', type=str, required=True)
    subparser.add_argument('--orig-lib-list', type=list, nargs='+', required=True)

    subparser = subparsers.add_parser('get_build_id')
    subparser.set_defaults(func=get_build_id)
    subparser.add_argument('--tool-path', type=str, required=True)
    subparser.add_argument('--output-file', type=str, required=True)
    subparser.add_argument('--file-list', type=list, nargs='+', required=True)

    args = parser.parse_args()
    if not args.func(args):
        sys.exit(1)


if __name__ == '__main__':
    main()