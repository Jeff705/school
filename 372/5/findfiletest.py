import os, sys
import src.DataRepo as repo
filename = sys.argv[1]

def findfile(filename, directory, namelist):
	for n in namelist:
		if filename == n:
			repo.getRepo().workingFile = n 

os.path.walk(os.getcwd(), findfile, filename)


print repo.getRepo().workingFile
