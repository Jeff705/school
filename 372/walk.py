import os
import sys
import stack

def walk(dir, mystery):
  stk = stack.stack()
  fileList = []
  dirList = [dir]
  stk.push(dir)
  mysteryList = []
  while not stk.empty():
    dirToSearch = stk.top()
    stk.pop()
    thisDirectory = os.listdir(dirToSearch)
    
    for i in thisDirectory:
      toPush = os.path.join(dirToSearch, i)
      if os.path.isdir(toPush):
        dirList.append(toPush)
        stk.push(toPush)
      else:
        fileList.append(toPush)
 
  for f in fileList: 
    if mystery in f:
      mysteryList.append(f)
  print dirList
  print len(dirList), "directories"
  print fileList
  print len(fileList), "files"
  print "found ", mysteryList
  print len(mysteryList), "occurences"
if __name__ == "__main__":
  if len(sys.argv) != 3:
    print "usage:", sys.argv[0], "<dir to search>"
  walk(sys.argv[1], sys.argv[2])
