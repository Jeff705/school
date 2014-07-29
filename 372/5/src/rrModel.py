import DataRepo as repo
import xml.etree.cElementTree as ET
import Observable as obs


class RRModel:
    def __init__(self):
        self.onHand = obs.Observable(repo.getRepo().getOnhand())
        self.allIng = repo.getRepo().getAlling()
        self.recipeDB = obs.Observable(repo.getRepo().getDB())
        self.availableRecipes = obs.Observable(self.query(False))
        self.dbChanged = False

    def setOnhand(self, root):
        self.onHand.set(root)
    # query() - runs a search on the recipe DB using the tree of onhand ingredients.
    # returns a list of available recipes.
    def query(self, viewAll):

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
            valid = True
            if not viewAll:
                for ing in recipe.findall('Ingredient'):
                    if ing.get('name') not in inglist:
                        valid = False
            if valid:
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
            if ingredient.get('mod'):
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

    def getOnhandList(self):
        ingList = []
        tree = self.onHand.get()
        for item in tree.findall('Ingredient'):
            ingList.append(item.get('name'))
        return ingList

    def getAllIng(self):
        return self.allIng
