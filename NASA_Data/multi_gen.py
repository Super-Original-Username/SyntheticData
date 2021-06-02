import subprocess
from multiprocessing.dummy import Pool
from functools import partial
import sys
import os

if __name__ == "__main__":
    # required args:
    # [1] root dir containing CSV/ and ch10/ directories
    # [2] path to ch10 generator executable
    # [3] number of forked process to generate (not functional atm)

    process_count = 1

    if len(sys.argv) < 2:
        print("Path to working directory missing")
        exit()

    working_dir = sys.argv[1]
    csv_dir = os.path.join(working_dir,"CSV/")
    ch10_dir = os.path.join(working_dir,"ch10/")

    if not os.path.exists(ch10_dir):
        os.makedirs(ch10_dir)

    if len(sys.argv) < 3:
        print("Missing path to generator executable")
        exit()

    exe_path = os.path.join(sys.argv[2])

    if len(sys.argv) >= 4:
        process_count = int(sys.argv[3])

    # initialize worker pool with max allowed workers

    pool = Pool(processes=process_count)

    csv_list = os.listdir(csv_dir)

    command_list = []

    for file in csv_list:
        csv_path = os.path.join(csv_dir,file)
        csv_name = os.path.splitext(file)[0]
        ch10_path = os.path.join(ch10_dir,(csv_name + '.ch10'))
        command = f"{exe_path} -N {csv_path} {ch10_path}"
        command_list.append(command)

    for i, ret in enumerate(pool.imap(partial(subprocess.run,capture_output=True),command_list)):
        print("running command:",i)
        if ret != 0:
            print("{} command failed: {}".format(i,ret))
        
