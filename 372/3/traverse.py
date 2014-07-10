import os
import sys
#Obervable was shamelessly taken from the Toy MVC in the project spec
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
    tk.Toplevel.__init__(self, root)
    self.protol('WM_DELETE_WINDOW', self.master.destroy)
    tk.Label(Graphical file locator 


class Controller(object):
  def __init__(self):
    pass
  
  def hitsChanged(self, hits):
    self.view.updateHits(hits)
 




if __name__ == '__main__':
  pass
