import sys, os
import DataRepo as repo
import xml.etree.cElementTree as ET
import Observable as obs


class RRModel:
    def __init__(self):
        self.onHand = obs.Observable(repo.getRepo().getOnhand())
        self.allIng = repo.getRepo().getAlling()
        self.recipeDB = obs.Observable(repo.getRepo().getDB())
        self.availableRecipes = obs.Observable(self.query())
        self.dbChanged = False

    # ingredient is to be passed as an element; this function just slaps it on the
    # end of the onhand tree
    def addOnhand(self, ingredient):
        tree = self.onHand.get()
        tree.append(ingredient)
        self.onHand.set(tree)
    # ingredient here is passed as a string; it's easier to implement this way
    # the arg to remove iterates over the onHand tree and finds an element with a
    # matching name, saves the unique ID, then calls remove() using this ID
    def delOnhand(self, ingredient):
        tree = self.onHand.get()
        ings = tree.findall('Ingredient')
        for x in ings:
            if x.get('name') == ingredient:
                removeMe = x
        tree.remove(removeMe)
        self.onHand.set(tree)
    # query() - runs a search on the recipe DB using the tree of onhand ingredients.
    # returns a tree of available recipes.
    def query(self):
        inglist = []
        available = ET.Element('Available')
        ohTree = self.onHand.get()
        recipeTree = self.recipeDB.get()
        # build onhand ingredients as a list of strings
        for ingredient in ohTree.findall('Ingredient'):
            inglist.append(ingredient.get('name'))
        # in each recipe, compare each ingredient 'name' attribute to the list of onhand
        # ingredients. if they're all there, 'unavailable' will remain false and the
        # recipe element will be added to the tree of available recipes.
        for recipe in recipeTree.findall('Recipe'):
            unavailable = False
            for ing in recipe.findall('Ingredient'):
                if ing.get('name') not in inglist:
                    unavailable = True
            if not unavailable:
                available.append(recipe)
            return available

    def getRecipeText(self, recipeName):
        dbTree = self.recipeDB.get()
        for recipe in dbTree.findall('Recipe'):
            if recipe.get('title') == recipeName:
                return self.buildRecipeBody(recipe)

    def buildRecipeBody(self, recipe):
        text = recipe.findtext('Text')
        ingredientHeader = []
        blurb = [recipe.findtext('Blurb'), '\n\n']
        for ingredient in recipe.findall('Ingredient'):
            ingredientHeader.append(ingredient.get('quantity'))
            ingredientHeader.append(ingredient.get('unit'))
            ingredientHeader.append(ingredient.get('name'))
            ingredientHeader.append(',')
            ingredientHeader.append(ingredient.get('mod'))
            ingredientHeader.append('\n')
        ingredientHeader.append('\n')
        recipeBody = [recipe.get('title'), '\n\n']
        recipeBody.extend(blurb)
        recipeBody.extend(ingredientHeader)
        recipeBody.append(text)
        recipeBody = ' '.join(recipeBody)
        return recipeBody

    def getOnhand(self):
        ingList = []
        tree = self.onHand.get()
        for child in tree:
            ingList.append(child.get('name'))
            return ingList

    def getAllIng(self):
        return self.allIng
