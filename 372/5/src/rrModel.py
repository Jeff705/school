#TODO: Figure out a valid way to remove Elements!

import sys, os
import DataRepo as repo

class RRModel:
  def __init__(self):
    self.onHand = repo.getRepo().getOnhand()
    self.allIng = repo.getRepo().getAlling()
    self.recipeDB = repo.getRepo().getDB()
    self.dbChanged = False

#ingredient is to be passed as an element
  def addOnhand(self, ingredient):
    self.onHand.append(ingredient)

#ingredient here is passed as a string; it's easier to implement this way
#the arg to remove iterates over the onHand tree and finds an element with a
  def removeOnhand(self, ingredient):
    self.onHand.remove(self.onHand.find("[@name = ingredient]"))
