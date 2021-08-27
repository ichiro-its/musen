# Copyright (c) 2021 ICHIRO ITS
#
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.

from difflib import unified_diff
from glob import glob
import os
import subprocess
import sys


fail = False
dir_path = os.path.dirname(os.path.realpath(__file__))
config_path = os.path.join(dir_path, 'config.cfg')


def check_file(file: str):
    global fail, config_path

    original = subprocess.run(['cat', file], capture_output=True)
    formatted = subprocess.run(['uncrustify', '-c', config_path, '-f', file], capture_output=True)
    difflines = list(unified_diff(original.stdout.decode("utf8").splitlines(), formatted.stdout.decode("utf8").splitlines()))
    if difflines:
        fail = True
        for line in difflines:
            print(line)


for i in range(1, len(sys.argv)):
    for ext in ['*.cpp', '*.hpp', '*.tpp']:
        glob_path = os.path.join(sys.argv[i], '**', ext)
        files = glob(glob_path, recursive=True)
        for file in files:
            check_file(file)

if fail:
    sys.exit(1)
