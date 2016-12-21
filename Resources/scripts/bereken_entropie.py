#!/usr/bin/python
# Author: Domien
# Date: 1/11/2016

import sys
from math import log

map = dict()
if(len(sys.argv) != 2):
	print("Gebruik: "+sys.argv[0]+" <bestand>",file=sys.stderr)
	sys.exit(1)

f=open(sys.argv[1],"r")
c=f.read(1)
totaal=0
while c:
	if(c not in map):
		map[c] = 0
	map[c] += 1
	totaal += 1
	c = f.read(1)
f.close()

entropie = 0
print("Kansen per karakter (totaal="+str(totaal)+"):")
for key in sorted(map):
	print(key+" : "+str(map[key]))
	kans = map[key]/totaal
	entropie += kans * log(kans,2)
print("Entropie = "+str(-entropie))