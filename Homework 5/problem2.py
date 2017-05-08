import scipy.misc as sm

def p(m,n,k,b):
	total = 0
	m = float(m)
	k = float(k)
	n = float(n)
	b = float(b)
	for i in range(0, int(2 ** b - 1)):
		total += sm.comb(n, i) * (k/m)**i * (1 - k/m) ** (n - i)
		#print total
	return 1 - total

print p(10 ** 6, 10 ** 5, 7, 3)
print p(10 ** 6, 10 ** 5, 7, 4)
print p(10 ** 6, 10 ** 5, 7, 5)