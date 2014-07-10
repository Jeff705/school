import Tkinter as tk
import ttk

class Note(tk.Frame):
  def makeBook(self, tabs=None):
    self.book = ttk.Notebook(self)
    for t in tabs:
      win = tk.Tk()
      tk.Label(t).pack(win)
      self.book.add(win)

  def __init__(self, root, tabs=None):
    tk.Frame.__init__(self, root)
    self.pack()
    self.makeBook(tabs)

tabs = ['one', 'two', 'three']
root = tk.Tk()
app = Note(root, tabs)
app.mainloop()
