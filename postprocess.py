from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import sys

def main(argv):
  filename = argv[1]
  data = []
  with open(filename, 'rb') as f:
    header1 = f.readline()
    header2 = f.readline()
    line = str(f.readline(), 'utf-8')
    while line:
      i = int('0x' + line[:-1], 16)
      data.append(i)
      line = str(f.readline(), 'utf-8')

  # sys.stdout.buffer.write(bytes(header1, encoding='utf-8'))
  # sys.stdout.buffer.write(bytes(header2, encoding='utf-8'))
  sys.stdout.buffer.write(header1)
  sys.stdout.buffer.write(header2)
  sys.stdout.buffer.write(bytes(data))

if __name__ == '__main__':
  main(sys.argv)
