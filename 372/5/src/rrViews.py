import Tkinter as tk
import sys, os
import rrModel

MAINGEOMETRY = ('800x600+200+200')
INGGEOMETRY = ('600x600')

class rrMainWindow:
  def __init__(self):
    self.root = tk.Tk()

  def mainloop(self)
    self.root.mainloop()

class rrIngDialog:
  def __init__(self):
    pass

class rrAddRecipe:
  def __init__(self):
    pass

#This class taken from http://effbot.org/tkinterbook/tkinter-dialog-windows.htm
class Dialog(Toplevel):
  def __init__(self, parent, title = None):

    Toplevel.__init__(self, parent)
    self.transient(parent)

    if title:
      self.title(title)

    self.parent = parent

    self.result = None

    body = Frame(self)
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

class rrAddRecipe(Dialog):
  def __init__(self, parent, title = None):
    Dialog.__init__(self, parent, title)
    #TODO: MAKE THE ACTUAL DIALOG
