# Telt het aantal voorkomens van een cijfer in een interval.
limit = 2**63 # Deze kan heel lang duren...
count = 0
charToCount = '0'
for i in range(limit+1):
	count += str(i).count(charToCount)
print("karakter:" + charToCount + "komt zoveel keer voor:"+ str(count))