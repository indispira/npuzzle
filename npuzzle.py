import ctypes
import argparse

from ctypes import *
from ctypes.util import find_library

# C Structure
class Node(ctypes.Structure):
  _fields_ = [
    ('g', c_uint),
    ('h', c_uint),
    ('f', c_uint),
    ('hash', c_uint),
    ('grid', ctypes.POINTER(ctypes.POINTER(c_uint))),
    # ('next', ctypes.POINTER(Node)),
  ]

class Puzzle(ctypes.Structure):
  _fields_ = [
    ('size', c_uint),
    ('start', ctypes.POINTER(ctypes.POINTER(c_uint))),
    ('goal', ctypes.POINTER(Node)),
  ]

# C functions
_libnpuzzle = ctypes.CDLL("./npuzzle.so")
_libnpuzzle.print_grid.argtypes = [ctypes.POINTER(ctypes.POINTER(c_uint)), c_uint]
_libnpuzzle.create_goal.argtypes = [ctypes.POINTER(Puzzle)]
_libnpuzzle.generate_hash.argtypes = [ctypes.POINTER(ctypes.POINTER(c_uint)), c_uint]
_libnpuzzle.generate_hash.restype = c_uint
_libnpuzzle.heuristic_manhattan_distance.argtypes = [ctypes.POINTER(ctypes.POINTER(c_uint)), ctypes.POINTER(ctypes.POINTER(c_uint)), c_uint]
_libnpuzzle.heuristic_manhattan_distance.restype = c_uint
_libnpuzzle.astar.argtypes = [ctypes.POINTER(Puzzle)]
_libnpuzzle.astar.restype = c_uint

# Argument parser
parser = argparse.ArgumentParser(prog='npuzzle.py', description='N-puzzle project')
parser.add_argument('--file', metavar='FILE', required=True, type=str, help='Path to the puzzle file')
parser.add_argument('--heuristic', metavar='HEURISTIC', choices=['unknow', 'chosen'], default='unknow', type=str, help='Heuristic used for solve the puzzle')
args = parser.parse_args()

# Python functions
def parser_error(text):
  if text == '':
    print("Incorrect input file")
  else:
    print(text)
  exit(0)

# Read infile and check the data
try:
  with open(args.file) as infile:
    # Read infile abd extract the data
    file = infile.read().splitlines()
    data_file = []
    for i in range(len(file)):
      comment = file[i].find('#')
      if comment != -1:
        file[i] = file[i][:comment]
      data_file.append(file[i].split(' '))

  # Create the python list
  puzzle = {}
  puzzle['size'] = None
  data = []
  values = []
  index_line = 0
  for element in data_file:
    line = []
    for cell in element:
      if cell == '':
        continue
      if puzzle['size'] is None:
        puzzle['size'] = int(cell)
        break
      line.append(int(cell))
      values.append(int(cell))
    if len(line):
      data.append(line)

  # Check the data (values and length of lines)
  max_value = puzzle['size'] * puzzle['size'] - 1
  values.sort()
  if len(values) != max_value + 1:
    parser_error('Incorrect number of values')
  if max(values) != max_value or min(values) != 0:
    parser_error('Incorrect value inside the file')
  prev = -1
  for value in values:
    if value != prev + 1:
      parser_error('Incorrect value inside the file')
    prev = value
  for line in data:
    if len(line) != puzzle['size']:
      parser_error('Incorrect length of a line of puzzle')

  # Allocate memory for the puzzle
  puzzle['start'] = (ctypes.POINTER(ctypes.c_uint) * (puzzle['size'] + 1))()
  puzzle['start'][puzzle['size']] = None
  for i, line in enumerate(data):
    puzzle['start'][i] = (ctypes.c_uint * puzzle['size'])()
    for j, value in enumerate(line):
      puzzle['start'][i][j] = value
  puzzle['goal'] = None
  c_puzzle = Puzzle(**puzzle)

except:
  print("ERROR")
  parser_error('')

_libnpuzzle.print_grid(c_puzzle.start, c_puzzle.size)
_libnpuzzle.create_goal(ctypes.pointer(c_puzzle), c_puzzle.size)
_libnpuzzle.print_grid(c_puzzle.goal.contents.grid, c_puzzle.size)
print(_libnpuzzle.generate_hash(c_puzzle.start, c_puzzle.size))
print(_libnpuzzle.generate_hash(c_puzzle.goal.contents.grid, c_puzzle.size))
# print(_libnpuzzle.heuristic_manhattan_distance(c_puzzle.start, c_puzzle.goal.contents.grid, c_puzzle.size))
if _libnpuzzle.astar(ctypes.pointer(c_puzzle)):
  print("Success")
else:
  print("Unsolvable...")




