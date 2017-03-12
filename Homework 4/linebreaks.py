import sys

filename = sys.argv[1]
M = int(sys.argv[2])
N = 0
infinity = float("inf")
lengths = []
words = []
	
# Read in Data
with open(filename, 'r') as f:
	for line in f:
		for word in line.split():
			words.append(word)
			lengths.append(len(word))
			N += 1

linebreaks = [i - 1 for i in range(N)]
optimal_costs = {-1 : 0}
linebreaks[0] = -1

def get_cost(spaces,i,j):
	if spaces < 0:
		return infinity
	elif j == N - 1:
		return 0
	else:
		return spaces ** 3

for j in range(N):
	optimal_costs[j] = optimal_costs[j-1] + get_cost((M - lengths[j]) ** 3,j,j)
	spaces = M - lengths[j]
	lower_bound = max(j - M/2 - 1, -1)
	for i in range(j-1,lower_bound,-1):
		spaces = spaces - lengths[i] - 1
		if optimal_costs[i-1] + get_cost(spaces,i,j) < optimal_costs[j]:
			optimal_costs[j] = optimal_costs[i-1] + get_cost(spaces,i,j)
			linebreaks[j] = i - 1

index = N - 1
breaks = []
while index != -1:
	breaks.append(index)
	index = linebreaks[index]

breaks = breaks[::-1]

res = ""
break_count = 0

for i in range(N):
	res += words[i]
	if i == breaks[break_count]:
		res += "\n"
		break_count += 1
	else:
		res += " "

print res,
print optimal_costs[N - 1]