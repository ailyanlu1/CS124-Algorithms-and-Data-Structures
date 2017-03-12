import sys
M = int(sys.argv[1])
total = 0
penalty = 0

with open("buffy_" + str(M) + ".txt", 'r') as f:
	for line in f:
		# remove new line character
		line = line[:-1]
		penalty = (M - len(line)) ** 3
		total += (M - len(line)) ** 3
# Exclude last line
print total - penalty