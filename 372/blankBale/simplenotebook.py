#!/usr/bin/python

import Tkinter as tk
import ttk
FRGEO = '400x300'
root = tk.Tk()
root.geometry('640x480')

child1 = tk.Frame(root)
child2 = tk.Frame(root)
child3 = tk.Frame(root)

firstlabel = tk.Label(child1, text = "First!")
firstlabel.config(height = 5, width = 10, bg = "#00FF00")
firstlabel.pack()
child1.pack()
child2.pack()
child3.pack()
notebook = ttk.Notebook(root, height = 400, width = 300)

notebook.add(child1, text = "first tab")


notebook.add(child2, text = "second tab")
notebook.add(child3, text = "third tab")


notebook.pack(side = tk.LEFT, padx = 30)
root.mainloop()

