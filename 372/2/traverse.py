import os
import sys

class Traverse(object):
  def __init__(self, sd):
    self.fileList = []
    self.startDir = sd

  def getFiles(self):
    return self.fileList

  def search(self, part):
    stk = []
    stk.append(self.startDir)
    dirList = []

    while stk:
      dirToSearch = stk.pop()
      thisDirectory = os.listdir(dirToSearch)

      for i in thisDirectory:
        toPush = os.path.join(dirToSearch, i)

        if os.path.isdir(toPush):
          dirList.append(toPush)
          stk.append(toPush)

        elif part in i:
	  self.fileList.append(toPush)

if __name__ == '__main__':
  if len(sys.argv) != 2:
    print "Usage: traverse.py <search term>"
    sys.exit()
  t = Traverse(os.getcwd())
  t.search(sys.argv[1])
  print t.getFiles()
