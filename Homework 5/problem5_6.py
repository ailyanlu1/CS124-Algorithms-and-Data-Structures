import numpy as np
import random

#(x,e,m):
def mod_exp(b, p, m):
	res = 1
	prev = b
	while p > 0:
		if p % 2:
			res *= prev
			res %= m
			p -= 1
		else:
			prev *= prev
			prev %= m
			p /= 2
	return res


def fermat(n):
	for a in range(2, n):
		if gcd(a, n) == 1 and (mod_exp(a, n - 1, n) != 1):
			print "%d is a witness because %d ^ %d mod %d != 1 " % (a, a, n - 1, n)
			return False
	#print "%d is probably prime" % n
	return True

def gcd(a, b):
	if b == 0:
		return a
	else:
		return gcd(b, a % b)

def split(n):
	r = 0
	while n % 2 == 0:
		r += 1
		n /= 2
	return r, n

def extended_euclid(a,b):
	if b == 0: return (a, 1, 0)
	k = a / b
	(d, x, y) = extended_euclid(b, a % b)
	return (d, y, x - k*y)

def miller_rabin(n, k):
	if n <= 1: return False
	if n == 2 or n == 3: return True
	t, u = split(n - 1)
	a = -3
	for i in range(k):
		br = False
		a = random.randint(2, n - 2)
		x = mod_exp(a, u, n)
		if x == 1 or x == n - 1:
			continue
		for j in range(t - 1):
			x = mod_exp(x, 2, n)
			if x == 1:
				return a, j, False
			if x == n - 1:
				br = True
				break
		if br:
			continue
		return a, j, False 
	return True

# Question 5
print fermat(636127)
print fermat(294409)
print miller_rabin(294409, 10)

# Question 6
(n, e) = (46947848749720430529628739081,37267486263679235062064536973)

message = "Give me an A"
def str_to_num(string):
	res = 0
	n = len(string) - 1
	for c in string:
		res += ord(c) * ((2 ** 8) ** n)
		n -= 1
	return res

def num_to_str(num):
	res = ""
	while num > 0:
		c = num % 256
		res += chr(c)
		num /= 256
	return res[::-1]

def encrypt(message, n , e):
	x = str_to_num(message)
	return mod_exp(x, e, n)

def decrypt(encrypted, n, e):
	p = 114114114114263
	q = 411411411411487
	(d, x, y) = extended_euclid(e, (p-1)*(q-1))
	
	return mod_exp(encrypted, x, n)

print message
print str_to_num(message)
print encrypt(message, n , e)
print num_to_str(encrypt(message, n , e))
print num_to_str(decrypt(encrypt(message, n , e), n, e))
