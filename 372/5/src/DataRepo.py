#TODO: write load and write functions for DataRepo

import sys, os
import xml.etree.cElementTree as ET

class DataRepo(BaseException):
  __repo = None
  def __init__(self):
    if DataRepo.__repo:
      raise DataRepo.__repo
    DataRepo.__repo = self
    self.directory = os.getcwd()
    os.chdir('..')
    self.workingFile = None
    self.onhand = self.loadOnhand()
    self.alling = self.loadAlling()
    self.recipedb = self.loadDB()
    os.chdir(self.directory)

  def loadOnhand(self):
    onhand = os.path.walk(self.directory, findFile(), 'onhand.xml')

  def loadAlling(self):
    pass

  def loadDB(self):
    pass

  def getOnhand(self):
    return self.onhand

  def getAlling(self):
    return self.alling

  def getDB(self):
    return self.recipedb

  def writeOnhand(self, onHand):
    pass

  def writeDB(self, DB):
    pass

def findFile(fname, dirname, namelist)
  for n in namelist
def getRepo():
  try:
    repo = DataRepo()
  except DataRepo, r:
    repo = r
  return repo


if __name__ == "__main__":
  a = getRepo()
  b = getRepo()
  print  "ids of a and b are:", ( id(a), id(b) ) 
