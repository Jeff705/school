import traverse as tv
import sys
import os

t = tv.Traverse(os.getcwd())
if len(sys.argv) != 2:
  print "Usage: search.py <search term>"
  sys.exit()
token = sys.argv[1]

t.search(token)
print "Search for", token, "returned:"
print t.getFiles()
print "A total of" , len(t.getFiles()), "occurrences."
