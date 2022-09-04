from Interpreter import Interpreter
from multiprocessing import Pool
import glob,os
import subprocess

TEST_DIR = "./tests"
SCRIPTS_DIR = "./codes"
OUT_DIR = "./out"


def compile_all(c):
    os.system(f"g++ -std=c++17 {c} -o " + str(OUT_DIR+'/'+c.split('\\')[1].split('.')[0]))
    return str(OUT_DIR+'/'+c.split('\\')[1].split('.')[0])

def main():
    tests = glob.glob(TEST_DIR+"/*.in")
    codes = glob.glob(SCRIPTS_DIR+"/*.cpp")
    compiled = []
    #compile
    with Pool() as pool:
        out = pool.imap_unordered(compile_all,codes)
        for o in out:
            compiled.append(o)
    #test
    for i in compiled:
        print(i + " < ess")
        out = subprocess.check_output([i, '< ess'])
        print(out)






if __name__ == '__main__':
    main()