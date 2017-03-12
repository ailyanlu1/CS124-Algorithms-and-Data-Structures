import sys

filename = sys.argv[1]
M = int(sys.argv[2])
lengths = []
N = 0
infinity = float("inf")

def error(i,j):
	if i > j:
		return infinity
	penalty = (M - j + i - sum(lengths[i:j + 1])) ** 3
	if penalty < 0:
		return infinity
	elif j == N - 1 and penalty >= 0:
		return 0
	else:
		return penalty

words = []
	
# Read in Data
with open(filename, 'r') as f:
	for line in f:
		for word in line.split():
			words.append(word)
			lengths.append(len(word))
			N += 1

# Calculate potential_costs of each sequence of words
potential_costs = []
for i in range(N):
	potential_costs.append([])
	for j in range(N):
		potential_costs[i].append(error(i,j))


linebreaks = [-200 for i in range(N)]
optimal_costs = {-1 : 0}
linebreaks[0] = -1
i = 0

for j in range(N):
	optimal_costs[j] = infinity
	for i in range(j):
		if optimal_costs[i-1] + potential_costs[i][j] < optimal_costs[j]:
			optimal_costs[j] = optimal_costs[i-1] + potential_costs[i][j]
			linebreaks[j] = i - 1

index = N - 1
breaks = []
while index != -1:
	breaks.append(index)
	index = linebreaks[index]

breaks = breaks[::-1]
#print breaks

res = ""
break_count = 0

for i in range(N):
	res += words[i]
	if i == breaks[break_count]:
		res += "\n"
		break_count += 1
	else:
		res += " "
#print optimal_costs
print res
