import Tkinter as tk
import rrModel
import ScrolledText as ST

MAINGEOMETRY = '800x450+200+200'
INGGEOMETRY = '400x600'

class rrMainWindow(tk.Toplevel):
    def __init__(self, master):
        tk.Toplevel.__init__(self, master)
        self.protocol('WM_DELETE_WINDOW', self.master.destroy)
        self.geometry(MAINGEOMETRY)
        self.title('RRoundup')

        self.selectFrame = tk.Frame(self)
        self.textFrame = tk.Frame(self)
        self.toolbarFrame = tk.Frame(self)
        self.recipeLabelFrame = tk.Frame(self.selectFrame, height = 1)
        self.textLabelFrame = tk.Frame(self.textFrame, height = 1)
        self.dispAll = tk.BooleanVar()
        self.dispCheck = tk.Checkbutton(self.selectFrame, text = 'Browse All',
                                        variable = self.dispAll)
        self.dispCheck.pack(side = tk.BOTTOM, pady = 4)

        self.recipeSearch = tk.Entry(self.selectFrame)
        self.recipeSearch.pack(side = tk.BOTTOM)

        self.recipeLabelFrame.pack(side = tk.TOP, fill = tk.X)

        self.recipeList = tk.Listbox(self.selectFrame)
        recipeBar = tk.Scrollbar(self.selectFrame, command = self.recipeList.yview)
        self.recipeList.config(height = 19, yscrollcommand = recipeBar.set)
        self.recipeList.pack(side = tk.LEFT, pady = 4, fill = tk.Y)
        recipeBar.pack(side = tk.RIGHT, fill = tk.Y)

        self.textLabelFrame.pack(side = tk.TOP, fill = tk.X)

        self.recipeText = ST.ScrolledText(self.textFrame, wrap = tk.WORD)
        self.recipeText.pack(padx = 5, pady = 5)

        self.listLabel = tk.Label(self.recipeLabelFrame, text = 'Recipes')
        self.listLabel.pack(side = tk.LEFT)

        self.recipeLabel = tk.Label(self.textLabelFrame, text = 'Full text')
        self.recipeLabel.pack(side = tk.LEFT, padx = 10)

        self.toolbarFrame.pack(side = tk.BOTTOM, fill = tk.X)
        self.selectFrame.pack(side = tk.LEFT, padx = 5)
        self.textFrame.pack(side = tk.RIGHT)

        self.makeToolbar()
        self.makeMenu()

    def getDisplayMode(self):
        return self.dispAll.get()

    def makeToolbar(self):
        self.addIngredients = tk.Button(self.toolbarFrame, text = 'On-hand')
        self.addIngredients.pack(side = tk.LEFT, padx = 5, pady = 5)
        self.newRecipe = tk.Button(self.toolbarFrame, text = 'Add Recipe')
        self.newRecipe.pack(side = tk.LEFT, pady = 5)
        self.quitbutton = tk.Button(self.toolbarFrame, text = 'Quit')
        self.quitbutton.pack(side = tk.RIGHT, padx = 5, pady = 5)

    def newRecipe(self):
        dia = rrAddRecipeDialog(self, 'Add new recipe')

    def changeOnhand(self):
        dia = rrIngDialog(self, 'Update on-hand ingredients')

    def makeMenu(self):
        bar = tk.Menu(self)
        self.filemenu = tk.Menu(bar, tearoff = 0)
        bar.add_cascade(menu = self.filemenu, label = 'File')
        self.editmenu = tk.Menu(bar, tearoff=0)
        bar.add_cascade(menu = self.editmenu, label = 'Edit')
        aboutmenu = tk.Menu(bar, tearoff=0)
        aboutmenu.add_command(label='Recipe Roundup', command=self.displayAbout)
        bar.add_cascade(menu=aboutmenu, label='About')
        self.config(menu=bar)

    def displayAbout(self):
        dia = rrAboutDialog(self, 'About Recipe Roundup')

    def updateRecipes(self, recipes):
        self.recipeList.delete(0, tk.END)
        for r in recipes:
            self.recipeList.insert(tk.END, r)

    def displayRecipe(self, text):
        self.recipeText.delete(1.0, tk.END)
        self.recipeText.insert(1.0, text)

    def saveCommand(self):
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

        self.body = tk.Frame(self)
        self.initial_focus = self.makebody(self.body)
        self.body.pack(padx=5, pady=5)

        self.buttonbox()

        self.grab_set()

        if not self.initial_focus:
            self.initial_focus = self

        self.protocol("WM_DELETE_WINDOW", self.cancel)

        self.geometry("+%d+%d" % (parent.winfo_rootx()+50, parent.winfo_rooty()+50))

        self.initial_focus.focus_set()

    def makebody(self, master):
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

