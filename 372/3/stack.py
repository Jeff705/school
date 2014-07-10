#!/usr/bin/python
# A stack class written by B. Malloy, Oct 21, 2009

class stack(object):
  def __init__(self):
    self.stk = []
  def push(self, item):
    self.stk.append(item)
  def pop(self):
    self.stk.pop()
  def top(self):
    return self.stk[len(self.stk)-1]
  def size(self):
    return len(self.stk)
  def empty(self):
    return len(self.stk) == 0

def empty_stack(s):
  print "The stack is:",
  while not s.empty():
    print s.top(),
    s.pop()
  print

if __name__ == "__main__":
  stk = stack()
  stk.push(4)
  stk.push(9)
  print stk.size()
  print stk.top()
  stk.push(99)
  print stk.size()
  print stk.top()
  empty_stack(stk)
  print stk.size()
  stk.push(99)
  print stk.size()
  print stk.top()
  
