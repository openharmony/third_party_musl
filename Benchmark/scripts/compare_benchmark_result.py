#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2023 Huawei Device Co., Ltd.
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
#/

import argparse
import re

g_before_data = {}
g_after_data = {}
g_all_cases = []


def get_format(before_data, after_data, pct, fluctuation_range):
    deg_format = "\033[1;31m|{:60}|{:20}|{:20}|{:20}\033[0m|"
    opt_format = "\033[1;32m|{:60}|{:20}|{:20}|{:20}\033[0m|"
    equal_format = "\033[1;37m|{:60}|{:20}|{:20}|{:20}\033[0m|"
    if pct <= fluctuation_range:
        return equal_format
    if before_data > after_data:
        return opt_format
    else:
        return deg_format


def read_file(file_path, is_before):
    bench_pattern = re.compile(r'''
        ^([\S]+)   # case name
        [^0-9]+
        ([0-9,.]+) # time
        [^0-9]+
        ([0-9|.]+) # cpu
        [^0-9]+
        ([0-9|.]+) # iters
        .*''', re.VERBOSE)

    with open(file_path, 'r') as f:
        for line in f.readlines():
            if line.startswith("BM_"):
                m = bench_pattern.match(line)
                if m:
                    case, time, cpu, iters = m.groups()
                    if g_all_cases.count(case) == 0:
                        g_all_cases.append(case)
                    result = [float(time), float(cpu), float(iters)]
                    if is_before:
                        g_before_data[case] = result
                    else:
                        g_after_data[case] = result
                else:
                    print("match error")


def print_header(args):
    header_format = "\033[1;37m|{:60}|{:20}|{:20}|{:20}|\033[0m"
    print("\033[1;34m[Notice]:\033[0m")
    print("file before optimization: " + args.before)
    print("file after optimization: " + args.after)
    print("allowed fluctuation range: " + args.range + "%")
    print("\033[1;32mgreen is better, \033[0m" + "\033[1;31mred is worse, \033[0m" + "\033[1;37mwhite is equal.\033[0m")
    print("\033[1;34m[Compare Result]:\033[0m")
    print(header_format.format("case", "before(ns)", "after(ns)", "(before - after / before)"))
    print(header_format.format("----", "----", "----", "----"))


def out_results(args):
    print_header(args)
    for case in g_all_cases:
        before_data = g_before_data[case][0] if case in g_before_data.keys() else 0
        after_data = g_after_data[case][0] if case in g_after_data.keys() else 0
        pct = 0 if before_data == 0 else round((abs(before_data - after_data) / before_data) * 100, 2)
        sign = "+" if (before_data - after_data) > 0  else "-"
        output_format = get_format(before_data, after_data, pct, float(args.range))
        signed_pct = "{}{}{}".format(sign, str(pct), "%")
        print(output_format.format(str(case), str(before_data), str(after_data), signed_pct))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-b', '--before', type=str, help='file before optimization')
    parser.add_argument('-a', '--after', type=str, help='file after optimization')
    parser.add_argument('-r', '--range', type=str, help='allowed fluctuation range')

    args = parser.parse_args()
    read_file(args.before,  1)
    read_file(args.after, 0)
    out_results(args)


if __name__ == '__main__':
    main()