import sys, os
import DataRepo as repo
class RRModel:
  def __init__(self):
    self.onHand = getRepo().getOnhand()
    self.allIng = getRepo().getAlling()
    self.recipeDB = getRepo().getDB()
    self.dbChanged = False

  def 
