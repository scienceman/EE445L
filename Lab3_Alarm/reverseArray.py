# Kevin Gilbert
# 8 February 2014

# File paths
inputFile = 'fullClock1.txt'
outputFile = 'ReversedFullClock1.txt'

writeFile = open(outputFile, 'w')
array = list(reversed(list(open(inputFile))))
writeFile.writelines(array)
writeFile.close()