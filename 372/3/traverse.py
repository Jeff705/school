import os
import sys

GEOMETRY = '640x480+400+400'
#Observable was shamelessly taken from the Toy MVC in the project spec
class Observable:
  def __init__(self, initValue=None):
    self.data = initValue
    self.callbacks = {}

  def addCallback(self, func):
    self.callbacks[func] = 1

  def delCallback(self, func):
    del self.callback[func]

  def _docallbacks(self):
    for func in self.callbacks:
      func(self.data)

  def set(self, data):
    self.data = data
    self._docallbacks()

  def get(self):
    return self.data

  def unset(self):
    self.data = None

class Model(object):

  def __init__(self):
    self.hits = Observable([]) 
    self.directory = os.getcwd()

  def clear(self):
    self.set([])
#UNUSED AS OF YET, WILL BE MADE FUNCTIONAL LATER
  def newDir(self, newDirectory):
    self.directory = newDirectory

  def search(self, part):
    self.clear()
    subdirs = [self.directory]
    tempHits = []
    while subdirs:
      dirToSearch = subdirs.pop()
      thisDirectory = os.listdir(dirToSearch)

      for i in thisDirectory:
        toPush = os.path.join(dirToSearch, i)

        if os.path.isdir(toPush):
          subdirs.append(toPush)
          
        if part in i:
	  tempHits.append(toPush)
      
      self.hits.set(tempHits)

class View(tk.Toplevel):
  def __init__(self, root):
    tk.Toplevel.__init__(self, master)
    self.protocol('WM_DELETE_WINDOW', self.master.destroy)
    self.geometry(GEOMETRY)
    self.bind("<Escape>", lambda event: self.quit())
    label = tk.Label(self, text = "Graphical file locator")
    label.pack()
    self.fileList = tk.Listbox(self)
    self.fileList.pack()
    bottomFrame = tk.Frame(self, height = 50)
    bottomFrame.pack(side = tk.BOTTOM)
    self.entryBox = tk.Entry(bottomFrame, width = 320)
    self.entryBox.pack(side = tk.LEFT)
    self.clearButton = tk.Button(bottomFrame, text = "Clear")
    self.clearButton.config(width = 100, height = 40)
    self.clearButton.pack(side = tk.LEFT, padx = 10)
    self.searchButton = tk.Button(bottomFrame, text = "Search")
    self.searchButton.config(width = 100, height = 40)
    self.searchButton.pack(side = tk.LEFT, padx = 10)


class Controller(object):
  def __init__(self, root):
    pass
  
  def hitsChanged(self, hits):
    self.view.updateHits(hits)
 




if __name__ == '__main__':
  pass
