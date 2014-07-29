import Tkinter as tk
import rrModel
import rrViews
import xml.etree.cElementTree as ET
import copy
import DataRepo as repo

class rrController:
    def __init__(self, root):
        self.dia = None
        self.model = rrModel.RRModel()
        self.mainview = rrViews.rrMainWindow(root)
        self.mainview.addIngredients.config(command = self.onhandDialog)
        self.mainview.newRecipe.config(command = self.newrecipeDialog)
        self.mainview.editmenu.add_command(label = 'New recipe', command = self.newrecipeDialog)
        self.mainview.recipeList.bind('<<ListboxSelect>>', self.showRecipeText)
        self.mainview.dispAll.trace("w", self.setRecipeViewMode)
        self.mainview.quitbutton.configure(command = self.saveExit)

        self.mainview.filemenu.add_command(label = 'Save changes', command = self.save)
        self.mainview.filemenu.add_command(label = 'Exit', command = self.saveExit)
        #at the end of initialization, tell program to update the recipe list
        self.updateAvailableRecipes()

    def save(self):
        onhand = self.model.getOnhand()
        repo.getRepo().writeOnhand(onhand)

    def saveExit(self):
        self.save()
        self.mainview.quit()

    def setRecipeViewMode(self, *args):
        mode = self.mainview.getDisplayMode()
        self.updateAvailableRecipes()

    def onhandDialog(self):
        self.dia = rrViews.rrIngDialog(self.mainview, title = 'Update Onhand Ingredients')
        self.dia.saveButton.config(command = self.dia.ok)
        self.dia.saveButton.bind('<Button-1>', self.saveOnhand)
        alling = copy.deepcopy(self.model.getAllIng())
        self.dia.populateLists(self.model.getOnhandList(), alling)
        self.dia.wait_window(self.dia)

    def saveOnhand(self, *args):
        onHand = self.dia.returnOnhand()
        root = ET.Element('OnHand')
        for ingredient in onHand:
            newElement = ET.Element('Ingredient', name = ingredient)
            root.append(newElement)
        self.model.setOnhand(root)
        self.updateAvailableRecipes()


    def updateAvailableRecipes(self, *args):
        mode = self.mainview.getDisplayMode()
        recipes = self.model.query(mode)
        values = []
        for child in recipes:
            values.append(child.get('title'))
        self.model.availableRecipes = values
        self.mainview.updateRecipes(values)

    def newrecipeDialog(self):
        dia = rrViews.rrAddRecipeDialog(self.mainview, title = 'Add a New Recipe')

    def showRecipeText(self, event):
        selection = event.widget.curselection()
        if selection:
            text = self.model.getRecipeText(event.widget.get(selection[0]))
            self.mainview.displayRecipe(text)


if __name__ == "__main__":
    root = tk.Tk()
    root.withdraw()
    controller = rrController(root)
    root.mainloop()