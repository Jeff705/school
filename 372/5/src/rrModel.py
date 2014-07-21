import sys, os
import DataRepo as repo
import xml.etree.cElementTree as ET

class RRModel:
  def __init__(self):
    self.onHand = repo.getRepo().getOnhand()
    self.allIng = repo.getRepo().getAlling()
    self.recipeDB = repo.getRepo().getDB()
    self.availableRecipes = self.query()
    self.dbChanged = False

# ingredient is to be passed as an element; this function just slaps it on the
# end of the onhand tree
  def addOnhand(self, ingredient):
    self.onHand.append(ingredient)

# ingredient here is passed as a string; it's easier to implement this way
# the arg to remove iterates over the onHand tree and finds an element with a
# matching name, saves the unique ID, then calls remove() using this ID
  def delOnhand(self, ingredient):
    ings = self.onHand.findall('Ingredient')
    for x in ings:
      if x.get('name') == ingredient:
        removeMe = x
    self.onHand.remove(removeMe)

# query() - runs a search on the recipe DB using the tree of onhand ingredients.
# returns a tree of available recipes.
  def query(self):
    inglist = []
    available = ET.Element('Available')
# build onhand ingredients as a list of strings
    for ingredient in self.onHand.findall('Ingredient'):
      inglist.append(ingredient.get('name'))
# in each recipe, compare each ingredient 'name' attribute to the list of onhand
# ingredients. if they're all there, 'unavailable' will remain false and the
# recipe element will be added to the tree of available recipes.
    for recipe in self.recipeDB.findall('Recipe'):
      unavailable = False
      for ing in recipe.findall('Ingredient'):
        if ing.get('name') not in inglist:
          unavailable = True
      if not unavailabe:
        available.append(recipe)
      return available
      
