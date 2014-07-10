#!/usr/bin/python
import Tkinter as tk
import sys, os

GEOMETRY = '300x300+200+200'

root = tk.Tk()
root.geometry(GEOMETRY)
root.bind('<Escape>', lambda event: root.quit())
button = tk.Button(root, text = 'I\'m a button!')
button.config(command = sys.exit, relief = tk.RAISED)
button.config(width = 10, height = 2, bg = "#ff0000", bd = 5)
button.pack(side = tk.BOTTOM)
frame = tk.Frame(root)
frame.pack()
label1 = tk.Label(frame, text = "Hello World!")
label2 = tk.Label(frame, text = "Seriously, hi!")
label1.pack(side = tk.LEFT, padx = 10, pady = 20)
label2.pack(side = tk.RIGHT, padx = 10)

tk.mainloop()



