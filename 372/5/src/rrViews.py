import Tkinter as tk
import sys, os
import rrModel

MAINGEOMETRY = '800x600+200+200'
INGGEOMETRY = '600x600'

class rrMainWindow(tk.Toplevel):
  def __init__(self, master):
    tk.Toplevel.__init__(self, master)
    self.protocol('WM_DELETE_WINDOW', self.master.destroy)
    self.geometry(MAINGEOMETRY)
    self.title('RRoundup')
    self.bind('<Escape>', lambda event: self.quit())
    
    self.selectFrame = tk.Frame(self)
    self.textFrame = tk.Frame(self)
    self.toolbarFrame = tk.Frame(self)
    
    self.recipeList = tk.Listbox(self.selectFrame)
    self.recipeList.bind('<Return>', self.displayRecipe)
    self.recipeList.config(height = 20)
    self.recipeList.pack(side = tk.TOP, pady = 4)
    self.recipeSearch = tk.Entry(self.selectFrame)
    self.recipeSearch.pack(side = tk.TOP)
    self.dispAll = tk.BooleanVar()
    self.dispCheck = tk.Checkbutton(self.selectFrame, text = 'Browse All', 
                                    variable = self.dispAll)
    self.dispCheck.pack(side = tk.TOP, pady = 4)
    self.recipeText = tk.Text(self.textFrame)
    self.recipeText.pack(padx = 5, pady = 5)

    self.toolbarFrame.pack(side = tk.BOTTOM, fill = tk.X)
    self.selectFrame.pack(side = tk.LEFT, padx = 5)
    self.textFrame.pack(side = tk.RIGHT)

    self.makeToolbar()
    self.makeMenu()

  def makeToolbar(self):
    addIngredients = tk.Button(self.toolbarFrame, text = 'On-hand',
                               command = self.changeOnhand)
    addIngredients.pack(side = tk.LEFT, padx = 5, pady = 5)
    newRecipe = tk.Button(self.toolbarFrame, text = 'Add Recipe',
                          command = self.addRecipe)
    newRecipe.pack(side = tk.LEFT, pady = 5)
    quit = tk.Button(self.toolbarFrame, text = 'Quit', command = self.quit)
    quit.pack(side = tk.RIGHT, padx = 5, pady = 5)

  def addRecipe(self):
    dia = rrAddRecipe(self, 'Add new recipe')

  def changeOnhand(self):
    dia = rrIngDialog(self, 'Update on-hand ingredients')

  def makeMenu(self):
    pass

  def updateRecipes(self, recipes):
    self.recipeList.delete(0, tk.END)
    for r in recipes:
      self.recipeList.insert(tk.END, r)

  def displayRecipe(self, event):
    pass

#This class taken from http://effbot.org/tkinterbook/tkinter-dialog-windows.htm
class Dialog(tk.Toplevel):
  def __init__(self, parent, title = None):

    tk.Toplevel.__init__(self, parent)
    self.transient(parent)

    if title:
      self.title(title)

    self.parent = parent

    self.result = None

    body = tk.Frame(self)
    self.initial_focus = self.body(body)
    body.pack(padx=5, pady=5)

    self.buttonbox()

    self.grab_set()

    if not self.initial_focus:
      self.initial_focus = self

    self.protocol("WM_DELETE_WINDOW", self.cancel)

    self.geometry("+%d+%d" % (parent.winfo_rootx()+50,
                              parent.winfo_rooty()+50))

    self.initial_focus.focus_set()

    self.wait_window(self)

  def body(self, master):
        # create dialog body.  return widget that should have
        # initial focus.  this method should be overridden

    pass

  def buttonbox(self):
        # add standard button box. override if you don't want the
        # standard buttons

    box = tk.Frame(self)

    w = tk.Button(box, text="OK", width=10, command=self.ok, default=tk.ACTIVE)
    w.pack(side=tk.LEFT, padx=5, pady=5)
    w = tk.Button(box, text="Cancel", width=10, command=self.cancel)
    w.pack(side=tk.LEFT, padx=5, pady=5)

    self.bind("<Return>", self.ok)
    self.bind("<Escape>", self.cancel)

    box.pack()

    #
    # standard button semantics

  def ok(self, event=None):

    if not self.validate():
      self.initial_focus.focus_set() # put focus back
      return

    self.withdraw()
    self.update_idletasks()

    self.apply()

    self.cancel()

  def cancel(self, event=None):

    # put focus back to the parent window
    self.parent.focus_set()
    self.destroy()

    #
    # command hooks

  def validate(self):

    return 1 # override

  def apply(self):

    pass # override

class rrAddRecipe(Dialog):
  def __init__(self, parent, title = None):
    Dialog.__init__(self, parent, title)
    #TODO: MAKE THE ACTUAL DIALOG

class rrIngDialog(Dialog):
  def __init__(self, parent, title = None):
    Dialog.__init__(self, parent, title)

if __name__ == "__main__":
  root = tk.Tk()
  root.withdraw()
  window = rrMainWindow(root)
  model = rrModel.RRModel()
  window.updateRecipes(['stuff', 'otherstuff'])
  window.mainloop()
