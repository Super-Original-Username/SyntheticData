import subprocess
import sys
import os

if __name__ == "__main__":
    # required args:
    # [1] root dir containing CSV/ and ch10/ directories
    # [2] path to ch10 generator executable
    # [3] number of forked process to generate (not functional atm)

    process_count = 1
    # exe_path = "G:\project-clones\SyntheticData\Debug\SynthCh10Gen.exe"



    if len(sys.argv) < 2:
        print("Path to working directory missing")
        exit()

    working_dir = sys.argv[1]
    csv_dir = os.path.join(working_dir,"CSV/")
    ch10_dir = os.path.join(working_dir,"ch10/")

    # print("working dir", working_dir)
    # print("csv dir", csv_dir)
    # print("ch10 dir", ch10_dir)

    if not os.path.exists(ch10_dir):
        os.makedirs(ch10_dir)

    if len(sys.argv) < 3:
        print("Missing path to generator executable")
        exit()

    exe_path = os.path.join(sys.argv[2])

    # print("exe path", exe_path)

    if len(sys.argv) >= 4:
        process_count = sys.argv[3]

    csv_list = os.listdir(csv_dir)

    # command = subprocess.run([exe_path, "-N", csv_dir, ch10_dir], capture_output=True)
    # print(command)
    # sys.stdout.buffer.write(command.stdout)
    # sys.stderr.buffer.write(command.stderr)
    # sys.exit(command.returncode)

    #print(csv_list)

    for file in csv_list:
        csv_path = os.path.join(csv_dir,file)
        csv_name = os.path.splitext(file)[0]
        print("Converting recording", csv_name, "to ch10")
        ch10_path = os.path.join(ch10_dir,(csv_name + '.ch10'))
        # print("csv path", csv_path,"\nch10 path",ch10_path)
        command = subprocess.run([exe_path, "-N", csv_path, ch10_path], capture_output=True)
        print(command)
        sys.stdout.buffer.write(command.stdout)
        sys.stderr.buffer.write(command.stderr)


