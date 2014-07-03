#!/usr/bin/python
# A program to generate directorys and
# files to test file maintenance programs.

import os
import sys
import gzip
import random
EXTENSIONS = ['o', '.pyc', '.txt', '.cpp', '.py', '.c']
MAX_MYSTERY_WORDS = 20

class Generator(object):
  def __init__(self, dir):
    self.newdir = dir
    self.errorFlag = False
    self.__noDirs = 0
    self.__noFiles = 0
    self.pwd = os.getcwd() 
    self.words = []
    self.files = []
    self.mysteryFile = ''
    self.mysteryWords = []
    dict = gzip.open ('webster.txt.gz')
    for line in dict:
      self.words.append( line.rstrip('\n') )
  def getNoDirs(self): return self.__noDirs
  def getNoFiles(self): return self.__noFiles

  def __repr__(self):
    if self.errorFlag: 
      return "No directory or files created!"
    else:
      return "\tNew directory is "+self.newdir+'\n' \
      +"\tGenerated "+str(self.__noDirs)+" Subdirectories" +'\n' \
      +"\tGenerated "+str(self.__noFiles)+" Files" +'\n'

  def writeFile(self, filename):
    self.files.append(filename)
    FILE = open(filename,"w")
    self.__noFiles += 1
    n = random.randint(5,10)
    for x in range(1, n):
      w= random.randint(0, len(self.words))
      word = self.words[w]
      FILE.write(word)
      FILE.write(' ')
    FILE.write('\n')
    FILE.close()

  def writeMysteryFile(self):
    toDoThis = random.randint(0,2)
    if not toDoThis:
      return
    self.files.append(self.mysteryFile)
    FILE = open(self.mysteryFile,"w")
    self.__noFiles += 1
    for word in self.mysteryWords:
      FILE.write(word)
      FILE.write(' ')
    FILE.write('\n')
    FILE.close()

  def makeRepeatFileName(self):
    n = random.randint(3,7)
    w = random.randint(0, len(self.words))
    name = self.words[w]
    self.mysteryFile = name+'.mys'
    max = random.randint(10, MAX_MYSTERY_WORDS)+10
    for index in range(10, max):
      w = random.randint(0, len(self.words))
      word = self.words[w]
      self.mysteryWords.append(word)
    print "Mystery file name:", self.mysteryFile
    #print self.mysteryWords

  def makeFiles(self, extension):
    n = random.randint(3,7)
    for x in range(2,n):
      w = random.randint(0, len(self.words))
      name = self.words[w]
      newfile = name+extension
      self.writeFile(newfile)

  def generate(self):
    if os.path.isdir(self.newdir) or os.path.isfile(self.newdir):
      print self.newdir, \
        " already exists, choose a non-existing directory."
      self.errorFlag = True
      return
    self.makeRepeatFileName()
    dir = self.newdir
    for x in range(1,4):
      os.mkdir(dir)
      self.__noDirs += 1
      os.chdir(dir)
      self.writeMysteryFile()
      for extension in EXTENSIONS:
        self.makeFiles(extension)
      n = random.randint(0, len(self.words))
      dir = self.words[n]

    os.chdir(self.pwd)


if __name__ == "__main__":
  if len(sys.argv) != 2:
    print "usage: ", sys.argv[0], " <start dir>"
    sys.exit()
  generator = Generator(sys.argv[1])
  generator.generate()
  print generator
  #print "The", len(generator.files), "files are:"
  #files= generator.files
  #files.sort()
  #print files
