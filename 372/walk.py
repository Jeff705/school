import sys
import stack

def walk(dir):
  stk = stack.stack()
  files = []
  stk.push(dir)
  while not stk.empty():
    dirToSearch = stk.top()
    stk.pop()

if __name__ == "__main__":
  if len(sys.argv) != 2:
    print "usage:", sys.argv[0], "<dir to search>"
  walk(sys.argv[1])
