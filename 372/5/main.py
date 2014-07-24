#!/usr/bin/python

import Tkinter as tk
import src.DataRepo
import src.Observable
import src.rrController as rr
import src.rrModel
import src.rrViews

root = tk.Tk()
root.withdraw()
controller = rr.rrController(root)
root.mainloop()
