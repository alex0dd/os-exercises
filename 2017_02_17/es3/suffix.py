import os
from pathlib import Path

nosuffix = []
suffix = {}
for root, dirs, files in os.walk("."):
    for filename in files:
      path = Path(filename)
      if (len(path.suffix)>0):
        suffix.setdefault(path.suffix,[])
        suffix[path.suffix].append(filename)
      else:
        nosuffix.append(filename)
for k in suffix.keys():
    print(k, end=": ")
    for v in suffix[k]:
        print(v,end=" ")
    print("")
for v in nosuffix: print(v, end=" ")