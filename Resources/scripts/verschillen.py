f=open("medium.txt",'r');
lns = [int(x) for x in f.read()[1:-1].split(',')]
f.close()
#
lins.insert(0,0)
delta=[]
for i in range(1,len(lns)):
	delta.append(lns[i]-lns[i-1])
#
#f=open("outputmed.txt","w")
outc= "" #outputControl
ctrlchr = 0 # currentcontrolchar
bitlen=1
for d in delta:
	while(2**bitlen-1<d):
		bitlen *= 2
		outc.append(ctrlchr)