class rrAddRecipeDialog(Dialog):
    def __init__(self, parent, title = None):
        Dialog.__init__(self, parent, title)

class rrIngDialog(Dialog):
    def __init__(self, parent, title = None):
        Dialog.__init__(self, parent, title)
#don't need a buttonbox for this subclass of dialog
    def buttonbox(self):
        pass

    def makebody(self, master):
        allFrame = tk.Frame(master)
        onHandFrame = tk.Frame(master)
        addremButtonFrame = tk.Frame(master)
        spacerFrame = tk.Frame(master)
        cmdFrame = tk.Frame(master)
        labelFrame = tk.Frame(master)


        self.allIng = tk.Listbox(allFrame)
        allBar = tk.Scrollbar(allFrame)
        self.allIng.config(yscrollcommand = allBar.set)
        allBar.config(command = self.allIng.yview)

        allLabel = tk.Label(labelFrame, text = 'All ingredients')
        allLabel.pack(side = tk.LEFT, padx = 5, pady = 5)

        self.onHandIng = tk.Listbox(onHandFrame)
        onHandBar = tk.Scrollbar(onHandFrame)
        self.onHandIng.config(yscrollcommand = onHandBar.set)
        onHandBar.config(command = self.onHandIng.yview)

        onHandLabel = tk.Label(labelFrame, text = 'On-hand ingredients')
        onHandLabel.pack(side = tk.RIGHT, padx = 5, pady = 5)

        addButton = tk.Button(addremButtonFrame,text = '>>', command = self.add)
        remButton = tk.Button(addremButtonFrame, text = '<<', command = self.remove)

        self.saveButton = tk.Button(cmdFrame, text = 'Save')
        cancelButton = tk.Button(cmdFrame,text = 'Cancel',command = self.cancel)

        labelFrame.pack(fill = tk.X)
        allFrame.pack(side = tk.LEFT)
        onHandFrame.pack(side = tk.RIGHT)
        addremButtonFrame.pack()
        cmdFrame.pack(side = tk.BOTTOM)
        spacerFrame.pack(fill = tk.Y)
        self.allIng.pack(side = tk.LEFT)
        allBar.pack(side = tk.RIGHT, fill = tk.Y)
        self.onHandIng.pack(side = tk.LEFT)
        onHandBar.pack(side = tk.RIGHT, fill = tk.Y)
        addButton.pack()
        remButton.pack()
        cancelButton.pack(side = tk.BOTTOM)
        self.saveButton.pack(side = tk.BOTTOM)

    def populateLists(self, onHand, allIng):
        self.allIng.delete(0, tk.END)
        self.onHandIng.delete(0,tk.END)
        onHand.sort()
        allIng.sort()
        for ingredient in onHand:
            self.onHandIng.insert(tk.END, ingredient)
            if ingredient in allIng:
                allIng.remove(ingredient)
        for ingredient in allIng:
            self.allIng.insert(tk.END, ingredient)

    def returnOnhand(self):
        return list(self.onHandIng.get(0,tk.END))

    def apply(self):
        #notify controller that stuff happened, pass it the onhand list box
        pass
    def add(self):
        allIngList = list(self.allIng.get(0,tk.END))
        onHandList = list(self.onHandIng.get(0,tk.END))
        selection = self.allIng.curselection()
        toMove = self.allIng.get(selection[0])
        allIngList.remove(toMove)
        onHandList.append(toMove)
        onHandList.sort()
        self.populateLists(onHandList, allIngList)


    def remove(self):
        allIngList = list(self.allIng.get(0,tk.END))
        onHandList = list(self.onHandIng.get(0,tk.END))
        selection = self.onHandIng.curselection()
        toMove = self.onHandIng.get(selection[0])
        onHandList.remove(toMove)
        allIngList.append(toMove)
        allIngList.sort()
        self.populateLists(onHandList, allIngList)


    def ok(self, event = None):
        self.apply()
        self.cancel()

class rrAboutDialog(Dialog):
    def __init__(self, parent, title = None):
        Dialog.__init__(self, parent, title)

if __name__ == "__main__":
    root = tk.Tk()
    root.withdraw()
    window = rrMainWindow(root)
    model = rrModel.RRModel()
    window.mainloop()
