import scipy.misc as sm
import numpy as np
import random
import sys
from scipy.stats import rv_discrete

def choose(n,k):
	return sm.comb(n,k)

N = 10 ** 9	
B = 10 ** 9	
ways = choose(N + B - 1, B - 1)

# Probability that a bucket has L balls in it when there are 
# N balls and B buckets
def p(L):
	return choose(N, L) * (1.0 / B) ** L * (1 - 1.0 / B) ** (N - L)

def fac(n):
	i = 1
	res = 1.0
	while i <= n:
		res *= i
		i += 1
	return res

probs = [p(i) for i in range(39)]
probs2 = [np.e ** -1 * fac(i) ** -1 for i in range(39)]

"""
for i in range(len(probs)):
 	print probs[i], probs2[i]

print sum(probs2)

#print probs2

avg = 0
trials = 1.0 #1000.0
for j in range(int(trials)):
	results =  np.random.multinomial(N, probs2, size = 1)
	max_i = -4
	for i in range(len(results[0])):
		if results[0][i] != 0:
			max_i = i
	avg += max_i
	print results
	t = 0
	for r in range(len(results[0])):
		t += r * results[0][r]
	print t


print avg / trials
"""


stored = 15
balls = float(10 ** 6)
bins = [0 for i in range(stored)]
nums = [i for i in range(stored)]
bins[0] = balls
probs = [0 for i in range(stored)]
probs[0] = 1.0

def lst_2_probs(lst, probs):
	for i in range(stored):
		probs[i] = lst[i] / balls
	return probs

i = 0
while i < balls:
	probs = lst_2_probs(bins, probs)
	new_count = np.random.choice(stored, 1, p=probs)[0] #rv_discrete(name='custm', values=(nums, probs)).rvs(size = 1)[0]
	bins[new_count + 1] += 1
	bins[new_count] -= 1
	i += 1
	if i % 10 ** 7 == 0:
		print i

print bins

#################################### Part 2 ###################################


