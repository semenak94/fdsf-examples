#
# Скрипт сборки проекта
#
import os
import sys
import subprocess

def main():
    build_dir = os.getcwd() + "/build"
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
    os.chdir(build_dir)
    # TODO: function run_subprocess_sync
    subprocess.call(["cmake", ".."], shell=True)
    subprocess.call(["cmake", "--build", ".", "--config","-DCMAKE_BUILD_TYPE=Release"])

main()
