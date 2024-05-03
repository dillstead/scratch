import os
import sys
import random
import subprocess
import hashlib
from itertools import permutations

def test(prog, path, size, ring_entries, block_sz):
    write_file(path, size)
    result = copy_file(path, ring_entries, block_sz) and verify_file(path)
    run_cmd(f'rm {path} ./copy')
    return result

def run_cmd(command):
    try:
        subprocess.run(command, shell=True, check=True)
        return True
    except subprocess.CalledProcessError:
        return False
    
def write_file(path, size):
    with open(path, 'wb') as fout:
        for _ in range(size):
            byte = random.randint(0, 255)
            fout.write(bytes([byte]))

def copy_file(path, ring_entries, block_sz):
    return run_cmd(f'./io_uring_cp {path} ./copy {ring_entries} {block_sz}')

def hash_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as f:
        for data in iter(lambda: f.read(1024), b''):
            digest.update(data)
    return digest.hexdigest()

def verify_file(path):
    return hash_file(path) == hash_file('./copy')

os.chdir('..')
for ring_entries, block_sz in  list(permutations([i for i in range(1, 7)], 2)):
    if not test('./io_uring.cp', './file.txt', 5, ring_entries, block_sz):
        print(f'{ring_entries}, {block_sz} FAILED')
