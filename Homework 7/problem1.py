def tree_IS(n):
	if n == 0:
		return 1
	elif n == 1:
		return 2
	else:
		return tree_IS(n - 1) ** 2 + tree_IS(n - 2) ** 4

print tree_IS(7)
#for i in range(9):
#	print i, tree_IS(i)

def line(n):
	if n == 0:
		return 1
	elif n == 1:
		return 2
	else:
		return line(n - 1) + line(n - 2)

# for i in range(9):
# 	print i, line(i)

def cycle(n):
	if n == 0:
		return 1
	elif n == 1:
		return 2
	elif n == 2:
		return 3
	else:
		return line(n-3) + line(n-1)

#for i in range(9):
#	print i, cycle(i)