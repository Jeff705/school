#TODO: implement write functions for DataRepo

import os
import xml.etree.cElementTree as ET

ONHAND = 'onhand.xml'
DB = 'recipeDB.xml'

''' DataRepo: a Singleton containing a working directory, file, and elementTrees
    for the recipe database and on-hand ingredients... and, of course, the
    associated data handling functions. '''

class DataRepo(BaseException):
  __repo = None
  def __init__(self):
    if DataRepo.__repo:
      raise DataRepo.__repo
    DataRepo.__repo = self
    startdir = os.getcwd()
    os.chdir('..')
    self.directory = os.getcwd()
    self.workingFile = None
    self.onhand = self.loadOnhand()
    self.recipedb = self.loadDB()
    self.alling = self.loadAlling()
    os.chdir(startdir)

  def loadOnhand(self):
    os.path.walk(self.directory, findFile, ONHAND)
    if ONHAND in self.workingFile:
      data = ET.parse(self.workingFile).getroot()
    else:
      data = ET.Element('OnHand')
      print "Failed to find a working file, starting with a blank tree"
    return data

#builds a list of ingredients from all recipes in the recipe DB.
#returns a sorted list of ingredients

  def loadAlling(self):
    ingredients = []
    db = self.getDB()
    for recipe in db.iter('Recipe'):
      ing = recipe.findall('Ingredient')
      for each in ing:
        if not each.get('name') in ingredients:
          ingredients.append(each.get('name'))
    ingredients.sort()
    print "loaded ingredients:", ingredients
    return ingredients

  def loadDB(self):
    os.path.walk(self.directory, findFile, DB)
    if DB in self.workingFile:
      data = ET.parse(self.workingFile).getroot()
    else:
      data = ET.Element("RecipeDB")
      print "DB load from file failed, creating blank recipe database"
    return data  

  def getOnhand(self):
    return self.onhand

  def getAlling(self):
    return self.alling

  def getDB(self):
    return self.recipedb

  def writeOnhand(self):
    pass

  def writeDB(self):
    pass

def findFile(fname, dirname, namelist):
  for n in namelist:
    if fname == n:
      getRepo().workingFile = os.path.join(dirname, n)
      
def getRepo():
  try:
    repo = DataRepo()
  except DataRepo, r:
    repo = r
  return repo


if __name__ == "__main__":
  repo = getRepo() 
