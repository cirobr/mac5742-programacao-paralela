"""
run(["ls", "-l"])
run(["./ss200ms", "5", "3", "2", "0", "1"])

prog = ["./ss200ms"]
args = ["5", "3", "2", "0", "1"]
# run(prog + args)
"""

import sys

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

print(sys.argv[0])
print(sys.argv[1])
print(sys.argv[2])