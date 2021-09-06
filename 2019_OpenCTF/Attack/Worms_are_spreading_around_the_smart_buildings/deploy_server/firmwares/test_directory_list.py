import os
for subdir, dirs, files in os.walk('../'):
    for file in files:
        if ".py" in file:
            print("File = {}".format(file))
      # content = open(file, "r").read()
      # print("Content = {}".format(content))


content = open("api.pyc", "r").read()
print("Content = {}".format(content))
