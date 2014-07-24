import sys, os
import DataRepo
import Observable
import Tkinter as tk
import rrModel
import rrViews

class rrController:
    def __init__(self, root):
        self.model = rrModel.RRModel()
        self.mainview = rrViews.rrMainWindow(root)
        self.mainview.addIngredients.config(command = self.onhandDialog)
        self.mainview.newRecipe.config(command = self.newrecipeDialog)
        self.mainview.editmenu.add_command(label = 'New recipe', command = self.newrecipeDialog)
        self.mainview.recipeList.bind('<Return>', self.showRecipeText)


        #at the end of initialization, tell program to update the recipe list
        self.onhandUpdated()


    def onhandDialog(self):
        dia = rrViews.rrIngDialog(self.mainview, title = 'Update Onhand Ingredients')
        dia.saveButton.config(command = dia.ok)
        dia.saveButton.bind('<Button-1>', self.onhandUpdated())
        dia.wait_window(dia)

    def onhandUpdated(self):
        recipes = self.model.query()
        values = []
        for r in recipes:
            values.append(r.get('title'))
        self.mainview.updateRecipes(values)

    def newrecipeDialog(self):
        dia = rrViews.rrAddRecipeDialog(self.mainview, title = 'Add a New Recipe')

    def showRecipeText(self, event):
        selection = event.widget.curselection()
        text = self.model.getRecipeText(event.widget.get(selection[0]))
        self.mainview.displayRecipe(text)


if __name__ == "__main__":
    root = tk.Tk()
    root.withdraw()
    controller = rrController(root)
    root.mainloop()