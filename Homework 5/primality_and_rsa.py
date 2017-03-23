import numpy as np

def fermat_test(a, p):
	return (1 == pow(a, p - 1, p))